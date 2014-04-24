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
public class MediaType implements org.apache.thrift.TBase<MediaType, MediaType._Fields>, java.io.Serializable, Cloneable, Comparable<MediaType> {
  private static final org.apache.thrift.protocol.TStruct STRUCT_DESC = new org.apache.thrift.protocol.TStruct("MediaType");

  private static final org.apache.thrift.protocol.TField MEDIA_NAME_FIELD_DESC = new org.apache.thrift.protocol.TField("mediaName", org.apache.thrift.protocol.TType.STRING, (short)1);
  private static final org.apache.thrift.protocol.TField EXTENSIONS_FIELD_DESC = new org.apache.thrift.protocol.TField("extensions", org.apache.thrift.protocol.TType.LIST, (short)2);

  private static final Map<Class<? extends IScheme>, SchemeFactory> schemes = new HashMap<Class<? extends IScheme>, SchemeFactory>();
  static {
    schemes.put(StandardScheme.class, new MediaTypeStandardSchemeFactory());
    schemes.put(TupleScheme.class, new MediaTypeTupleSchemeFactory());
  }

  public String mediaName; // required
  public List<String> extensions; // required

  /** The set of fields this struct contains, along with convenience methods for finding and manipulating them. */
  public enum _Fields implements org.apache.thrift.TFieldIdEnum {
    MEDIA_NAME((short)1, "mediaName"),
    EXTENSIONS((short)2, "extensions");

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
        case 1: // MEDIA_NAME
          return MEDIA_NAME;
        case 2: // EXTENSIONS
          return EXTENSIONS;
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
    tmpMap.put(_Fields.MEDIA_NAME, new org.apache.thrift.meta_data.FieldMetaData("mediaName", org.apache.thrift.TFieldRequirementType.DEFAULT, 
        new org.apache.thrift.meta_data.FieldValueMetaData(org.apache.thrift.protocol.TType.STRING)));
    tmpMap.put(_Fields.EXTENSIONS, new org.apache.thrift.meta_data.FieldMetaData("extensions", org.apache.thrift.TFieldRequirementType.DEFAULT, 
        new org.apache.thrift.meta_data.ListMetaData(org.apache.thrift.protocol.TType.LIST, 
            new org.apache.thrift.meta_data.FieldValueMetaData(org.apache.thrift.protocol.TType.STRING))));
    metaDataMap = Collections.unmodifiableMap(tmpMap);
    org.apache.thrift.meta_data.FieldMetaData.addStructMetaDataMap(MediaType.class, metaDataMap);
  }

  public MediaType() {
  }

  public MediaType(
    String mediaName,
    List<String> extensions)
  {
    this();
    this.mediaName = mediaName;
    this.extensions = extensions;
  }

  /**
   * Performs a deep copy on <i>other</i>.
   */
  public MediaType(MediaType other) {
    if (other.isSetMediaName()) {
      this.mediaName = other.mediaName;
    }
    if (other.isSetExtensions()) {
      List<String> __this__extensions = new ArrayList<String>(other.extensions);
      this.extensions = __this__extensions;
    }
  }

  public MediaType deepCopy() {
    return new MediaType(this);
  }

  @Override
  public void clear() {
    this.mediaName = null;
    this.extensions = null;
  }

  public String getMediaName() {
    return this.mediaName;
  }

  public MediaType setMediaName(String mediaName) {
    this.mediaName = mediaName;
    return this;
  }

  public void unsetMediaName() {
    this.mediaName = null;
  }

  /** Returns true if field mediaName is set (has been assigned a value) and false otherwise */
  public boolean isSetMediaName() {
    return this.mediaName != null;
  }

  public void setMediaNameIsSet(boolean value) {
    if (!value) {
      this.mediaName = null;
    }
  }

  public int getExtensionsSize() {
    return (this.extensions == null) ? 0 : this.extensions.size();
  }

  public java.util.Iterator<String> getExtensionsIterator() {
    return (this.extensions == null) ? null : this.extensions.iterator();
  }

  public void addToExtensions(String elem) {
    if (this.extensions == null) {
      this.extensions = new ArrayList<String>();
    }
    this.extensions.add(elem);
  }

