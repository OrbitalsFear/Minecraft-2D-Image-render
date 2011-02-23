#ifndef MC_LIST_H
#define MC_LIST_H

#include <QList>

  //Holds a list of mc variables
template <typename T>
class McList : public QList<T>
{
  private:
    //Holds the type
  char Type;
  
  public:
    //set the type
  void setType( char type ) { Type = type; }

    //Return teh types
  char type() { return Type; }
};

#endif
