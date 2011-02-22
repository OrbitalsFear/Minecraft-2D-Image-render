#ifndef BLOCK_COMP_H
#define BLOCK_COMP_H

#include <QThread>
#include <QMutex>
#include <QColor>
#include <QRgb>
#include <QImage>

  //Run in a thread
class BlockComp : public QThread
{
  private:
    //Buffer pointers
  unsigned char* Blocks;
    //Buffer pointers
  unsigned char* Data;
    //Store my height and width and depth
  int Width;
  int Depth;
  int Height;
    //Store my image pointer
  QImage* Front;
  QImage* Side;
  QImage* Top;

  public:
    //Constructor
  BlockComp( unsigned char* block, unsigned char* data,
             int width, int depth, int height,
             QImage* front, QImage* side, QImage* top );

    //Calc my points
  void calcPoint( double& x, double& y, double& z, QRgb c );
  void calcPoint( double& x, double& y, double& z, QRgb c1, QRgb c2, QRgb c3 );
  double calcDist( double& x1, double& y1, double& z1,
             double& x2, double& y2, double& z2 );
  double calcDist( double& x, double& y, double& z, QRgb c );

    //Reset my Y
  void reset();

    //Read a y value
  int readY();

    //Run in a thread
  void run();
};

#endif
