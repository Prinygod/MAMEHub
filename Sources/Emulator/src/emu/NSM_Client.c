#include <stdexcept>

#include "RakNet/RakPeerInterface.h"
#include "RakNet/RakNetStatistics.h"
#include "RakNet/RakNetTypes.h"
#include "RakNet/BitStream.h"
#include "RakNet/PacketLogger.h"
#include "RakNet/RakNetTypes.h"
#include "RakNet/RakSleep.h"

#include "google/protobuf/io/lzma_protobuf_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"

#include "NSM_Client.h"

#include "NSM_Server.h"

#include "gen-cpp/MameHubRpc.h"

#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TJSONProtocol.h>

#include <assert.h>
#include <cstdio>
#include <cstring>
#include <stdlib.h>

#define NO_MEM_TRACKING
#include "osdcore.h"
#include "osdepend.h"
#include "emu.h"
#include "emuopts.h"

#include "unicode.h"
#include "ui/ui.h"
#include "osdcore.h"

using namespace std;
using namespace boost;
using namespace nsm;
using namespace google::protobuf::io;

Client *netClient=NULL;

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

Client *createGlobalClient(string _username)
{
  netClient = new Client(_username);
  netCommon = netClient;
  return netClient;
}

void deleteGlobalClient()
{
  if(netClient)
  {
    netClient->shutdown();
  }
  netClient = NULL;
}

#define INITIAL_BUFFER_SIZE (1024*1024*32)
unsigned char *compressedBuffer = (unsigned char*)malloc(INITIAL_BUFFER_SIZE);
int compressedBufferSize = INITIAL_BUFFER_SIZE;

bool hasCompleteResync = false;


Client::Client(string _username)
  :
  Common(_username,50)
{
  initialSyncBuffer.reserve(1024*1024);

  rakInterface = RakNet::RakPeerInterface::GetInstance();
  rakInterface->AllowConnectionResponseIPMigration(false);

#if 0
#endif

  initComplete=false;
  firstResync=true;

  syncPtr = compressedBuffer;

  selfPeerID = 0;
}

void Client::shutdown()
{
  // Be nice and let the server know we quit.
  rakInterface->Shutdown(300);

  // We're done with the network
  RakNet::RakPeerInterface::DestroyInstance(rakInterface);
}

vector<boost::shared_ptr<MemoryBlock> > Client::createMemoryBlock(const std::string& name, unsigned char *ptr,int size)
{
  if(initComplete)
  {
    cout << "ERROR: CREATED MEMORY BLOCK TOO LATE\n";
    exit(1);
  }
  vector<shared_ptr<MemoryBlock> > retval;
  const int BYTES_IN_MB=1024*1024;
  if(size>BYTES_IN_MB)
  {
    for(int a=0;; a+=BYTES_IN_MB)
    {
      if(a+BYTES_IN_MB>=size)
      {
        vector<shared_ptr<MemoryBlock> > tmp = createMemoryBlock(name,ptr+a,size-a);
        retval.insert(retval.end(),tmp.begin(),tmp.end());
        break;
      }
      else
      {
        vector<shared_ptr<MemoryBlock> > tmp = createMemoryBlock(name,ptr+a,BYTES_IN_MB);
        retval.insert(retval.end(),tmp.begin(),tmp.end());
      }
    }
    return retval;
  }
  //printf("Creating memory block at %X with size %d\n",ptr,size);
  blocks.push_back(shared_ptr<MemoryBlock>(new MemoryBlock(name,ptr,size)));
  staleBlocks.push_back(shared_ptr<MemoryBlock>(new MemoryBlock(name,size)));
  syncCheckBlocks.push_back(shared_ptr<MemoryBlock>(new MemoryBlock(name,size)));
  retval.push_back(blocks.back());
  return retval;
}

// Copied from Multiplayer.cpp
// If the first byte is ID_TIMESTAMP or ID_MAMEHUB_TIMESTAMP, then we want the 5th byte
// Otherwise we want the 1st byte
unsigned char GetPacketIdentifier(RakNet::Packet *p)
{
  if (p==0)
    return 255;

  if ((unsigned char)p->data[0] == ID_TIMESTAMP || (unsigned char)p->data[0] == ID_MAMEHUB_TIMESTAMP)
  {
    assert(p->length > sizeof(unsigned char) + sizeof(RakNet::Time));
    return (unsigned char) p->data[sizeof(unsigned char) + sizeof(RakNet::Time)];
  }
  else {
    return (unsigned char) p->data[0];
  }
}

