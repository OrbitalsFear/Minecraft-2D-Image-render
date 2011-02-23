#ifndef MINECRAFT_BYTE_ARRAY_H
#define MINECRAFT_BYTE_ARRAY_H

#include <QByteArray>
#include <QString>
#include "mc_variable.h"

  //Reads minecraft support values
class MinecraftByteArray: public QByteArray
{
  private:
    //Holds a index to by byte array
  int Index;

  public:
    //Make my object
  MinecraftByteArray();

    //Make my object form a byte array
  MinecraftByteArray( QByteArray& ary );

    //Make my object form a byte array
  MinecraftByteArray( const char*data, int len );

    //Reset the Index
  void reset();

    //Returns true if we've read all the data
  bool isDone();

    //Read a single byte
  char readByte();

    //Read a short
  short readShort();

    //Read a int
  int readInt();

    //Read a 64bit int
  qlonglong readLong();

    //Read a float
  float readFloat();

    //Read a double
  double readDouble();

    //Reads raw data
  QByteArray readRaw( int len );

    //Reads a name for a named tag
  QString readName();

    //Read a named byte
  bool readNamedByte( QString& name, McVariable& value );

    //Read a named Short
  bool readNamedShort( QString& name, McVariable& value );

    //Read a named int
  bool readNamedInt( QString& name, McVariable& value );

    //Read a named long
  bool readNamedLong( QString& name, McVariable& value );

    //Read a named float
  bool readNamedFloat( QString& name, McVariable& value );

    //Read a named double
  bool readNamedDouble( QString& name, McVariable& value );

    //Read a named string
  bool readNamedString( QString& name, McVariable& value );

    //Read a named byte_array
  bool readNamedByteArray( QString& name, McVariable& value );

    //Read a named list
  bool readNamedList( QString& name, char& type, int& count );

    //Read a named hash
  bool readNamedHash( QString& name );
};

#endif