  public List<String> getExtensions() {
    return this.extensions;
  }

  public MediaType setExtensions(List<String> extensions) {
    this.extensions = extensions;
    return this;
  }

  public void unsetExtensions() {
    this.extensions = null;
  }

  /** Returns true if field extensions is set (has been assigned a value) and false otherwise */
  public boolean isSetExtensions() {
    return this.extensions != null;
  }

  public void setExtensionsIsSet(boolean value) {
    if (!value) {
      this.extensions = null;
    }
  }

  public void setFieldValue(_Fields field, Object value) {
    switch (field) {
    case MEDIA_NAME:
      if (value == null) {
        unsetMediaName();
      } else {
        setMediaName((String)value);
      }
      break;

    case EXTENSIONS:
      if (value == null) {
        unsetExtensions();
      } else {
        setExtensions((List<String>)value);
      }
      break;

    }
  }

  public Object getFieldValue(_Fields field) {
    switch (field) {
    case MEDIA_NAME:
      return getMediaName();

    case EXTENSIONS:
      return getExtensions();

    }
    throw new IllegalStateException();
  }

  /** Returns true if field corresponding to fieldID is set (has been assigned a value) and false otherwise */
  public boolean isSet(_Fields field) {
    if (field == null) {
      throw new IllegalArgumentException();
    }

    switch (field) {
    case MEDIA_NAME:
      return isSetMediaName();
    case EXTENSIONS:
      return isSetExtensions();
    }
    throw new IllegalStateException();
  }

  @Override
  public boolean equals(Object that) {
    if (that == null)
      return false;
    if (that instanceof MediaType)
      return this.equals((MediaType)that);
    return false;
  }

  public boolean equals(MediaType that) {
    if (that == null)
      return false;

    boolean this_present_mediaName = true && this.isSetMediaName();
    boolean that_present_mediaName = true && that.isSetMediaName();
    if (this_present_mediaName || that_present_mediaName) {
      if (!(this_present_mediaName && that_present_mediaName))
        return false;
      if (!this.mediaName.equals(that.mediaName))
        return false;
    }

    boolean this_present_extensions = true && this.isSetExtensions();
    boolean that_present_extensions = true && that.isSetExtensions();
    if (this_present_extensions || that_present_extensions) {
      if (!(this_present_extensions && that_present_extensions))
        return false;
      if (!this.extensions.equals(that.extensions))
        return false;
    }

    return true;
  }

  @Override
  public int hashCode() {
    return 0;
  }

