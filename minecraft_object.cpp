#include "minecraft_object.h"
#include <string.h>

  //Loads my endian specific data into memory
void MinecraftObject::loadShort( short v, char* buf )
{
  buf[0] = ((v >> 8) & 0xFF);
  buf[1] = ((v >> 0) & 0xFF);
}

  //Load up some int
void MinecraftObject::loadInt( int v, char* buf )
{
  buf[0] = ((v >> 24) & 0xFF);
  buf[1] = ((v >> 16) & 0xFF);
  buf[2] = ((v >>  8) & 0xFF);
  buf[3] = ((v >>  0) & 0xFF);
}

  //Load a long long long long
void MinecraftObject::loadLong( qlonglong v, char* buf )
{
  buf[0] = ((v >> 56) & 0xFF);
  buf[1] = ((v >> 48) & 0xFF);
  buf[2] = ((v >> 40) & 0xFF);
  buf[3] = ((v >> 32) & 0xFF);
  buf[4] = ((v >> 24) & 0xFF);
  buf[5] = ((v >> 16) & 0xFF);
  buf[6] = ((v >>  8) & 0xFF);
  buf[7] = ((v >>  0) & 0xFF);
}

  //Load a float
void MinecraftObject::loadFloat( float v, char* buf )
{
  char* val = (char*)&v;

  buf[0] = val[3];
  buf[1] = val[2];
  buf[2] = val[1];
  buf[3] = val[0];
}

  //Load a double
void MinecraftObject::loadDouble( double v, char* buf )
{
  char* val = (char*)&v;

  buf[0] = val[7];
  buf[1] = val[6];
  buf[2] = val[5];
  buf[3] = val[4];
  buf[4] = val[3];
  buf[5] = val[2];
  buf[6] = val[1];
  buf[7] = val[0];
}

    //Used to make offsets when printing
void MinecraftObject::offset( int depth )
{
  for ( int i = 0; i <= depth; i++ )
    qDebug("    ");
}

//Construct a minecraft object from a hash
MinecraftObject::MinecraftObject()
{
}

  //Construct a minecraft object from a hash
MinecraftObject::MinecraftObject( McVariable& obj ) :
  McHash<McVariable>( obj.toHash() )
{
}

  //Store the object name
void MinecraftObject::storeName( QString name )
{
  Name = name;
}

  //Read back the name
QString MinecraftObject::name()
{
  return Name;
}

  //Dump the contents of this file
void MinecraftObject::dump()
{
  qDebug("Object Dump: %s {\n", Name.toAscii().data() );
  dumpHash( this, 0 );
}

  //Dump out a hash
void MinecraftObject::dumpHash( McHash<McVariable>* hash, int depth )
{
  QList<QString> list = hash->keys();
  int i, len;
  for ( i = 0, len = list.size(); i < len; i++ )
  {
    QString name = list[i];
    McVariable val = McVariable( (*hash)[name] );

    offset( depth ); 
    switch ( val.type() )
    {
      case McVariable::TAG_END: //NEver get these actually
        qDebug("}\n");
        return;

      case McVariable::TAG_BYTE:
        qDebug( "Byte (%s): 0x%02X\n", name.toAscii().data(), val.toChar() );
        break;

      case McVariable::TAG_SHORT:
        qDebug( "Short (%s): %d\n", name.toAscii().data(), val.toShort() );
        break;

      case McVariable::TAG_INT:
        qDebug( "Int (%s): %d\n", name.toAscii().data(), val.toInt() );
        break;

      case McVariable::TAG_LONG:
        qDebug( "Long (%s): %d\n", name.toAscii().data(), (int)val.toLong() );
        break;

      case McVariable::TAG_FLOAT:
        qDebug( "Float (%s): %f\n", name.toAscii().data(), val.toFloat() );
        break;

      case McVariable::TAG_DOUBLE:
        qDebug( "Double (%s): %f\n", name.toAscii().data(), val.toDouble() );
        break;

      case McVariable::TAG_BYTE_ARRAY:
        qDebug( "Byte Array (%s): [%d]\n", name.toAscii().data(), val.toByteArray().size() );
        break;

      case McVariable::TAG_STRING:
        qDebug( "String (%s): %s\n", name.toAscii().data(), val.toString().toAscii().data() );
        break;

      case McVariable::TAG_LIST:
        qDebug("List (%s) [%d] {\n",name.toAscii().data(), val.toList().size());
        dumpList( val.toList(), depth + 1 );
        break;

      case McVariable::TAG_COMPOUND:
        qDebug( "Hash (%s) {\n", name.toAscii().data() );
        dumpHash( &val.toHash(), depth + 1 );
        break;

      default: break;
    }
  }

  offset( depth - 1 ); 
  qDebug("}\n");
}

  //Dump a list
void MinecraftObject::dumpList( McList<McVariable>& list, int depth )
{
  int i, len;
  McVariable val;

    //Go through my list
  for ( i = 0, len = list.size(); i < len; i++ )
  {
      //SEtup for my switch
    offset( depth ); 
    val = list[i]; 

      //Figure out hwat it is, this is wasteful, we could make this faster
    switch ( val.type() )
    {
      case McVariable::TAG_BYTE:
        qDebug( "Byte: 0x%02X\n", val.toChar() );
        break;

      case McVariable::TAG_SHORT:
        qDebug( "Short: %d\n", val.toShort() );
        break;

      case McVariable::TAG_INT:
        qDebug( "Int %d\n", val.toInt() );
        break;

      case McVariable::TAG_LONG:
        qDebug( "Long: %d\n", (int)val.toLong() );
        break;

      case McVariable::TAG_FLOAT:
        qDebug( "Float: %f\n", val.toFloat() );
        break;

      case McVariable::TAG_DOUBLE:
        qDebug( "Double: %f\n", val.toDouble() );
        break;

      case McVariable::TAG_BYTE_ARRAY:
        qDebug( "Byte Array: [%d]\n", val.toByteArray().size() );
        break;

      case McVariable::TAG_STRING:
        qDebug( "String: %s\n", val.toString().toAscii().data() );
        break;

      case McVariable::TAG_LIST:
        qDebug("List [%d] {\n", val.toList().size());
        dumpList( val.toList(), depth + 1 );
        break;

      case McVariable::TAG_COMPOUND:
        qDebug( "Hash {\n" );
        dumpHash( &val.toHash(), depth + 1 );
        break;

      default: break;
    }
  }

  offset( depth - 1 ); 
  qDebug("}\n");
}

  //Dump the contents of this file
