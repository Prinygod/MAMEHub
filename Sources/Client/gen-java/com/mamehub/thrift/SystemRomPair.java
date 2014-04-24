/**
 * Autogenerated by Thrift Compiler (1.0.0-dev)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
package com.mamehub.thrift;

import org.apache.thrift.scheme.IScheme;
import org.apache.thrift.scheme.SchemeFactory;
import org.apache.thrift.scheme.StandardScheme;

import org.apache.thrift.scheme.TupleScheme;
import org.apache.thrift.protocol.TTupleProtocol;
import org.apache.thrift.protocol.TProtocolException;
import org.apache.thrift.EncodingUtils;
import org.apache.thrift.TException;
import org.apache.thrift.async.AsyncMethodCallback;
import org.apache.thrift.server.AbstractNonblockingServer.*;
import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;
import java.util.EnumMap;
import java.util.Set;
import java.util.HashSet;
import java.util.EnumSet;
import java.util.Collections;
import java.util.BitSet;
import java.nio.ByteBuffer;
import java.util.Arrays;
import javax.annotation.Generated;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

@SuppressWarnings({"cast", "rawtypes", "serial", "unchecked"})
@Generated(value = "Autogenerated by Thrift Compiler (1.0.0-dev)", date = "2014-4-24")
public class SystemRomPair implements org.apache.thrift.TBase<SystemRomPair, SystemRomPair._Fields>, java.io.Serializable, Cloneable, Comparable<SystemRomPair> {
  private static final org.apache.thrift.protocol.TStruct STRUCT_DESC = new org.apache.thrift.protocol.TStruct("SystemRomPair");

  private static final org.apache.thrift.protocol.TField SYSTEM_FIELD_DESC = new org.apache.thrift.protocol.TField("system", org.apache.thrift.protocol.TType.STRING, (short)1);
  private static final org.apache.thrift.protocol.TField ROM_FIELD_DESC = new org.apache.thrift.protocol.TField("rom", org.apache.thrift.protocol.TType.STRING, (short)2);

  private static final Map<Class<? extends IScheme>, SchemeFactory> schemes = new HashMap<Class<? extends IScheme>, SchemeFactory>();
  static {
    schemes.put(StandardScheme.class, new SystemRomPairStandardSchemeFactory());
    schemes.put(TupleScheme.class, new SystemRomPairTupleSchemeFactory());
  }

  public String system; // required
  public String rom; // required

  /** The set of fields this struct contains, along with convenience methods for finding and manipulating them. */
  public enum _Fields implements org.apache.thrift.TFieldIdEnum {
    SYSTEM((short)1, "system"),
    ROM((short)2, "rom");

    private static final Map<String, _Fields> byName = new HashMap<String, _Fields>();

    static {
      for (_Fields field : EnumSet.allOf(_Fields.class)) {
        byName.put(field.getFieldName(), field);
      }
    }

    /**
     * Find the _Fields constant that matches fieldId, or null if its not found.
     */
    public static _Fields findByThriftId(int fieldId) {
      switch(fieldId) {
        case 1: // SYSTEM
          return SYSTEM;
        case 2: // ROM
          return ROM;
        default:
          return null;
      }
    }

    /**
     * Find the _Fields constant that matches fieldId, throwing an exception
     * if it is not found.
     */
    public static _Fields findByThriftIdOrThrow(int fieldId) {
      _Fields fields = findByThriftId(fieldId);
      if (fields == null) throw new IllegalArgumentException("Field " + fieldId + " doesn't exist!");
      return fields;
    }

    /**
     * Find the _Fields constant that matches name, or null if its not found.
     */
    public static _Fields findByName(String name) {
      return byName.get(name);
    }

    private final short _thriftId;
    private final String _fieldName;

    _Fields(short thriftId, String fieldName) {
      _thriftId = thriftId;
      _fieldName = fieldName;
    }

    public short getThriftFieldId() {
      return _thriftId;
    }

    public String getFieldName() {
      return _fieldName;
    }
  }

  // isset id assignments
  public static final Map<_Fields, org.apache.thrift.meta_data.FieldMetaData> metaDataMap;
  static {
    Map<_Fields, org.apache.thrift.meta_data.FieldMetaData> tmpMap = new EnumMap<_Fields, org.apache.thrift.meta_data.FieldMetaData>(_Fields.class);
    tmpMap.put(_Fields.SYSTEM, new org.apache.thrift.meta_data.FieldMetaData("system", org.apache.thrift.TFieldRequirementType.DEFAULT, 
        new org.apache.thrift.meta_data.FieldValueMetaData(org.apache.thrift.protocol.TType.STRING)));
    tmpMap.put(_Fields.ROM, new org.apache.thrift.meta_data.FieldMetaData("rom", org.apache.thrift.TFieldRequirementType.DEFAULT, 
        new org.apache.thrift.meta_data.FieldValueMetaData(org.apache.thrift.protocol.TType.STRING)));
    metaDataMap = Collections.unmodifiableMap(tmpMap);
    org.apache.thrift.meta_data.FieldMetaData.addStructMetaDataMap(SystemRomPair.class, metaDataMap);
  }

  public SystemRomPair() {
  }

  public SystemRomPair(
    String system,
    String rom)
  {
    this();
    this.system = system;
    this.rom = rom;
  }

  /**
   * Performs a deep copy on <i>other</i>.
   */
  public SystemRomPair(SystemRomPair other) {
    if (other.isSetSystem()) {
      this.system = other.system;
    }
    if (other.isSetRom()) {
      this.rom = other.rom;
    }
  }

  public SystemRomPair deepCopy() {
    return new SystemRomPair(this);
  }

  @Override
  public void clear() {
    this.system = null;
    this.rom = null;
  }

  public String getSystem() {
    return this.system;
  }

  public SystemRomPair setSystem(String system) {
    this.system = system;
    return this;
  }

  public void unsetSystem() {
    this.system = null;
  }

  /** Returns true if field system is set (has been assigned a value) and false otherwise */
  public boolean isSetSystem() {
    return this.system != null;
  }

  public void setSystemIsSet(boolean value) {
    if (!value) {
      this.system = null;
    }
  }

  public String getRom() {
    return this.rom;
  }

  public SystemRomPair setRom(String rom) {
    this.rom = rom;
    return this;
  }

  public void unsetRom() {
    this.rom = null;
  }

  /** Returns true if field rom is set (has been assigned a value) and false otherwise */
  public boolean isSetRom() {
    return this.rom != null;
  }

  public void setRomIsSet(boolean value) {
    if (!value) {
      this.rom = null;
    }
  }

  public void setFieldValue(_Fields field, Object value) {
    switch (field) {
    case SYSTEM:
      if (value == null) {
        unsetSystem();
      } else {
        setSystem((String)value);
      }
      break;

    case ROM:
      if (value == null) {
        unsetRom();
      } else {
        setRom((String)value);
      }
      break;

    }
  }

  public Object getFieldValue(_Fields field) {
    switch (field) {
    case SYSTEM:
      return getSystem();

    case ROM:
      return getRom();

    }
    throw new IllegalStateException();
  }

  /** Returns true if field corresponding to fieldID is set (has been assigned a value) and false otherwise */
  public boolean isSet(_Fields field) {
    if (field == null) {
      throw new IllegalArgumentException();
    }

    switch (field) {
    case SYSTEM:
      return isSetSystem();
    case ROM:
      return isSetRom();
    }
    throw new IllegalStateException();
  }

  @Override
  public boolean equals(Object that) {
    if (that == null)
      return false;
    if (that instanceof SystemRomPair)
      return this.equals((SystemRomPair)that);
    return false;
  }

  public boolean equals(SystemRomPair that) {
    if (that == null)
      return false;

    boolean this_present_system = true && this.isSetSystem();
    boolean that_present_system = true && that.isSetSystem();
    if (this_present_system || that_present_system) {
      if (!(this_present_system && that_present_system))
        return false;
      if (!this.system.equals(that.system))
        return false;
    }

    boolean this_present_rom = true && this.isSetRom();
    boolean that_present_rom = true && that.isSetRom();
    if (this_present_rom || that_present_rom) {
      if (!(this_present_rom && that_present_rom))
        return false;
      if (!this.rom.equals(that.rom))
        return false;
    }

    return true;
  }

  @Override
  public int hashCode() {
    return 0;
  }

  @Override
  public int compareTo(SystemRomPair other) {
    if (!getClass().equals(other.getClass())) {
      return getClass().getName().compareTo(other.getClass().getName());
    }

    int lastComparison = 0;

    lastComparison = Boolean.valueOf(isSetSystem()).compareTo(other.isSetSystem());
    if (lastComparison != 0) {
      return lastComparison;
    }
    if (isSetSystem()) {
      lastComparison = org.apache.thrift.TBaseHelper.compareTo(this.system, other.system);
      if (lastComparison != 0) {
        return lastComparison;
      }
    }
    lastComparison = Boolean.valueOf(isSetRom()).compareTo(other.isSetRom());
    if (lastComparison != 0) {
      return lastComparison;
    }
    if (isSetRom()) {
      lastComparison = org.apache.thrift.TBaseHelper.compareTo(this.rom, other.rom);
      if (lastComparison != 0) {
        return lastComparison;
      }
    }
    return 0;
  }

  public _Fields fieldForId(int fieldId) {
    return _Fields.findByThriftId(fieldId);
  }

  public void read(org.apache.thrift.protocol.TProtocol iprot) throws org.apache.thrift.TException {
    schemes.get(iprot.getScheme()).getScheme().read(iprot, this);
  }

  public void write(org.apache.thrift.protocol.TProtocol oprot) throws org.apache.thrift.TException {
    schemes.get(oprot.getScheme()).getScheme().write(oprot, this);
  }

  @Override
  public String toString() {
    StringBuilder sb = new StringBuilder("SystemRomPair(");
    boolean first = true;

    sb.append("system:");
    if (this.system == null) {
      sb.append("null");
    } else {
      sb.append(this.system);
    }
    first = false;
    if (!first) sb.append(", ");
    sb.append("rom:");
    if (this.rom == null) {
      sb.append("null");
    } else {
      sb.append(this.rom);
    }
    first = false;
    sb.append(")");
    return sb.toString();
  }

  public void validate() throws org.apache.thrift.TException {
    // check for required fields
    // check for sub-struct validity
  }

  private void writeObject(java.io.ObjectOutputStream out) throws java.io.IOException {
    try {
      write(new org.apache.thrift.protocol.TCompactProtocol(new org.apache.thrift.transport.TIOStreamTransport(out)));
    } catch (org.apache.thrift.TException te) {
      throw new java.io.IOException(te);
    }
  }

  private void readObject(java.io.ObjectInputStream in) throws java.io.IOException, ClassNotFoundException {
    try {
      read(new org.apache.thrift.protocol.TCompactProtocol(new org.apache.thrift.transport.TIOStreamTransport(in)));
    } catch (org.apache.thrift.TException te) {
      throw new java.io.IOException(te);
    }
  }

  private static class SystemRomPairStandardSchemeFactory implements SchemeFactory {
    public SystemRomPairStandardScheme getScheme() {
      return new SystemRomPairStandardScheme();
    }
  }

  private static class SystemRomPairStandardScheme extends StandardScheme<SystemRomPair> {

    public void read(org.apache.thrift.protocol.TProtocol iprot, SystemRomPair struct) throws org.apache.thrift.TException {
      org.apache.thrift.protocol.TField schemeField;
      iprot.readStructBegin();
      while (true)
      {
        schemeField = iprot.readFieldBegin();
        if (schemeField.type == org.apache.thrift.protocol.TType.STOP) { 
          break;
        }
        switch (schemeField.id) {
          case 1: // SYSTEM
            if (schemeField.type == org.apache.thrift.protocol.TType.STRING) {
              struct.system = iprot.readString();
              struct.setSystemIsSet(true);
            } else { 
              org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
            }
            break;
          case 2: // ROM
            if (schemeField.type == org.apache.thrift.protocol.TType.STRING) {
              struct.rom = iprot.readString();
              struct.setRomIsSet(true);
            } else { 
              org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
            }
            break;
          default:
            org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
        }
        iprot.readFieldEnd();
      }
      iprot.readStructEnd();

      // check for required fields of primitive type, which can't be checked in the validate method
      struct.validate();
    }

    public void write(org.apache.thrift.protocol.TProtocol oprot, SystemRomPair struct) throws org.apache.thrift.TException {
      struct.validate();

      oprot.writeStructBegin(STRUCT_DESC);
      if (struct.system != null) {
        oprot.writeFieldBegin(SYSTEM_FIELD_DESC);
        oprot.writeString(struct.system);
        oprot.writeFieldEnd();
      }
      if (struct.rom != null) {
        oprot.writeFieldBegin(ROM_FIELD_DESC);
        oprot.writeString(struct.rom);
        oprot.writeFieldEnd();
      }
      oprot.writeFieldStop();
      oprot.writeStructEnd();
    }

  }

  private static class SystemRomPairTupleSchemeFactory implements SchemeFactory {
    public SystemRomPairTupleScheme getScheme() {
      return new SystemRomPairTupleScheme();
    }
  }

  private static class SystemRomPairTupleScheme extends TupleScheme<SystemRomPair> {

    @Override
    public void write(org.apache.thrift.protocol.TProtocol prot, SystemRomPair struct) throws org.apache.thrift.TException {
      TTupleProtocol oprot = (TTupleProtocol) prot;
      BitSet optionals = new BitSet();
      if (struct.isSetSystem()) {
        optionals.set(0);
      }
      if (struct.isSetRom()) {
        optionals.set(1);
      }
      oprot.writeBitSet(optionals, 2);
      if (struct.isSetSystem()) {
        oprot.writeString(struct.system);
      }
      if (struct.isSetRom()) {
        oprot.writeString(struct.rom);
      }
    }

    @Override
    public void read(org.apache.thrift.protocol.TProtocol prot, SystemRomPair struct) throws org.apache.thrift.TException {
      TTupleProtocol iprot = (TTupleProtocol) prot;
      BitSet incoming = iprot.readBitSet(2);
      if (incoming.get(0)) {
        struct.system = iprot.readString();
        struct.setSystemIsSet(true);
      }
      if (incoming.get(1)) {
        struct.rom = iprot.readString();
        struct.setRomIsSet(true);
      }
    }
  }

}