unsigned char *GetPacketData(RakNet::Packet *p)
{
  if (p==0)
    return 0;

  if ((unsigned char)p->data[0] == ID_TIMESTAMP || (unsigned char)p->data[0] == ID_MAMEHUB_TIMESTAMP)
  {
    assert(p->length > (2*sizeof(unsigned char)) + sizeof(RakNet::Time));
    return (unsigned char*) &(p->data[(2*sizeof(unsigned char)) + sizeof(RakNet::Time)]);
  }
  else
  {
    return (unsigned char*) &(p->data[sizeof(unsigned char)]);
  }
}
int GetPacketSize(RakNet::Packet *p)
{
  if (p==0)
    return 0;

  if ((unsigned char)p->data[0] == ID_TIMESTAMP || (unsigned char)p->data[0] == ID_MAMEHUB_TIMESTAMP)
  {
    assert(p->length > (2*sizeof(unsigned char)) + sizeof(RakNet::Time));
    return int(p->length) - int((2*sizeof(unsigned char)) + sizeof(RakNet::Time));
  }
  else
  {
    return int(p->length) - int(sizeof(unsigned char));
  }
}

int initialSyncPercentComplete=0;
extern bool waitingForClientCatchup;
extern int baseDelayFromPing;
extern attotime mostRecentSentReport;
int doCatchup=0;
RakNet::RakNetGUID masterGuid;
RakNet::Time largestPacketTime=0;

