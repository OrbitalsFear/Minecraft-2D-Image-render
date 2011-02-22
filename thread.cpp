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
#include <math.h>

#include "thread.h"

#define THREAD_COUNT 8

#define xyz( x, y, z ) (x * Height + z * Square + y)
#define mxyz( x, y, z ) (z * 128 + x * 2048 + y)
#define dxyz( x, y, z ) (z * 64 + x * 1024 + y)
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
  int x, y, z, yt;
//  double dx, dy, dz;
  int px, pz;
  int t1, t2, t3;
//  double dist;
//  double min_dist;
//  QRgb t,s,f;
  QString fx, fz, fux, fuz;
//  int block;
//  int data;
  int i;
  bool done;

    //Load up my image
  QImage front( "Front_50.png" );
  QImage side( "Side_50.png" );
  QImage top( "Top_50.png" );

    //Get my basic world info
  Width = front.width();
  Height = side.height();
  Depth = top.width();
  Square = Width * Height;
  Size = Square * Depth;

    //Alloc my world
  qDebug("Alloc World: %d\n", Size );
  Blocks = new unsigned char[ Size ];
  Data = new unsigned char[ Size ];

    //Zero all the memory
  memset( Blocks, 0, Size );
  memset( Data, 0, Size );

  BlockComp** block_comp = new BlockComp*[THREAD_COUNT];
  for ( i = 0; i < THREAD_COUNT; i++ )
  {
    block_comp[i] = new BlockComp(Blocks, Data, 
                                  Width, Depth, Height, 
                                  &front, &side, &top );
    
      //Reset my y to start
    if ( i == 0 ) block_comp[i]->reset();
    block_comp[i]->start();
  }

    //Wait until we're done
  do {
    QThread::yieldCurrentThread();

      //Check for being done
    for ( done = true, i = 0; done && i < THREAD_COUNT; i++ )
      done = (done & block_comp[i]->isFinished());

  } while ( !done );

/*
    //Reading in Image
  qDebug("Reading in Image\n" );
  for ( y = 0; y < Height; y++ )
  {
    qDebug("Calc %d/%d\n", y, Height );
    for ( z = 0; z < Depth; z++ )
      for ( x = 0; x < Width; x++ )
      {
        t = (top.pixel( z, x )   | 0xFF000000);
        s = (side.pixel( z, y )  | 0xFF000000);
        f = (front.pixel( x, y ) | 0xFF000000);

          //Try to calculate what color best fits the bsg
        min_dist = 1;
        block = data = 0;

          //Get my xyz
        calcPoint( dx, dy, dz, t, s, f );

        //Figure out waht color is closets

          //White space
        if ( min_dist > (dist = calcDist( dx, dy, dz, rgb( 255, 0, 255 )) ) )
        {
          min_dist = dist;
          block = 0;
          data = 0;
        }

          //Gray
        if ( min_dist > (dist = calcDist( dx, dy, dz, rgb(0xB0, 0xB0, 0xB0))))
        {
          min_dist = dist;
          block = 0x23;
          data = 0x01;
        }
          //Dark Gray
        if ( min_dist > (dist = calcDist( dx, dy, dz, rgb(0x44, 0x44, 0x44))))
        {
          min_dist = dist;
          block = 0x23;
          data = 0x02;
        }
          //Black
        if ( min_dist > (dist = calcDist( dx, dy, dz, rgb(0x00, 0x00, 0x00))))
        {
          min_dist = dist;
          block = 0x23;
          data = 0x03;
        }
          //Red
        if ( min_dist > (dist = calcDist( dx, dy, dz, rgb(0xCC, 0x00, 0x00))))
        {
          min_dist = dist;
          block = 0x23;
          data = 0x04;
        }
          //Orange
        if ( min_dist > (dist = calcDist( dx, dy, dz, rgb(0xFF, 0x7F, 0x00))))
        {
          min_dist = dist;
          block = 0x23;
          data = 0x05;
        }
          //Yellow
        if ( min_dist > (dist = calcDist( dx, dy, dz, rgb(0xFF, 0xFF, 0x00))))
        {
          min_dist = dist;
          block = 0x23;
          data = 0x06;
        }


        Blocks[ xyz( x, y, z )] = block;
        Data[ xyz( x, y, z )] = data;
      }
  }
*/

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
        fux = QString::number( px & 0x3F, 36 );
      }
      else
      {
        fx = QString("-%1").arg(QString::number( -px, 36 ));
        fux = QString::number( (((~(-px)) & 0xFF) + 1) & 0x3F, 36);
      }
      if ( pz >= 0 )
      {
        fz = QString::number( pz, 36 );
        fuz = QString::number( pz & 0x3F, 36 );
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
      bData.fill(0);
      bHeightMap.fill( Height / 2 + 5 );
      bBlocks.fill(0);

        //Load my object with data
      for ( z = 0; z < 16; z++ )
        for ( x = 0; x < 16; x++ )
          for ( y = 0; y < Height; y++ )
          {
              //get my t's
            yt = (Height - y) + 5;
            t1 = mxyz( x, yt, z );
            t2 = xyz( (x + px * 16), y, (z + pz * 16) );
            t3 = dxyz( x, (yt / 2), z );

              //Store an image
            if ( px >= 0 && pz >= 0 && 
                  x + px * 16 < Width && z + pz * 16 < Depth && y < Height )
            {
              bBlocks[t1] = Blocks[t2];
              bData[t3] = (bData[t3] | ((Data[t2] & 0x0F) << ((yt & 1) * 4)));
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
