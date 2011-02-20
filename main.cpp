#include <qcoreapplication.h>
#include <stdio.h>

#include "thread.h"
#include "minecraft_object.h"
#include "file_parser.h"

void myMessageOutput(QtMsgType type, const char *msg)
{
  switch (type) {
    case QtDebugMsg:
      fprintf(stderr, "%s", msg);
      break;
    case QtWarningMsg:
      fprintf(stderr, "Warning: %s", msg);
      break;
    case QtCriticalMsg:
      fprintf(stderr, "Critical: %s", msg);
      break;
    case QtFatalMsg:
      fprintf(stderr, "Fatal: %s", msg);
      break;
   }
}

int main( int argc, char** argv )
{
  qInstallMsgHandler(myMessageOutput);
  QCoreApplication* app = new QCoreApplication(argc, argv);

    //Quit if we suck
  if ( argc != 2 )
    return printf("Usage: %s OUTPUT_DIR\n", argv[0] );

  Thread thread ( argv[1], argv[1] );
  thread.startThread();

  return app->exec();
}