  @Override
  public int compareTo(MediaType other) {
    if (!getClass().equals(other.getClass())) {
      return getClass().getName().compareTo(other.getClass().getName());
    }

    int lastComparison = 0;

    lastComparison = Boolean.valueOf(isSetMediaName()).compareTo(other.isSetMediaName());
    if (lastComparison != 0) {
      return lastComparison;
    }
    if (isSetMediaName()) {
      lastComparison = org.apache.thrift.TBaseHelper.compareTo(this.mediaName, other.mediaName);
      if (lastComparison != 0) {
        return lastComparison;
      }
    }
    lastComparison = Boolean.valueOf(isSetExtensions()).compareTo(other.isSetExtensions());
    if (lastComparison != 0) {
      return lastComparison;
    }
    if (isSetExtensions()) {
      lastComparison = org.apache.thrift.TBaseHelper.compareTo(this.extensions, other.extensions);
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
    StringBuilder sb = new StringBuilder("MediaType(");
    boolean first = true;

    sb.append("mediaName:");
    if (this.mediaName == null) {
      sb.append("null");
    } else {
      sb.append(this.mediaName);
    }
    first = false;
    if (!first) sb.append(", ");
    sb.append("extensions:");
    if (this.extensions == null) {
      sb.append("null");
    } else {
      sb.append(this.extensions);
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

  private static class MediaTypeStandardSchemeFactory implements SchemeFactory {
    public MediaTypeStandardScheme getScheme() {
      return new MediaTypeStandardScheme();
    }
  }

  private static class MediaTypeStandardScheme extends StandardScheme<MediaType> {

    public void read(org.apache.thrift.protocol.TProtocol iprot, MediaType struct) throws org.apache.thrift.TException {
      org.apache.thrift.protocol.TField schemeField;
      iprot.readStructBegin();
      while (true)
      {
        schemeField = iprot.readFieldBegin();
        if (schemeField.type == org.apache.thrift.protocol.TType.STOP) { 
          break;
        }
        switch (schemeField.id) {
          case 1: // MEDIA_NAME
            if (schemeField.type == org.apache.thrift.protocol.TType.STRING) {
              struct.mediaName = iprot.readString();
              struct.setMediaNameIsSet(true);
            } else { 
              org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
            }
            break;
          case 2: // EXTENSIONS
            if (schemeField.type == org.apache.thrift.protocol.TType.LIST) {
              {
                org.apache.thrift.protocol.TList _list44 = iprot.readListBegin();
                struct.extensions = new ArrayList<String>(_list44.size);
                for (int _i45 = 0; _i45 < _list44.size; ++_i45)
                {
                  String _elem46;
                  _elem46 = iprot.readString();
                  struct.extensions.add(_elem46);
                }
                iprot.readListEnd();
              }
              struct.setExtensionsIsSet(true);
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

    public void write(org.apache.thrift.protocol.TProtocol oprot, MediaType struct) throws org.apache.thrift.TException {
      struct.validate();

      oprot.writeStructBegin(STRUCT_DESC);
      if (struct.mediaName != null) {
        oprot.writeFieldBegin(MEDIA_NAME_FIELD_DESC);
        oprot.writeString(struct.mediaName);
        oprot.writeFieldEnd();
      }
      if (struct.extensions != null) {
        oprot.writeFieldBegin(EXTENSIONS_FIELD_DESC);
        {
          oprot.writeListBegin(new org.apache.thrift.protocol.TList(org.apache.thrift.protocol.TType.STRING, struct.extensions.size()));
          for (String _iter47 : struct.extensions)
          {
            oprot.writeString(_iter47);
          }
          oprot.writeListEnd();
        }
        oprot.writeFieldEnd();
      }
      oprot.writeFieldStop();
      oprot.writeStructEnd();
    }

  }

  private static class MediaTypeTupleSchemeFactory implements SchemeFactory {
    public MediaTypeTupleScheme getScheme() {
      return new MediaTypeTupleScheme();
    }
  }

  private static class MediaTypeTupleScheme extends TupleScheme<MediaType> {

    @Override
    public void write(org.apache.thrift.protocol.TProtocol prot, MediaType struct) throws org.apache.thrift.TException {
      TTupleProtocol oprot = (TTupleProtocol) prot;
      BitSet optionals = new BitSet();
      if (struct.isSetMediaName()) {
        optionals.set(0);
      }
      if (struct.isSetExtensions()) {
        optionals.set(1);
      }
      oprot.writeBitSet(optionals, 2);
      if (struct.isSetMediaName()) {
        oprot.writeString(struct.mediaName);
      }
      if (struct.isSetExtensions()) {
        {
          oprot.writeI32(struct.extensions.size());
          for (String _iter48 : struct.extensions)
          {
            oprot.writeString(_iter48);
          }
        }
      }
    }

    @Override
    public void read(org.apache.thrift.protocol.TProtocol prot, MediaType struct) throws org.apache.thrift.TException {
      TTupleProtocol iprot = (TTupleProtocol) prot;
      BitSet incoming = iprot.readBitSet(2);
      if (incoming.get(0)) {
        struct.mediaName = iprot.readString();
        struct.setMediaNameIsSet(true);
      }
      if (incoming.get(1)) {
        {
          org.apache.thrift.protocol.TList _list49 = new org.apache.thrift.protocol.TList(org.apache.thrift.protocol.TType.STRING, iprot.readI32());
          struct.extensions = new ArrayList<String>(_list49.size);
          for (int _i50 = 0; _i50 < _list49.size; ++_i50)
          {
            String _elem51;
            _elem51 = iprot.readString();
            struct.extensions.add(_elem51);
          }
        }
        struct.setExtensionsIsSet(true);
      }
    }
  }

}