int MinecraftObject::load( QByteArray& data )
{
    //Set the size ahead of time
  data.reserve( DEFAULT_SIZE );

    //Start out my data
  Tmp[0] = McVariable::TAG_COMPOUND;
  loadShort( Name.size(), Tmp + 1 );
  memcpy( Tmp + 3, Name.toAscii().data(), Name.size() );

    //Load up the data
  data.append( Tmp, Name.size() + 3 );
  
    //Load up my data
  loadHash( data, this );
  return data.size();
}

  //Dump out a hash
void MinecraftObject::loadHash( QByteArray& data, McHash<McVariable>* hash )
{
  QList<QString> list = hash->keys();
  int i, len;
  for ( i = 0, len = list.size(); i < len; i++ )
  {
    QString name = list[i];
    McVariable val = McVariable( (*hash)[name] );

      //Store the type and name
    Tmp[0] = val.type();
    loadShort( name.size(), Tmp + 1 );
    memcpy( Tmp + 3, name.toAscii().data(), name.size() );
    data.append( Tmp, name.size() + 3 );

      //Store the value
    switch ( val.type() )
    {
      case McVariable::TAG_END:  //Not possible
        return;

      case McVariable::TAG_BYTE:
        data.append( val.toChar() );
        break;

      case McVariable::TAG_SHORT:
        loadShort( val.toShort(), Tmp );
        data.append( Tmp, 2 );
        break;

      case McVariable::TAG_INT:
        loadInt( val.toInt(), Tmp );
        data.append( Tmp, 4 );
        break;

      case McVariable::TAG_LONG:
        loadLong( val.toLong(), Tmp );
        data.append( Tmp, 8 );
        break;

      case McVariable::TAG_FLOAT:
        loadFloat( val.toFloat(), Tmp );
        data.append( Tmp, 4 );
        break;

      case McVariable::TAG_DOUBLE:
        loadDouble( val.toDouble(), Tmp );
        data.append( Tmp, 8 );
        break;

      case McVariable::TAG_BYTE_ARRAY:
        loadInt( val.toByteArray().size(), Tmp );
        data.append( Tmp, 4 );
        data.append( val.toByteArray() );
        break;

      case McVariable::TAG_STRING:
        data.append( val.toString().toAscii() );
        break;

      case McVariable::TAG_LIST:
        loadList( data, val.toList() );
        break;

      case McVariable::TAG_COMPOUND:
        loadHash( data, &val.toHash() );
        break;

      default: break;
    }
  }

    //Put on my end hash
  data.append( McVariable::TAG_END );
}

  //Dump a list
void MinecraftObject::loadList( QByteArray& data, McList<McVariable>& list )
{
  int i, len;

    //Store the type and name
  Tmp[0] = list.type();
  loadInt( list.size(), Tmp + 1 );
  data.append( Tmp, 5 );

    //Figure out hwat it is, this is wasteful, we could make this faster
  switch ( list.type() )
  {
    case McVariable::TAG_BYTE:
      for ( i = 0, len = list.size(); i < len; i++ )
        data.append( list[i].toChar() );
      break;

    case McVariable::TAG_SHORT:
      for ( i = 0, len = list.size(); i < len; i++ )
      {
        loadShort( list[i].toShort(), Tmp );
        data.append( Tmp, 2 );
      }
      break;

    case McVariable::TAG_INT:
      for ( i = 0, len = list.size(); i < len; i++ )
      {
        loadInt( list[i].toInt(), Tmp );
        data.append( Tmp, 4 );
      }
      break;

    case McVariable::TAG_LONG:
      for ( i = 0, len = list.size(); i < len; i++ )
      {
        loadLong( list[i].toLong(), Tmp );
        data.append( Tmp, 8 );
      }
      break;

    case McVariable::TAG_FLOAT:
      for ( i = 0, len = list.size(); i < len; i++ )
      {
        loadFloat( list[i].toFloat(), Tmp );
        data.append( Tmp, 4 );
      }
      break;

    case McVariable::TAG_DOUBLE:
      for ( i = 0, len = list.size(); i < len; i++ )
      {
        loadDouble( list[i].toDouble(), Tmp );
        data.append( Tmp, 8 );
      }
      break;

    case McVariable::TAG_BYTE_ARRAY:
      for ( i = 0, len = list.size(); i < len; i++ )
      {
        loadInt( list[i].toByteArray().size(), Tmp );
        data.append( Tmp, 4 );
        data.append( list[i].toByteArray() );
      }
      break;

    case McVariable::TAG_STRING:
      for ( i = 0, len = list.size(); i < len; i++ )
        data.append( list[i].toString().toAscii() );
      break;

    case McVariable::TAG_LIST:
      for ( i = 0, len = list.size(); i < len; i++ )
        loadList( data, list[i].toList() );
      break;

    case McVariable::TAG_COMPOUND:
      for ( i = 0, len = list.size(); i < len; i++ )
        loadHash( data, &list[i].toHash() );
      break;

    default: break;
  }
}
