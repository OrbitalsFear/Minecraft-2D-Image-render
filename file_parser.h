#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include "minecraft_byte_array.h"
#include "mc_variable.h"
#include "minecraft_object.h"

  //My file parser
class FileParser
{
    //Loads unamed entries
  void loadUnnamed( MinecraftByteArray& data, McVariable& obj_list,
                    char type, int count );

    //Loads named entries
  void loadNamed( MinecraftByteArray& data, McVariable& obj );

  public:
    //Load a file
  MinecraftObject loadData( MinecraftByteArray data );
};

#endif
