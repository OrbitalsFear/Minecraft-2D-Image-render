#include "minecraft_byte_array.h"

    //Make my object
MinecraftByteArray::MinecraftByteArray() : QByteArray()
{
  reset();
}

    //Make my object form a byte array
MinecraftByteArray::MinecraftByteArray( QByteArray& ary ) : QByteArray( ary )
{
  reset();
}

    //Make my object form a byte array
MinecraftByteArray::MinecraftByteArray( const char* data, int len ) : 
  QByteArray( data, len )
{
  reset();
}


  //Reset the Index
void MinecraftByteArray::reset()
{
  Index = 0;
}

  //Return true if we're done
bool MinecraftByteArray::isDone()
{
  return (Index >= size());
}

  //Read a single byte
char MinecraftByteArray::readByte()
{
  Index++; 
  return at(Index - 1);
}

  //Read a short
short MinecraftByteArray::readShort()
{
  const char* data = constData();
  Index += 2; 

  return ((data[Index - 2] & 0xFF) << 8) | 
         ((data[Index - 1] & 0xFF) << 0);
}

  //Read a int
int MinecraftByteArray::readInt()
{
  const char* data = constData();
  Index += 4; 

  return ((data[Index - 4] & 0xFF) << 24) |
         ((data[Index - 3] & 0xFF) << 16) |
         ((data[Index - 2] & 0xFF) <<  8) |
         ((data[Index - 1] & 0xFF) <<  0);
}

  //Read a 64bit int
qlonglong MinecraftByteArray::readLong()
{
  const char* data = constData();
  Index += 8; 

  return  (((qlonglong)data[Index - 8] & 0xFF) << 56) |
          (((qlonglong)data[Index - 7] & 0xFF) << 48) |
          (((qlonglong)data[Index - 6] & 0xFF) << 40) |
          (((qlonglong)data[Index - 5] & 0xFF) << 32) |
          (((qlonglong)data[Index - 4] & 0xFF) << 24) |
          (((qlonglong)data[Index - 3] & 0xFF) << 16) |
          (((qlonglong)data[Index - 2] & 0xFF) <<  8) |
          (((qlonglong)data[Index - 1] & 0xFF) <<  0);
}

  //Read a float
float MinecraftByteArray::readFloat()
{
  int a = readInt();
  return *(float*)(void*)&a;
}

  //Read a double
double MinecraftByteArray::readDouble()
{
  qlonglong a = readLong();
  return *(double*)(void*)&a;
}

  //Reads raw data
QByteArray MinecraftByteArray::readRaw( int len )
{
  Index += len;
  return mid( Index - len, len );
}

  //REad a name
QString MinecraftByteArray::readName()
{
  return QString( readRaw( readShort() ) );
}

  //Read a named byte
bool MinecraftByteArray::readNamedByte( QString& name, McVariable& value )
{
  name = readName();
  value = readByte();

  return true;
}

  //Read a named Short
bool MinecraftByteArray::readNamedShort( QString& name, McVariable& value )
{
  name = readName();
  value = readShort();

  return true;
}

  //Read a named int
bool MinecraftByteArray::readNamedInt( QString& name, McVariable& value )
{
  name = readName();
  value = readInt();

  return true;
}

  //Read a named long
bool MinecraftByteArray::readNamedLong( QString& name, McVariable& value )
{
  name = readName();
  value = readLong();

  return true;
}

  //Read a named float
bool MinecraftByteArray::readNamedFloat( QString& name, McVariable& value )
{
  name = readName();
  value = readFloat();

  return true;
}

  //Read a named double
bool MinecraftByteArray::readNamedDouble( QString& name, McVariable& value )
{
  name = readName();
  value = readDouble();

  return true;
}

  //Read a named string
bool MinecraftByteArray::readNamedString( QString& name, McVariable& value )
{
  name = readName();
  value = readName();

  return true;
}

  //Read a named byte_array
bool MinecraftByteArray::readNamedByteArray(QString& name, McVariable& value)
{
  name = readName();
  value = QByteArray( readRaw( readInt() ) );

  return true;
}

  //Read a named list
bool MinecraftByteArray::readNamedList(QString& name, char& type, int& count)
{
  name = readName();
  type = readByte(); 
  count = readInt();

  return true;
}

  //Read a named hash
bool MinecraftByteArray::readNamedHash( QString& name )
{
  name = readName();

  return true;
}
