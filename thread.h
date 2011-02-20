#ifndef THREAD_H
#define THREAD_H

#include <QObject>

class Thread : public QObject
{
  Q_OBJECT

  QString Image_Filename;
  QString Dir;

  unsigned char* Data;
  unsigned char* Blocks;
  int Width;
  int Height;
  int Depth;
  int Square;
  int Size;

  public:
  Thread( char* image, char* dir );

    //strat my thread
  void startThread();

  public slots:
  void thread();
};

#endif