bool Client::initializeConnection(unsigned short selfPort,const char *hostname,unsigned short port,running_machine *machine)
{

  /*
    boost::shared_ptr<TSocket> socket(new TSocket("localhost", port + 1));
    boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TJSONProtocol(transport));

    MameHubRpcClient client(protocol);
    transport->open();
    Status status;
    client.getStatus(status);
    cout << status.playerStatus[0].name << " <<<<< NAME" << endl;
    transport->close();
    exit(1);
  */


  RakNet::SocketDescriptor socketDescriptor(0,0);
  socketDescriptor.port = selfPort;
  printf("Client running on port %d\n",selfPort);
  RakNet::StartupResult retval = rakInterface->Startup(8,&socketDescriptor,1);
  rakInterface->SetMaximumIncomingConnections(512);
  rakInterface->SetIncomingPassword("MAME",(int)strlen("MAME"));
  rakInterface->SetTimeoutTime(30000,RakNet::UNASSIGNED_SYSTEM_ADDRESS);
  rakInterface->SetOccasionalPing(true);
  rakInterface->SetUnreliableTimeout(1000);

  if(retval != RakNet::RAKNET_STARTED)
  {
    printf("Client failed to start. Terminating\n");
    return false;
  }

  RakNet::SystemAddress sa = ConnectBlocking(hostname,port,true);
  if(sa==RakNet::UNASSIGNED_SYSTEM_ADDRESS)
  {
    printf("Could not connect to server!\n");
    return false;
  }
  RakNet::RakNetGUID guid = rakInterface->GetGuidFromSystemAddress(sa);
  masterGuid = guid;

  {
    char buf[4096];
    buf[0] = ID_CLIENT_INFO;
    strcpy(buf+1,username.c_str());
    rakInterface->Send(buf,1+username.length()+1,IMMEDIATE_PRIORITY,RELIABLE_ORDERED,0,sa,false);
  }

  peerIDs[guid] = 1;

  while(initComplete==false)
  {
    RakNet::Packet *p = rakInterface->Receive();
    if(!p)
    {
      //printf("WAITING FOR SERVER TO SEND GAME WORLD...\n");
      machine->video().frame_update();
      RakSleep(10);
      continue; //We need the first few packets, so stall until we get them
    }
    unsigned char packetID = GetPacketIdentifier(p);

    //printf("GOT PACKET: %d\n",int(packetID));

    switch(packetID)
    {
    case ID_DISCONNECTION_NOTIFICATION:
    {
      // Connection lost normally
      printf("ID_DISCONNECTION_NOTIFICATION\n");
      if(selfPeerID==0)
      {
        printf("Disconnected because a connection could not be made between you and another peer.\n");
      }
      return false;
    }
    case ID_ALREADY_CONNECTED:
      printf("ID_ALREADY_CONNECTED\n");
      return false;
    case ID_INCOMPATIBLE_PROTOCOL_VERSION:
      printf("ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
      return false;
    case ID_REMOTE_DISCONNECTION_NOTIFICATION: // Server telling the clients of another client disconnecting gracefully.  You can manually broadcast this in a peer to peer enviroment if you want.
      printf("ID_REMOTE_DISCONNECTION_NOTIFICATION\n");
      return false;
    case ID_REMOTE_CONNECTION_LOST: // Server telling the clients of another client disconnecting forcefully.  You can manually broadcast this in a peer to peer enviroment if you want.
      printf("ID_REMOTE_CONNECTION_LOST\n");
      return false;
    case ID_REMOTE_NEW_INCOMING_CONNECTION: // Server telling the clients of another client connecting.  You can manually broadcast this in a peer to peer enviroment if you want.
      printf("ID_REMOTE_NEW_INCOMING_CONNECTION\n");
      break;
    case ID_CONNECTION_BANNED: // Banned from this server
      printf("We are banned from this server.\n");
      return false;
    case ID_CONNECTION_ATTEMPT_FAILED:
      printf("Connection attempt failed\n");
      return false;
    case ID_NO_FREE_INCOMING_CONNECTIONS:
      // Sorry, the server is full.  I don't do anything here but
      // A real app should tell the user
      printf("ID_NO_FREE_INCOMING_CONNECTIONS\n");
      return false;

    case ID_INVALID_PASSWORD:
      printf("ID_INVALID_PASSWORD\n");
      return false;

    case ID_CONNECTION_LOST:
      // Couldn't deliver a reliable packet - i.e. the other system was abnormally
      // terminated
      printf("ID_CONNECTION_LOST\n");
      return false;

    case ID_CONNECTION_REQUEST_ACCEPTED:
      // This tells the client they have connected
      printf("ID_CONNECTION_REQUEST_ACCEPTED to %s with GUID %s\n", p->systemAddress.ToString(true), p->guid.ToString());
      printf("My external address is %s\n", rakInterface->GetExternalID(p->systemAddress).ToString(true));
      break;

    case ID_HOST_ACCEPTED:
    {
      unsigned char* dataPtr = p->data+1;
      int peerID;
      memcpy(&peerID,dataPtr,sizeof(int));
      dataPtr += sizeof(int);
      RakNet::RakNetGUID guid;
      memcpy(&(guid.g),dataPtr,sizeof(uint64_t));
      dataPtr += sizeof(uint64_t);
      int secs;
      long long attosecs;
      memcpy(&secs,dataPtr,sizeof(secs));
      dataPtr += sizeof(secs);
      memcpy(&attosecs,dataPtr,sizeof(attosecs));
      dataPtr += sizeof(attosecs);
      nsm::Attotime startTime = newAttotime(secs,attosecs);

      memcpy(&rollback,dataPtr,sizeof(bool));
      dataPtr += sizeof(bool);
      cout << "ROLLBACK " << (rollback?"ENABLED":"DISABLED") << endl;

      memcpy(&largestPacketTime,dataPtr,sizeof(RakNet::Time));
      dataPtr += sizeof(RakNet::Time);

      char buf[4096];
      strcpy(buf,(char*)(dataPtr));
      cout << "HOSTNAME " << sa.ToString() << " ACCEPTED (MY HOSTNAME IS " << rakInterface->GetExternalID(p->systemAddress).ToString() << ")\n";
      if(rakInterface->GetMyGUID()==guid)
      {
        //This is me, set my own ID and name
        selfPeerID = peerID;
        // Default player index is in order of join.
        player = selfPeerID - 1;
        mostRecentSentReport.seconds = startTime.seconds();
        mostRecentSentReport.attoseconds = startTime.attoseconds();
        cout << "CLIENT STARTED AT TIME: " << startTime.seconds() << "." << startTime.attoseconds() << endl;
      }
      else
      {
        //This is someone else, set their ID and name and wait for them to catch up
        waitingForClientCatchup=true;
        machine->osd().pauseAudio(true);
      }
      upsertPeer(guid,peerID,buf,startTime);
    }
    break;

    case ID_SEND_PEER_ID:
    {
      unsigned char *tmpbuf = p->data+1;
      int peerID;
      memcpy(&peerID,tmpbuf,sizeof(int));
      tmpbuf += sizeof(int);
      int secs;
      long long attosecs;
      memcpy(&secs,tmpbuf,sizeof(secs));
      tmpbuf += sizeof(secs);
      memcpy(&(attosecs),tmpbuf,sizeof(attosecs));
      tmpbuf += sizeof(attosecs);
      nsm::Attotime startTime = newAttotime(secs,attosecs);
      cout << "Matching: " << p->systemAddress.ToString() << " To " << peerID << endl;
      char buf[4096];
      strcpy(buf,(char*)(tmpbuf));
      cout << "Matching: " << p->systemAddress.ToString() << " To " << buf << endl;
      upsertPeer(p->guid,peerID,buf,startTime);
    }
    break;


    case ID_INITIAL_SYNC_PARTIAL:
    {
      //printf("GOT PARTIAL SYNC FROM SERVER\n");
      int curPos = (int)initialSyncBuffer.size();
      initialSyncBuffer.resize(initialSyncBuffer.size()+GetPacketSize(p));
      memcpy(&initialSyncBuffer[curPos],GetPacketData(p),GetPacketSize(p));

      int totalSize;
      memcpy(&totalSize,(&initialSyncBuffer[sizeof(int)]),sizeof(int));
      initialSyncPercentComplete = initialSyncBuffer.size()*1000/totalSize;
    }
    break;
    case ID_INITIAL_SYNC_COMPLETE:
    {
      printf("GOT INITIAL SYNC FROM SERVER!\n");
      int curPos = (int)initialSyncBuffer.size();
      initialSyncBuffer.resize(initialSyncBuffer.size()+GetPacketSize(p));
      memcpy(&initialSyncBuffer[curPos],GetPacketData(p),GetPacketSize(p));
      loadInitialData(&initialSyncBuffer[0],(int)initialSyncBuffer.size(),machine);
      initComplete=true;
    }
    break;
    case ID_INPUTS:
    {
      if(initComplete) throw emu_fatalerror("GOT INPUT BEFORE INIT COMPLETE");
      if (p->guid == masterGuid) {
        // Inputs from server.  Record time if it's newer
        RakNet::BitStream timeBS( (unsigned char*)&(p->data[1]), sizeof(RakNet::Time), false);
        timeBS.EndianSwapBytes(0,sizeof(RakNet::Time));
        RakNet::Time packetTime;
        timeBS.Read(packetTime);
        cout << "Got server time: " << packetTime << endl;
        if (packetTime>largestPacketTime) {
          largestPacketTime = packetTime;
        }
      }
      string s = doInflate(GetPacketData(p), GetPacketSize(p));
      PeerInputDataList inputDataList;
      inputDataList.ParseFromString(s);
      receiveInputs(&inputDataList);
      break;
    }
    case ID_BASE_DELAY:
    {
      cout << "Changing base delay from " << baseDelayFromPing;
      memcpy(&baseDelayFromPing,GetPacketData(p),sizeof(int));
      cout << " to " << baseDelayFromPing << endl;
    }
    break;
    case ID_SETTINGS:
    {
      if(p->data[1]) {
        doCatchup=1;
        cout << "DOING CATCHUP\n";
      } else {
        doCatchup=0;
        cout << "NOT DOING CATCHUP\n";
      }
      memcpy(&secondsBetweenSync,p->data+2,sizeof(int));
      memcpy(&unmeasuredNoise,p->data+2+sizeof(int),sizeof(int));
      char buf[4096];
      strcpy(buf,(const char*)(p->data+2+(2*sizeof(int))));
      string s(buf,strlen(buf));
      // Create peerdata for server
      upsertPeer(p->guid,1,buf,newAttotime(0,0));
    }
    break;
    default:
      //printf("GOT AN INVALID PACKET TYPE: %d\n",int(packetID));
      //throw std::runtime_error("OOPS");
      break;
    }

    rakInterface->DeallocatePacket(p);
  }

  return true;
}

nsm::InitialSync initial_sync;

void Client::loadInitialData(unsigned char *data,int size,running_machine *machine)
{

  {
    ArrayInputStream ais(data,size);
    LzmaInputStream lis(&ais);
    initial_sync.ParseFromZeroCopyStream(&lis);
  }

  waitingForClientCatchup=true;

  syncGeneration = initial_sync.generation();
  syncSeconds = initial_sync.global_time().seconds();
  syncAttoseconds = initial_sync.global_time().attoseconds();

  for(int a=0;a<initial_sync.peer_data_size();a++) {
    const PeerInputDataList &peer_data = initial_sync.peer_data(a);
    receiveInputs(&peer_data);
  }

  cout << "GOT NVRAM: " << initial_sync.nvram_size() << endl;

  if(initial_sync.nvram_size())
  {
    int nvram_index=0;

    nvram_interface_iterator iter(machine->root_device());
    for (device_nvram_interface *nvram = iter.first(); nvram != NULL; nvram = iter.next())
    {
      astring filename;
      emu_file file(machine->options().nvram_directory(), OPEN_FLAG_WRITE | OPEN_FLAG_CREATE | OPEN_FLAG_CREATE_PATHS);
      if (file.open(machine->nvram_filename(filename,nvram->device())) == FILERR_NONE)
      {
        cout << "SAVING NVRAM OF SIZE: " << initial_sync.nvram(nvram_index).length() << endl;
        file.write(initial_sync.nvram(nvram_index).c_str(),initial_sync.nvram(nvram_index).length());
        file.close();
      }
      else
      {
        cout << "ERROR STORING NVRAM\n";
        exit(1);
      }
      nvram_index++;
    }
  }
}

void Client::createInitialBlocks(running_machine *machine) {
  unsigned char checksum = 0;

  machine->save().dispatch_presave();

  if(getSecondsBetweenSync())
  {
    //Server and client must match on blocks
    if(blocks.size() != initial_sync.initial_block_size())
    {
      cout << "ERROR: CLIENT AND SERVER BLOCK COUNTS DO NOT MATCH!\n";
      exit(1);
    }

    for(int blockIndex=0; blockIndex<initial_sync.initial_block_size(); blockIndex++)
    {
      if(initial_sync.initial_block(blockIndex).length() != blocks[blockIndex]->size)
      {
        cout << "ERROR: CLIENT AND SERVER BLOCK SIZES AT INDEX " << blockIndex << " DO NOT MATCH!\n";
        cout << initial_sync.initial_block(blockIndex).length() << " != " << blocks[blockIndex]->size << endl;
        exit(1);
      }

      //initialSyncStream.ReadBits(staleBlocks[blockIndex]->data,blockSize*8);

      //cout << "BLOCK " << blockIndex << ":\n";
      for(int a=0; a<blocks[blockIndex]->size; a++)
      {
        unsigned char xorValue = initial_sync.initial_block(blockIndex)[a];
        //cout << int(blocks[blockIndex]->data[a] ^ xorValue) << '\n';
        staleBlocks[blockIndex]->data[a] = blocks[blockIndex]->data[a] ^ xorValue;
        checksum = checksum ^ staleBlocks[blockIndex]->data[a];
      }
      if(checksum != initial_sync.checksum(blockIndex)) {
        cout << "CHECKSUM ERROR: " << int(checksum) << " != " << int(initial_sync.checksum(blockIndex)) << " " << blocks[blockIndex]->name << " (index: " << blockIndex << ") " << endl;
        exit(1);
      }
    }
  }

  revert(machine);

  machine->save().dispatch_postload();
}

void Client::updateSyncCheck()
{
  printf("UPDATING SYNC CHECK\n");
  unsigned char checksum = 0;
  for(int blockIndex=0; blockIndex<int(blocks.size()); blockIndex++)
  {
    memcpy(
      syncCheckBlocks[blockIndex]->data,
      blocks[blockIndex]->data,
      blocks[blockIndex]->size
      );
    for(int b=0;b<syncCheckBlocks[blockIndex]->size;b++) {
      checksum = checksum ^ syncCheckBlocks[blockIndex]->data[b];
    }
  }
  cout << "SYNC CHECK CHECKSUM: " << int(checksum) << endl;
}


bool printWhenCheck=false;

bool Client::sync(running_machine *machine)
{
  if(!hasCompleteResync) return false;
  hasCompleteResync=false;

  bool hadToResync = resync(compressedBuffer,int(syncPtr-compressedBuffer),machine);

  //We have to return here because processing two syncs without a frame
  //in between can cause crashes
  syncPtr = compressedBuffer;
  if(firstResync || hadToResync)
  {
    printf("BEGINNING VIDEO SKIP\n");
    firstResync=false;
    return true;
  }
  else
  {
    return false;
  }
}

bool Client::update(running_machine *machine)
{
  RakSleep(0);
  if(printWhenCheck)
  {
    printWhenCheck=false;
    //printf("Checking for packets\n");
  }
  bool inNegotiation=false;


  do
  {
    RakNet::Packet *p = rakInterface->Receive();
    if(!p)
    {
      if (inNegotiation) {
        continue;
      } else {
        break;
     }
    }
    unsigned char packetID = GetPacketIdentifier(p);
    //cout << "GOT PACKET WITH ID: " << packetID << endl;

    switch(packetID)
    {
    case ID_CONNECTION_LOST:
      // Couldn't deliver a reliable packet - i.e. the other system was abnormally
      // terminated
      printf("ID_CONNECTION_LOST\n");
    case ID_DISCONNECTION_NOTIFICATION:
      // Connection lost normally
      printf("ID_DISCONNECTION_NOTIFICATION\n");
      if(peerIDs.find(p->guid)!=peerIDs.end())
      {
        if(peerIDs[p->guid]==1)
        {
          //Server quit, we are done.
          return false;
        }
        else
        {
          if(peerData.find(peerIDs[p->guid]) != peerData.end())
            peerData.erase(peerIDs[p->guid]);
          peerIDs.erase(p->guid);
        }
      }
      break;
    case ID_ALREADY_CONNECTED:
      printf("ID_ALREADY_CONNECTED\n");
      return false;
      break;
    case ID_INCOMPATIBLE_PROTOCOL_VERSION:
      printf("ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
      return false;
      break;
    case ID_REMOTE_DISCONNECTION_NOTIFICATION: // Server telling the clients of another client disconnecting gracefully.  You can manually broadcast this in a peer to peer enviroment if you want.
      printf("ID_REMOTE_DISCONNECTION_NOTIFICATION\n");
      break;
    case ID_REMOTE_CONNECTION_LOST: // Server telling the clients of another client disconnecting forcefully.  You can manually broadcast this in a peer to peer enviroment if you want.
      printf("ID_REMOTE_CONNECTION_LOST\n");
      break;
    case ID_REMOTE_NEW_INCOMING_CONNECTION: // Server telling the clients of another client connecting.  You can manually broadcast this in a peer to peer enviroment if you want.
      printf("ID_REMOTE_NEW_INCOMING_CONNECTION\n");
      break;
    case ID_CONNECTION_BANNED: // Banned from this server
      printf("We are banned from this server.\n");
      return false;
      break;
    case ID_CONNECTION_ATTEMPT_FAILED:
      printf("Connection attempt failed\n");
      return false;
      break;
    case ID_NO_FREE_INCOMING_CONNECTIONS:
      // Sorry, the server is full.  I don't do anything here but
      // A real app should tell the user
      printf("ID_NO_FREE_INCOMING_CONNECTIONS\n");
      return false;
      break;

    case ID_INVALID_PASSWORD:
      printf("ID_INVALID_PASSWORD\n");
      return false;
      break;

    case ID_CONNECTION_REQUEST_ACCEPTED:
      // This tells the client they have connected
      printf("ID_CONNECTION_REQUEST_ACCEPTED to %s with GUID %s\n", p->systemAddress.ToString(true), p->guid.ToString());
      printf("My external address is %s\n", rakInterface->GetExternalID(p->systemAddress).ToString(true));
      break;

    case ID_ADVERTISE_SYSTEM:
    {
      inNegotiation=true;
      RakNet::SystemAddress sa;
      sa.SetBinaryAddress(((char*)p->data)+1);
      RakNet::SystemAddress sa2 = ConnectBlocking(sa.ToString(false),sa.GetPort(),false);
      if(sa2 != RakNet::UNASSIGNED_SYSTEM_ADDRESS)
      {
        cout << "Sending ID\n";
        //Tell the new guy your ID and start time
        char buf[4096];
        buf[0] = ID_SEND_PEER_ID;
        char* tmpbuf = buf+1;
        memcpy(tmpbuf,&selfPeerID,sizeof(int));
        tmpbuf += sizeof(int);
        attotime startTime;
        memcpy(tmpbuf,&(startTime.seconds),sizeof(startTime.seconds));
        tmpbuf += sizeof(startTime.seconds);
        memcpy(tmpbuf,&(startTime.attoseconds),sizeof(startTime.attoseconds));
        tmpbuf += sizeof(startTime.attoseconds);
        strcpy(tmpbuf,username.c_str());
        tmpbuf += username.length()+1;
        rakInterface->Send(buf,int(tmpbuf-buf),IMMEDIATE_PRIORITY,RELIABLE_ORDERED,0,sa2,false);
      }
      if(sa2==RakNet::UNASSIGNED_SYSTEM_ADDRESS)
      {
        //Tell the boss that you can't accept this guy
        char buf[4096];
        buf[0] = ID_REJECT_NEW_HOST;
        strcpy(buf+1,((char*)p->data)+1);
        rakInterface->Send(buf,1+strlen(((char*)p->data)+1)+1,IMMEDIATE_PRIORITY,RELIABLE_ORDERED,0,rakInterface->GetSystemAddressFromIndex(0),false);
      }
      else
      {
        //Tell the boss that you can accept this guy
        char buf[4096];
        buf[0] = ID_ACCEPT_NEW_HOST;
        strcpy(buf+1,((char*)p->data)+1);
        rakInterface->Send(buf,1+strlen(((char*)p->data)+1)+1,IMMEDIATE_PRIORITY,RELIABLE_ORDERED,0,rakInterface->GetSystemAddressFromIndex(0),false);
      }

    }
    break;

    case ID_HOST_ACCEPTED:
    {
      inNegotiation=false;
      unsigned char *tmpbuf = p->data+1;
      int peerID;
      memcpy(&peerID,tmpbuf,sizeof(int));
      tmpbuf += sizeof(int);
      RakNet::RakNetGUID guid;
      memcpy(&(guid.g),tmpbuf,sizeof(uint64_t));
      tmpbuf += sizeof(uint64_t);
      int secs;
      long long attosecs;
      memcpy(&secs,tmpbuf,sizeof(secs));
      tmpbuf += sizeof(secs);
      memcpy(&attosecs,tmpbuf,sizeof(attosecs));
      tmpbuf += sizeof(attosecs);
      nsm::Attotime startTime = newAttotime(secs,attosecs);

      char buf[4096];
      strcpy(buf,(char*)(tmpbuf));
      cout << "HOST ACCEPTED\n";
      if(rakInterface->GetMyGUID()==guid)
      {
        //This is me, set my own ID and name
        selfPeerID = peerID;
        mostRecentSentReport.seconds = startTime.seconds();
        mostRecentSentReport.attoseconds = startTime.attoseconds();
        cout << "CLIENT STARTED AT TIME: " << startTime.seconds() << "." << startTime.attoseconds() << endl;
      }
      else
      {
        //This is someone else, set their ID and name
        waitingForClientCatchup=true;
        machine->osd().pauseAudio(true);
      }
      upsertPeer(guid,peerID,buf,startTime);
    }
    break;

    case ID_RESYNC_PARTIAL:
    {
      printf("GOT PARTIAL RESYNC\n");
      if(hasCompleteResync)
      {
        //hasCompleteResync=false;
        //syncPtr = compressedBuffer;
        printf("ERROR: GOT NEW RESYNC WHILE ANOTHER RESYNC WAS ON THE QUEUE!\n");
        return false;
      }
      int bytesUsed = syncPtr-compressedBuffer;
      while(bytesUsed+GetPacketSize(p) >= compressedBufferSize)
      {
        compressedBufferSize *= 1.5;
	free(compressedBuffer);
        compressedBuffer = (unsigned char*)malloc(compressedBufferSize);
        if(!compressedBuffer)
        {
          cout << __FILE__ << ":" << __LINE__ << " OUT OF MEMORY\n";
          exit(1);
        }
        syncPtr = compressedBuffer+bytesUsed;
      }
      memcpy(syncPtr,GetPacketData(p),GetPacketSize(p));
      syncPtr += GetPacketSize(p);
      printWhenCheck=true;
      break;
    }
    case ID_RESYNC_COMPLETE:
    {
      cout << "GOT COMPLETE RESYNC WITH SIZE " << GetPacketSize(p) << " (Without header)\n";
      int bytesUsed = syncPtr-compressedBuffer;
      while(bytesUsed+GetPacketSize(p) >= compressedBufferSize)
      {
        compressedBufferSize *= 1.5;
	free(compressedBuffer);
        compressedBuffer = (unsigned char*)malloc(compressedBufferSize);
        if(!compressedBuffer)
        {
          cout << __FILE__ << ":" << __LINE__ << " OUT OF MEMORY\n";
          exit(1);
        }
        syncPtr = compressedBuffer+bytesUsed;
      }
      memcpy(syncPtr,GetPacketData(p),GetPacketSize(p));
      syncPtr += GetPacketSize(p);
      hasCompleteResync=true;
      return true;
      break;
    }
    case ID_INPUTS:
    {
      if(peerIDs.find(p->guid)==peerIDs.end())
      {
        throw std::runtime_error("GOT INPUTS FROM UNKNOWN PEER");
      }
      if (p->data[0] != ID_MAMEHUB_TIMESTAMP) {
        throw std::runtime_error("OLD VERSION OF MAMEHUB TRYING TO TALK TO NEW VERSION");
      }
      if (p->guid == masterGuid) {
        // Inputs from server.  Record time if it's newer
        RakNet::BitStream timeBS( (unsigned char*)&(p->data[1]), sizeof(RakNet::Time), false);
        RakNet::Time packetTime;
        timeBS.EndianSwapBytes(0,sizeof(RakNet::Time));
        timeBS.Read(packetTime);
        if (packetTime>largestPacketTime) {
          //cout << "GOT NEW PACKET TIME: " << packetTime << endl;
          largestPacketTime = packetTime;
        }
      }
      string s = doInflate(GetPacketData(p), GetPacketSize(p));
      PeerInputDataList inputDataList;
      inputDataList.ParseFromString(s);
      receiveInputs(&inputDataList);
      break;
    }
    case ID_BASE_DELAY:
    {
      cout << "Changing base delay from " << baseDelayFromPing;
      memcpy(&baseDelayFromPing,GetPacketData(p),sizeof(int));
      cout << " to " << baseDelayFromPing << endl;
    }
    break;
    case ID_SETTINGS:
      memcpy(&secondsBetweenSync,p->data+1,sizeof(int));
      break;
    default:
      printf("GOT AN INVALID PACKET TYPE: %d %d %d\n",int(packetID), (int(packetID) - int(ID_USER_PACKET_ENUM)), GetPacketSize(p));
      return false;
    }

    rakInterface->DeallocatePacket(p);
  }
  while(true);

  return true;
}

bool Client::resync(unsigned char *data,int size,running_machine *machine)
{
  int compressedSize;
  memcpy(&compressedSize,data,sizeof(int));
  data += sizeof(int);
  cout << "COMPRESSED SIZE: " << compressedSize << endl;

  nsm::Sync syncProto;

  {
    ArrayInputStream ais(data,compressedSize);
    LzmaInputStream lis(&ais);
    syncProto.ParseFromZeroCopyStream(&lis);
  }

  syncGeneration = syncProto.generation();
  syncSeconds = syncProto.global_time().seconds();
  syncAttoseconds = syncProto.global_time().attoseconds();

  //Check to see if the client is clean
  bool clientIsClean=true;

  int badByteCount=0;
  int totalByteCount=0;

  for (int i=0;i<syncProto.block_size();i++)
  {
    const nsm::SyncBlock& syncBlock = syncProto.block(i);
    int blockIndex = syncBlock.index();
    unsigned char *syncBlockData = (unsigned char*)(&syncBlock.data()[0]);

    if(blockIndex >= int(blocks.size()) || blockIndex<0)
    {
      cout << "GOT AN INVALID BLOCK INDEX: " << blockIndex << endl;
      break;
    }

    MemoryBlock &block = *(blocks[blockIndex]);
    MemoryBlock &syncCheckBlock = *(syncCheckBlocks[blockIndex]);
    MemoryBlock &staleBlock = *(staleBlocks[blockIndex]);

    //cout << "CLIENT: GOT MESSAGE FOR INDEX: " << blockIndex << endl;

    //if(clientSizeOfNextMessage!=block.size)
    //{
    //cout << "ERROR!: SIZE MISMATCH " << clientSizeOfNextMessage
    //<< " != " << block.size << endl;
    //}

    //cout << "BYTES READ: " << (xorBlock.size-strm.avail_out) << endl;
    for(int a=0; a<block.size; a++)
    {
      totalByteCount++;
      if(syncCheckBlock.data[a] != (syncBlockData[a] ^ staleBlock.data[a]))
      {
        badByteCount++;
        if(badByteCount<50)
        {
          printf("BLOCK %d BYTE %d IS BAD: %d %d %d\n",blockIndex,a,
                 int(syncBlockData[a]), int(syncCheckBlock.data[a]),
                 int(staleBlock.data[a]));
        }
        if(badByteCount>64)
          clientIsClean=false;
        syncCheckBlock.data[a] = (syncBlockData[a] ^ staleBlock.data[a]);
      }
      staleBlock.data[a] = (syncBlockData[a] ^ staleBlock.data[a]);
    }
  }

  if(badByteCount*100/totalByteCount>=1) clientIsClean=false; //1% or more of the bytes are bad

  if(clientIsClean)
  {
    printf("CLIENT IS CLEAN\n");
    for(int a=0; a<int(blocks.size()); a++)
    {
      //memcpy(staleBlocks[a].data,syncCheckBlocks[a].data,syncCheckBlocks[a].size);
    }
    return false;
  }

  if (machine->scheduler().can_save()==false)
  {
    printf("CLIENT IS DIRTY BUT HAD ANONYMOUS TIMER SO CAN'T FIX!\n");
    return false;
  }

  printf("CLIENT IS DIRTY (%d bad blocks, %f%% of total)\n",badByteCount,float(badByteCount)*100.0f/totalByteCount);
  machine->ui().popup_time(3, "You are out of sync with the server, resyncing...");

  machine->save().dispatch_presave();

  revert(machine);

  machine->save().dispatch_postload();
  cout << "POST LOAD FINISHED\n";

  {
    unsigned char checksum = 0;
    //If the client has predicted anything, erase the prediction
    for(int a=0;a<blocks.size();a++)
    {
      //cout << "BLOCK " << a << ": ";
      for(int b=0;b<blocks[a]->size;b++) {
        checksum = checksum ^ blocks[a]->data[b];
      }
      //cout << int(checksum) << endl;
    }
    cout << "(revert) BLOCK CHECKSUM POST LOAD: " << int(checksum) << endl;
  }

  return true;
}

void Client::revert(running_machine *machine)
{
  generation = syncGeneration;
  machine->machine_time().seconds = syncSeconds;
  machine->machine_time().attoseconds = syncAttoseconds;

  {
    unsigned char checksum = 0;
    //If the client has predicted anything, erase the prediction
    for(int a=0;a<blocks.size();a++)
    {
      //cout << "BLOCK " << a << ": ";
      memcpy(blocks[a]->data,staleBlocks[a]->data,blocks[a]->size);
      for(int b=0;b<staleBlocks[a]->size;b++) {
        checksum = checksum ^ staleBlocks[a]->data[b];
      }
      //cout << int(checksum) << endl;
    }
    cout << "(revert) STALE CHECKSUM: " << int(checksum) << endl;
  }

}

int Client::getNumSessions()
{
  return rakInterface->NumberOfConnections();
}

unsigned long long Client::getCurrentServerTime() {
  //cout << "LAST PING: " << largestPacketTime << " " << (rakInterface->GetLastPing(masterGuid)/2) << endl;
  return largestPacketTime + (rakInterface->GetLastPing(masterGuid)/2);
}
