#ifndef MC_VARIABLE_H
#define MC_VARIABLE_H

#include <QString>
#include <QHash>
#include <QByteArray>
#include "mc_list.h"
#include "mc_hash.h"

  //Define my list
  //! \brief Wraps primative types with a class
class McVariable
{
  public:
      //Minecraft tags
  typedef enum TagTypes {
    TAG_UNKNOWN     = -1,
    TAG_END         = 0,
    TAG_BYTE        = 1,
    TAG_SHORT       = 2,
    TAG_INT         = 3,
    TAG_LONG        = 4,
    TAG_FLOAT       = 5,
    TAG_DOUBLE      = 6,
    TAG_BYTE_ARRAY  = 7,
    TAG_STRING      = 8,
    TAG_LIST        = 9,
    TAG_COMPOUND    = 10
  } TagType;

  protected:
    //Holds the tag type
  TagType Type;

    //My variables
  char        Tag_Byte;
  short       Tag_Short;
  int         Tag_Int;
  qlonglong   Tag_Long;
  float       Tag_Float;
  double      Tag_Double;
  QByteArray  Tag_Byte_Array;
  QString     Tag_String;
  McList<McVariable>          Tag_List;
  McHash<McVariable>  Tag_Hash;

  public:
    //! \brief Copy myself
  McVariable();
  McVariable( const McVariable& base );
  McVariable( char v );
  McVariable( short v );
  McVariable( int v );
  McVariable( qlonglong v );
  McVariable( float v );
  McVariable( double v );
  McVariable( QByteArray v );
  McVariable( QString v );
  McVariable( McList<McVariable> v );
  McVariable( McHash<McVariable> v );

    //! \brief Read back the tag
  TagType type();

    //Gives access to my variables
  char&       toChar();
  short&      toShort();
  int&        toInt();
  qlonglong&  toLong();
  float&      toFloat();
  double&     toDouble();
  QByteArray& toByteArray();
  QString&    toString();
  McList<McVariable>& toList();
  McHash<McVariable>& toHash();

    //! \brief Defines a basic copy constructor for the primative variable
  char& operator=( char v );
  short& operator=( short v );
  int& operator=( int v );
  qlonglong& operator=( qlonglong v );
  float& operator=( float v );
  double& operator=( double v );
  QByteArray& operator=( QByteArray v );
  QString& operator=( QString v );
  McList<McVariable>& operator=( McList<McVariable> v );
  McHash<McVariable>& operator=( McHash<McVariable> v );

    //Makes dealing with hashes and lists nice
  void push( McVariable v );
  void insert( QString k, McVariable v );

    //Provde access
  McVariable& operator[]( int idx );
  McVariable& operator[]( QString );

    //Give the size if any
  int size();
};

#endif
