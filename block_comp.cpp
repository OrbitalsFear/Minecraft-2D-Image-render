#include "block_comp.h"
#include "math.h"

#define xyz( x, y, z ) (x * Height + z * square + y)
#define rgb( r, g, b ) (0xFF000000 | ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF))

  //The y index
int Y;
 //The mutex to do thread safe stuff
QMutex Mutex;
  //When we are done, we' are done
bool Done;

  //Run in a thread
BlockComp::BlockComp( unsigned char* block, unsigned char* data,
                      int width, int depth, int height,
                      QImage* front, QImage* side, QImage* top )
{
  Blocks = block;
  Data = data;

  Width = width;
  Depth = depth;
  Height = height;

  Front = front;
  Side = side;
  Top = top;
}

    //Calc my points
void BlockComp::calcPoint( double& x, double& y, double& z, QRgb c )
{
  QColor color = QColor( c );

  int hue = color.hsvHue();
  double h = (double)hue / 180.0 * M_PI;
  double s = (double)color.hsvSaturation() / 255.0;
  double v = (double)color.value() / 255.0;

    //Store the hue
  if ( hue >= 0 )
  {
    x = cos( h ) * s * v;
    z = sin( h ) * s * v;
  }
  else
    x = z =  0;
  y = v;
}

void BlockComp::calcPoint(double& x, double& y, double& z, 
                          QRgb c1, QRgb c2, QRgb c3 )
{
  double x1, y1, z1;

  calcPoint( x1, y1, z1, c1 );
  x = x1; y = y1; z = z1;
  calcPoint( x1, y1, z1, c2 );
  x += x1; y += y1; z += z1;
  calcPoint( x1, y1, z1, c3 );
  x += x1; y += y1; z += z1;

  x /= 3.0;
  y /= 3.0;
  z /= 3.0;
}

double BlockComp::calcDist( double& x1, double& y1, double& z1,
                            double& x2, double& y2, double& z2 )
{
  return sqrt( (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) +
               (z1 - z2) * (z1 - z2) );
}

double BlockComp::calcDist( double& x1, double& y1, double& z1, QRgb c )
{
  double x2, y2, z2;
  calcPoint( x2, y2, z2, c );
  return sqrt( (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) +
               (z1 - z2) * (z1 - z2) );
}

  //Reset my Y
void BlockComp::reset()
{
  Y = -1;
  Done = false;
  Mutex.unlock();
}

  //read the y value
int BlockComp::readY()
{
  int y;
  Mutex.lock();

    //Is y valid?
  if ( Done || Y + 1 >= Height )
  {
    Y = -1;
    Done = true;
    y = Y;
  }
  else
    y = ++Y;

  Mutex.unlock();

  return y;
}

  //Run in a thread
void BlockComp::run()
{
  int x, y, z;
  int idx;
  double dx, dy, dz;
  double dist;
  double min_dist;
  QRgb t,s,f;
  int block, data;

  int square = Width * Height;

    //Cach my colors
//  int color_count = 6;
//  double colors[6][3];

    //Calc my colors    

    //REad the y
  while ( (y = readY()) >= 0 )
  {
    qDebug("Calc %d/%d\n", y, Height );

      //Go through the images
    for ( z = 0; z < Depth; z++ )
      for ( x = 0; x < Width; x++ )
      {
        t = (Top->pixel( z, x )   | 0xFF000000);
        s = (Side->pixel( z, y )  | 0xFF000000);
        f = (Front->pixel( x, y ) | 0xFF000000);

          //Air, nothing to calculate
        if (t == rgb( 255, 0, 255 ) || 
            s == rgb( 255, 0, 255 ) || 
            f == rgb( 255, 0, 255 ) )
          continue;

          //Try to calculate what color best fits the bsg
        min_dist = 1;
        block = data = 0;

          //Get my xyz
        calcPoint( dx, dy, dz, t, s, f );

        //Figure out waht color is closets
        {
          min_dist = calcDist( dx, dy, dz, rgb(0xFF, 0x00, 0xFF));
          block = 0x0;
          data = 0x00;
        }

          //White, default color
        if ( min_dist > (dist = calcDist( dx, dy, dz, rgb(0xFF, 0xFF, 0xFF))))
        {
          min_dist = dist;
          block = 0x23;
          data = 0x00;
        }
      
          //Orange
        if ( min_dist > (dist = calcDist( dx, dy, dz, rgb(0xFF, 0x7F, 0x00))))
        {
          min_dist = dist;
          block = 0x23;
          data = 0x01;
        }
          //Light blue
        if ( min_dist > (dist = calcDist( dx, dy, dz, rgb(0x7F, 0x7F, 0xFF))))
        {
          min_dist = dist;
          block = 0x23;
          data = 0x03;
        }
          //Yellow
        if ( min_dist > (dist = calcDist( dx, dy, dz, rgb(0xFF, 0xFF, 0x00))))
        {
          min_dist = dist;
          block = 0x23;
          data = 0x04;
        }
          //Green
        if ( min_dist > (dist = calcDist( dx, dy, dz, rgb(0x00, 0xFF, 0x00))))
        {
          min_dist = calcDist( dx, dy, dz, rgb(0xB0, 0xB0, 0xB0));
          block = 0x23;
          data = 0x05;
        }
          //Dark Gray
        if ( min_dist > (dist = calcDist( dx, dy, dz, rgb(0x44, 0x44, 0x44))))
        {
          min_dist = dist;
          block = 0x23;
          data = 0x07;
        }
          //Light Gray
        if ( min_dist > (dist = calcDist( dx, dy, dz, rgb(0xB0, 0xB0, 0xB0))))
        {
          min_dist = calcDist( dx, dy, dz, rgb(0xB0, 0xB0, 0xB0));
          block = 0x23;
          data = 0x08;
        }
          //blue
        if ( min_dist > (dist = calcDist( dx, dy, dz, rgb(0x00, 0x00, 0xFF))))
        {
          min_dist = dist;
          block = 0x23;
          data = 0x0B;
        }
          //Brown
        if ( min_dist > (dist = calcDist( dx, dy, dz, rgb(0x80, 0x40, 0x00))))
        {
          min_dist = dist;
          block = 0x23;
          data = 0x0C;
        }
          //Red
        if ( min_dist > (dist = calcDist( dx, dy, dz, rgb(0xCC, 0x00, 0x00))))
        {
          min_dist = dist;
          block = 0x23;
          data = 0x0E;
        }
          //Black
        if ( min_dist > (dist = calcDist( dx, dy, dz, rgb(0x00, 0x00, 0x00))))
        {
          min_dist = dist;
          block = 0x23;
          data = 0x0F;
        }

          //Store my resulting blocks
        idx = xyz( x, y, z );
        Blocks[idx] = block;
        Data[idx] = data;
      }
  }
}
