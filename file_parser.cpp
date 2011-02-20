#include "file_parser.h"

#include "mc_hash.h"
#include "mc_list.h"

  //Load unnamed entries
void FileParser::loadUnnamed( MinecraftByteArray& data, McVariable& obj_list,
                              char type, int count )
{
  int i;
  McList<McVariable> list;
  McVariable tmp_value;
  char tmp_type;
  int tmp_count;

    //Set my size
  list.reserve( count );

    //Fill up my list
  switch ( type )
  {
    case McVariable::TAG_BYTE:
      for ( i = 0; i < count; i++ )
        list.append( McVariable( data.readByte() ) );
      break;

    case McVariable::TAG_SHORT:
      for ( i = 0; i < count; i++ )
        list.append( McVariable( data.readShort() ) );
      break;

    case McVariable::TAG_INT:
      for ( i = 0; i < count; i++ )
        list.append( McVariable( data.readInt() ) );
      break;

    case McVariable::TAG_LONG:
      for ( i = 0; i < count; i++ )
        list.append( McVariable( data.readLong() ) );
      break;

    case McVariable::TAG_FLOAT:
      for ( i = 0; i < count; i++ )
        list.append( McVariable( data.readFloat() ) );
      break;

    case McVariable::TAG_DOUBLE:
      for ( i = 0; i < count; i++ )
        list.append( McVariable( data.readDouble() ) );
      break;

    case McVariable::TAG_BYTE_ARRAY:
      for ( i = 0; i < count; i++ )
        list.append( McVariable( data.readRaw( data.readInt()) ) );
      break;

    case McVariable::TAG_STRING:
      for ( i = 0; i < count; i++ )
        list.append( McVariable( data.readName() ) );
      break;

    case McVariable::TAG_LIST:
      for ( i = 0; i < count; i++ )
      {
        tmp_value = McList<McVariable>();
        tmp_type =  data.readByte();
        tmp_count = data.readInt();
        tmp_value.toList().setType( type );

          //Load up a list of a list
        loadUnnamed( data, tmp_value, tmp_type, tmp_count );
        list.append( tmp_value );
      }
      break;

    case McVariable::TAG_COMPOUND:
      for ( i = 0; i < count; i++ )
      {
        tmp_value = McVariable( McHash<McVariable>() );

          //Load up a list of a list
        loadNamed( data, tmp_value );
        list.append( tmp_value );
      }
      break;

    default: break;
  }

    //Set my list to the user's object
  list.setType( type );
  obj_list = list;
}

  //Load up named entires
void FileParser::loadNamed( MinecraftByteArray& data, 
                            McVariable& obj )
{
  QString name;
  McVariable value;
  char tmp_type;
  int tmp_count;

    //Read data while there's data
  while ( !data.isDone() )
  {
    value = McVariable();

      //Read in teh data type
    switch ( data.readByte() )
    {
      case McVariable::TAG_END:
        return;

      case McVariable::TAG_BYTE:
        data.readNamedByte( name, value );
        break;

      case McVariable::TAG_SHORT:
        data.readNamedShort( name, value );
        break;

      case McVariable::TAG_INT:
        data.readNamedInt( name, value );
        break;

      case McVariable::TAG_LONG:
        data.readNamedLong( name, value );
        break;

      case McVariable::TAG_FLOAT:
        data.readNamedFloat( name, value );
        break;

      case McVariable::TAG_DOUBLE:
        data.readNamedDouble( name, value );
        break;

      case McVariable::TAG_BYTE_ARRAY:
        data.readNamedByteArray( name, value );
        break;

      case McVariable::TAG_STRING:
        data.readNamedString( name, value );
        break;

      case McVariable::TAG_LIST:
        data.readNamedList( name, tmp_type, tmp_count );
        loadUnnamed( data, value, tmp_type, tmp_count );
        break;

      case McVariable::TAG_COMPOUND:
        data.readNamedHash( name );
        value = McVariable( McHash<McVariable>() );

        loadNamed( data, value );
        break;

      default: break;
    }

      //Load up the value found this time
    obj.toHash().insert( name, value );
  }
}

  //Load up data
MinecraftObject FileParser::loadData( MinecraftByteArray data )
{
  QString name;
  McVariable obj;
  char tag;

    //REad off the first compound
  tag = data.readByte();
  if ( tag != McVariable::TAG_COMPOUND )
    throw QString("Invalid Start token: %d != %d\n").
            arg(tag).arg(McVariable::TAG_COMPOUND);

    //Store the name and start read data
  name = data.readName();
  loadNamed( data, obj );

    //Make my result
  MinecraftObject result( obj );
  result.storeName( name );
  return result;
}
