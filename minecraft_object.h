#ifndef MINECRAFT_OBJECT_H
#define MINECRAFT_OBJECT_H

#include <QString>
#include "mc_variable.h"
#include "mc_hash.h"

  //A minecraft object
class MinecraftObject : public McHash<McVariable>
{
  public:
    //Default size
  static const int DEFAULT_SIZE = 65536;

    //Holds my buffer size, used for inserting names
  static const int NAME_BUFFER_SIZE = 256;

  private:
    //Holds the name
  QString Name;

    //Holds a buffer for outputing values
  char Tmp[ NAME_BUFFER_SIZE];

    //Loads my endian specific data into memory
  void loadShort( short v, char* buf );
  void loadInt( int v, char* buf );
  void loadLong( qlonglong v, char* buf );
  void loadFloat( float v, char* buf );
  void loadDouble( double v, char* buf );

    //Used to make offsets when printing
  void offset( int depth );

  public:
    //Construct a minecraft object from a hash
  MinecraftObject();

    //Construct a minecraft object from a hash
  MinecraftObject( McVariable& obj );

    //Store the object name
  void storeName( QString name );

    //Read back the name
  QString name();

    //Dump the contents of this file
  void dump();

  private:
    //Dump the contents of this file
  void dumpHash( McHash<McVariable>* hash, int depth );

    //Dump the contents of this list
  void dumpList( McList<McVariable>& list, int depth );

  public:
    //Load the contence of the object into a byte array
  int load( QByteArray& data );

  private:
    //Dump the contents of this file
  void loadHash( QByteArray& data, McHash<McVariable>* hash );

    //Dump the contents of this list
  void loadList( QByteArray& data, McList<McVariable>& list );
};

#endif
