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
  QRgb t,s,f;
  QString fx, fz, fux, fuz;

    //Load up my image
  QImage front( "Front_50.png" );
  QImage side( "Side_50.png" );
  QImage top( "Top_50.png" );

    //Get my basic world info
  Width = front.width();
  Height = side.height();
  Depth = top.width();
  Square = Width * Depth;
  Size = Square * Height;

    //Alloc my world
  qDebug("Alloc World: %d\n", Size );
  Blocks = new unsigned char[ Size ];

    //Zero all the memory
  memset( Blocks, 0, Size );

    //Reading in Image
  qDebug("Reading in Image\n" );
  for ( y = 0; y < Height; y++ )
    for ( z = 0; z < Depth; z++ )
      for ( x = 0; x < Width; x++ )
      {
        t = top.pixel( z, x );
        s = side.pixel( z, y );
        f = front.pixel( x, y );

          //If this is inside each image, we have a winner!
        if (t != rgb( 0xFF, 0x0, 0xFF ) &&
            s != rgb( 0xFF, 0x0, 0xFF ) &&
            f != rgb( 0xFF, 0x0, 0xFF ) )
          Blocks[ xyz( x, y, z )] = 0x04;
      }

    //Create my level file
  qDebug("Building Level object\n" );
  MinecraftObject level;
  level.storeName( QString("") );
  level.insert( QString("Data"), McVariable(McHash<McVariable>()) );
  level["Data"].insert( QString("Time"), McVariable((qlonglong)0) );
  level["Data"].insert( QString("LastPlayed"), McVariable((qlonglong)1106379604) );
  level["Data"].insert( QString("Player"), McVariable(McHash<McVariable>()) );
  level["Data"]["Player"].insert( QString("Motion"), McVariable(McList<McVariable>()));
    level["Data"]["Player"]["Motion"].toList().setType( McVariable::TAG_DOUBLE );
    level["Data"]["Player"]["Motion"].push( (double)0 );
    level["Data"]["Player"]["Motion"].push( (double)-.078 );
    level["Data"]["Player"]["Motion"].push( (double)0 );
  level["Data"]["Player"].insert( QString("OnGround"),  McVariable((char)0x01));
  level["Data"]["Player"].insert( QString("HurtTime"),  McVariable((short)0));
  level["Data"]["Player"].insert( QString("Health"),    McVariable((short)20));
  level["Data"]["Player"].insert( QString("Dimension"), McVariable((int)0));
  level["Data"]["Player"].insert( QString("Air"),       McVariable((short)300));
  level["Data"]["Player"].insert(QString("Inventory"),McVariable(McList<McVariable>()));
  level["Data"]["Player"].insert(QString("Pos"),McVariable(McList<McVariable>()));
    level["Data"]["Player"]["Pos"].toList().setType( McVariable::TAG_DOUBLE );
    level["Data"]["Player"]["Pos"].push( (double)1 );
    level["Data"]["Player"]["Pos"].push( (double)4.5 );
    level["Data"]["Player"]["Pos"].push( (double)1 );
  level["Data"]["Player"].insert( QString("AttackTime"),McVariable((short)0));
  level["Data"]["Player"].insert( QString("Fire"), McVariable((short)-20));
  level["Data"]["Player"].insert( QString("FallDistance"), McVariable((float)0));
  level["Data"]["Player"].insert(QString("Rotation"),McVariable(McList<McVariable>()));
    level["Data"]["Player"]["Rotation"].toList().setType( McVariable::TAG_FLOAT );
    level["Data"]["Player"]["Rotation"].push( (float)-433.350037 );
    level["Data"]["Player"]["Rotation"].push( (float)10.650003 );
  level["Data"]["Player"].insert( QString("Score"),     McVariable((int)0));
  level["Data"]["Player"].insert( QString("DeathTime"), McVariable((short)0));
  level["Data"].insert( QString("SpawnX"), McVariable((int)1) );
  level["Data"].insert( QString("SpawnY"), McVariable((int)3) );
  level["Data"].insert( QString("SpawnZ"), McVariable((int)1) );
  level["Data"].insert( QString("SizeOnDisk"), McVariable((qlonglong)91700) );
  level["Data"].insert( QString("RandomSeed"), McVariable((qlonglong)0x917) );

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
  for ( pz = -6; pz < Depth / 16 + 7; pz++ )
    for ( px = -6; px < Width / 16 + 7; px++ )
    {
      if ( px >= 0 )
      {
        fx = QString::number( px, 36 );
        fux = QString::number( px & 0x1F, 36 );
      }
      else
      {
        fx = QString("-%1").arg(QString::number( -px, 36 ));
        fux = QString::number( (((~(-px)) & 0xFF) + 1) & 0x3F, 36);
      }
      if ( pz >= 0 )
      {
        fz = QString::number( pz, 36 );
        fuz = QString::number( pz & 0x1F, 36 );
      }
      else
      {
        fz = QString("-%1").arg(QString::number( -pz, 36 ));
        fuz = QString::number( (((~(-pz)) & 0xFF) + 1) & 0x3F, 36);
      }
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
      bBlocks.resize( 16 * 16 * 128);

        //Fill some lighting stuff
      bSkyLight.fill(0xff);
      bBlockLight.fill(0xff);
      bData.fill(0xFF);
      bHeightMap.fill(1);
      bBlocks.fill(0);

        //Load my object with data
      for ( y = 0; y < Height; y++ )
        for ( z = 0; z < 16; z++ )
          for ( x = 0; x < 16; x++ )
          {
              //get my t's
            t1 = mxyz( x, (Height - y) + 5, z );
            t2 = xyz( (x + px * 16), y, (z + pz * 16) );

              //Store an image
            if ( px >= 0 && pz >= 0 && 
                  x + px * 16 < Width && z + pz * 16 < Depth && y < Height )
            {
              bBlocks[t1] = Blocks[t2];
              if ( (int)bHeightMap[x + z * 16] < (127 - y) )
                bHeightMap[x + z * 16] = (127 - y);
            }
          }

        //Lay down a floor
      for ( z = 0; z < 16; z++ )
        for ( x = 0; x < 16; x++ )
          bBlocks[ mxyz( x, 0, z ) ] = 0x07;

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
