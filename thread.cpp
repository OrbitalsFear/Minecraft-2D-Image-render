#include <QTimer>
#include <QImage>
#include <QRgb>
#include <QColor>
#include <QFile>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <qcoreapplication.h>

#include "minecraft_object.h"

#include "thread.h"

#define xyz( x, y, z ) (x + z * Width + y * Square)
#define mxyz( x, y, z ) (z * 128 + x * 2048 + y)
#define rgb( r, g, b ) (0xFF000000 | ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF))

Thread::Thread( char* image, char* dir ) : Image_Filename( image ), Dir( dir )
{
}

void Thread::startThread()
{
  QTimer::singleShot(0, this, SLOT(thread()));
}

void Thread::thread()
{
  int x, y, z;
  int px, pz;
  int t1, t2;

    //Load up my image
  qDebug("Loading: %s\n", Image_Filename.toAscii().data() );
  QImage image( Image_Filename );

    //Get my basic world info
  Width = image.width();
  Height = image.height();
  Square = Width * Height;
  Size = Square * 64;

    //Error if the image size is invalid
  if ( (Width % 16) != 0 || (Height % 16) != 0 )
  {
    qDebug("Invalid image size, must be divisiable by 16!\n");
    QCoreApplication::exit(0);
    return;
  }

    //Alloc my world
  qDebug("Alloc World: %d\n", Size );
  Data = new unsigned char[ Size ];
  Blocks = new unsigned char[ Size * 2 ];

    //Zero all the memory
  memset( Data, 0, Size );
  memset( Blocks, 0, Size * 2 );

    //Lay down a floor
  qDebug("Laying down floor\n" );
  for ( z = 0; z < Height; z++ )
    for ( x = 0; x < Width; x++ )
          Blocks[ xyz(x, 1, z)] = 7;

    //Create my level file
  qDebug("Building Level object\n" );
  MinecraftObject level;
  level.storeName( QString("") );
  level.insert( QString("Time"), McVariable((qlonglong)0) );
  level.insert( QString("LastPlayed"), McVariable((qlonglong)0) );
  level.insert( QString("Player"), McVariable(McHash<McVariable>()) );
  level["Player"].insert( QString("Motion"), McVariable(McList<McVariable>()));
    level["Player"]["Motion"].toList().setType( McVariable::TAG_DOUBLE );
    level["Player"]["Motion"].push( (double)0 );
    level["Player"]["Motion"].push( (double)0 );
    level["Player"]["Motion"].push( (double)0 );
  level["Player"].insert( QString("OnGround"),  McVariable((char)0x01));
  level["Player"].insert( QString("HurtTime"),  McVariable((short)0));
  level["Player"].insert( QString("Health"),    McVariable((short)20));
  level["Player"].insert( QString("Dimension"), McVariable((int)0));
  level["Player"].insert( QString("Air"),       McVariable((short)300));
  level["Player"].insert(QString("Inventory"),McVariable(McList<McVariable>()));
  level["Player"].insert(QString("Pos"),McVariable(McList<McVariable>()));
    level["Player"]["Pos"].toList().setType( McVariable::TAG_DOUBLE );
    level["Player"]["Pos"].push( (double)0 );
    level["Player"]["Pos"].push( (double)2.5 );
    level["Player"]["Pos"].push( (double)0 );
  level["Player"].insert( QString("AttackTime"),McVariable((short)0));
  level["Player"].insert( QString("Fire"), McVariable((short)-20));
  level["Player"].insert( QString("FallDistance"), McVariable((float)0));
  level["Player"].insert(QString("Rotation"),McVariable(McList<McVariable>()));
    level["Player"]["Rotation"].toList().setType( McVariable::TAG_FLOAT );
    level["Player"]["Rotation"].push( (float)0 );
    level["Player"]["Rotation"].push( (float)0 );
  level["Player"].insert( QString("Score"),     McVariable((int)0));
  level["Player"].insert( QString("DeathTime"), McVariable((short)0));
  level.insert( QString("SpawnX"), McVariable((int)0) );
  level.insert( QString("SpawnY"), McVariable((int)2) );
  level.insert( QString("SpawnZ"), McVariable((int)0) );
  level.insert( QString("SizeOnDisk"), McVariable((qlonglong)91700) );
  level.insert( QString("RandomSeed"), McVariable((qlonglong)0x917) );

    //Build a map object
  MinecraftObject map;
  map.storeName( QString("") );
  map.insert( QString("Level"), McVariable(McHash<McVariable>()) );
  map["Level"].insert( QString("Data"), McVariable(QByteArray()) );
  map["Level"].insert( QString("Entities"), McVariable(McList<McVariable>()));
    map["Player"]["Entities"].toList().setType( McVariable::TAG_COMPOUND );
  map["Level"].insert( QString("LastUpdate"), McVariable((qlonglong)0));
  map["Level"].insert( QString("xPos"), McVariable((int)0));
  map["Level"].insert( QString("zPos"), McVariable((int)0));
  map["Level"].insert(QString("TileEntities"),McVariable(McList<McVariable>()));
    map["Player"]["TileEntities"].toList().setType( McVariable::TAG_COMPOUND);
  map["Level"].insert( QString("TerrainPopulated"), McVariable((char)0x01));
  map["Level"].insert( QString("SkyLight"), McVariable(QByteArray()));
  map["Level"].insert( QString("HeightMap"), McVariable(QByteArray()));
  map["Level"].insert( QString("BlockLight"), McVariable(QByteArray()));
  map["Level"].insert( QString("Blocks"), McVariable(QByteArray()));

    //Reading in Image
  qDebug("Reading in Image\n" );
  for ( z = 0; z < Height; z++ )
    for ( x = 0; x < Width; x++ )
      switch ( image.pixel( x, z ) )
      {
        case rgb( 0x91, 0x77, 0x29 ): //Starting point
          level["Player"]["SpawnX"] = (int)x;
          level["Player"]["SpawnZ"] = (int)z;
          level["Player"]["Pos"][0] = (double)x;
          level["Player"]["Pos"][2] = (double)z;
          break;

          //Block
        case rgb( 0, 0, 0 ): 
          Blocks[ xyz(x,2,z)] = 0x04;
          break;

          //Block
        case rgb( 0, 0, 0x7f ): 
          Blocks[ xyz(x,3,z)] = 0x04;
          break;

          //Block
        case rgb( 0, 0, 0xff ): 
          Blocks[ xyz(x,4,z)] = 0x04;
          break;

          //Redstone
        case rgb( 0xFF, 0, 0 ): 
          Blocks[ xyz(x,2,z)] = 0x37; 
          break;

          //Redstone on a block
        case rgb( 0xFF, 0, 0x7f ): 
          Blocks[ xyz(x,3,z)] = 0x37; 
          Blocks[ xyz(x,2,z)] = 0x04; 
          break;

          //Redstone on a block
        case rgb( 0xFF, 0, 0xff ): 
          Blocks[ xyz(x,4,z)] = 0x37; 
          Blocks[ xyz(x,3,z)] = 0x04; 
          break;

          //Torch
        case rgb( 0, 0xff, 0 ): 
          Blocks[ xyz(x,2,z)] = 0x4c;
          break;


        default: break;
      }

    //Now that we've read all my data in, write the map out
  qDebug("Writing Level\n" );
  FILE* handle;

    //start piping data
  if ( (handle = popen( QString("gzip > %1/level.dat").arg(Dir).toAscii().data(), "w")) == NULL )
  {
    qDebug("Couldn't write level.dat\n");
    QCoreApplication::exit(0);
    return;
  }

    //Write out the data
  QByteArray level_dat;
  level.load( level_dat );
  fwrite( level_dat.data(), 1, level_dat.size(), handle );
  pclose( handle );

    //Write out the maps
  for ( pz = 0; pz < Height / 16; pz++ )
    for ( px = 0; px < Width / 16; px++ )
    {
      QString fx = QString::number( px, 36 );
      QString fz = QString::number( pz, 36 );
      //QString fux = QString::number( (*(unsigned int*)(char*)&px) % 64, 36 );
      //QString fuz = QString::number( (*(unsigned int*)(char*)&pz) % 64, 36 );
      QString fux = QString::number( px % 64, 36 );
      QString fuz = QString::number( pz % 64, 36 );
      QString filename = QString("%1/%2/%3/c.%4.%5.dat").
        arg(Dir).arg(fux).arg(fuz).arg(fx).arg(fz);

        //Make my directors
      int ret;
      ret = system(QString("mkdir %1/%2 2> /dev/null").arg(Dir).arg(fux).toAscii().data());
      ret = system(QString("mkdir %1/%2/%3 2> /dev/null").arg(Dir).arg(fux).arg(fuz).toAscii().data());

        //Start building the map
      qDebug("Building Map %s... ", filename.toAscii().data() );
      fflush( stdout );

        //Reset my data buffers
      QByteArray bData;
      QByteArray bSkyLight;
      QByteArray bHeightMap;
      QByteArray bBlockLight;
      QByteArray bBlocks;

        //Size my stuff
      bData.resize( 16 * 16 * 64 );
      bSkyLight.resize( 16 * 16 * 64 );
      bHeightMap.resize( 16 * 16 );
      bBlockLight.resize( 16 * 16 * 64 );
      bBlocks.resize( 16 * 16 * 64 * 2);

        //Fill some lighting stuff
      bSkyLight.fill(0xFF);
      bBlockLight.fill(0xFF);

        //Load my object with data
      for ( y = 0; y < 128; y++ )
        for ( z = 0; z < 16; z++ )
          for ( x = 0; x < 16; x++ )
          {
            t1 = mxyz( x, y, z );
            t2 = xyz( (x + px * 16), y, (z + pz * 16) );

///Buidl a dome
            double dist = 
              ((x + px * 16) - 200) *
              ((x + px * 16) - 200) +
              ((z + pz * 16) - 200) *
              ((z + pz * 16) - 200) +
              y * y;

            if ( dist >= 12700 && dist <= 12996 )
              bBlocks[t1] = 0x14;
            else if ( dist > 12996 && y <= 50 )
              bBlocks[t1] = 0x08;
            else
//End build dome
                //Store my info
              bBlocks[t1] = Blocks[t2];
          }

        //Set the position
      map["Level"]["xPos"] = px;
      map["Level"]["zPos"] = pz;

        //Store my data
      map["Level"]["Data"] = bData;
      map["Level"]["SkyLight"] = bSkyLight;
      map["Level"]["HeightMap"] = bHeightMap;
      map["Level"]["BlockLight"] = bBlockLight;
      map["Level"]["Blocks"] = bBlocks;

        //Write out the data
      qDebug("Writing\n");

        //start piping data
      if ( (handle = popen(QString("gzip > %1").arg(filename).toAscii().data(),
                            "w")) == NULL )
      {
        qDebug("Couldn't write level.dat\n");
        QCoreApplication::exit(0);
        return;
      }

        //Write out the data
      QByteArray map_dat;
      map.load( map_dat );
      fwrite( map_dat.data(), 1, map_dat.size(), handle );
      pclose( handle );
    }

  QCoreApplication::exit(0);
}
