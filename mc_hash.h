#ifndef MC_HASH_H
#define MC_HASH_H

#include <QString>
#include <QHash>
#include <QList>

  //Holds a list of mc variables
template <typename T>
class McHash : public QHash<QString, T>
{
  private:
    //Keeps the keys
  QList<QString> Keys;
  
  public:
    //Return my fancy ordered keys
  QList<QString>  keys() const
  {
    return Keys;
  }

    //Catch the inserts
  void insert ( const QString & key, const T & value )
  {
    Keys.push_back( key );
    QHash<QString, T>::insert( key, value );
  }
};

#endif
