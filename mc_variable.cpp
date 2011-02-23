#include "mc_variable.h"

  //Build my object
McVariable::McVariable() : Type( TAG_UNKNOWN ) {}
McVariable::McVariable( const McVariable& base)
  { *this = base; }
McVariable::McVariable( char v )
  { Tag_Byte = v; Type = TAG_BYTE; }
McVariable::McVariable( short v )
  { Tag_Short = v; Type = TAG_SHORT; }
McVariable::McVariable( int v )
  { Tag_Int = v; Type = TAG_INT; }
McVariable::McVariable( qlonglong v )
  { Tag_Long = v; Type = TAG_LONG; }
McVariable::McVariable( float v )
  { Tag_Float = v; Type = TAG_FLOAT; }
McVariable::McVariable( double v )
  { Tag_Double = v; Type = TAG_DOUBLE; }
McVariable::McVariable( QByteArray v )
  { Tag_Byte_Array = v; Type = TAG_BYTE_ARRAY; }
McVariable::McVariable( QString v )
  { Tag_String = v; Type = TAG_STRING; }
McVariable::McVariable( McList<McVariable> v )
  { Tag_List = v; Type = TAG_LIST; }
McVariable::McVariable( McHash<McVariable> v )
{ Tag_Hash = v; Type = TAG_COMPOUND; }

  //! \brief Read back the tag
McVariable::TagType McVariable::type()
{
  return Type;
}

  //Return my values
char& McVariable::toChar()      { return Tag_Byte; }
short& McVariable::toShort()    { return Tag_Short; }
int& McVariable::toInt()        { return Tag_Int; } 
qlonglong& McVariable::toLong() { return Tag_Long; } 
float& McVariable::toFloat()    { return Tag_Float; } 
double& McVariable::toDouble()  { return Tag_Double; } 
QByteArray& McVariable::toByteArray()     { return Tag_Byte_Array; } 
QString& McVariable::toString()           { return Tag_String; } 
McList<McVariable>& McVariable::toList()  { return Tag_List; } 
McHash<McVariable>& McVariable::toHash()  { return Tag_Hash; } 

  //Store the operator
char& McVariable::operator=( char v )
  { Type = TAG_BYTE; return Tag_Byte = v; } 
short& McVariable::operator=( short v )
  { Type = TAG_SHORT; return Tag_Short = v; } 
int& McVariable::operator=( int v )
  { Type = TAG_INT; return Tag_Int = v; } 
qlonglong& McVariable::operator=( qlonglong v )
  { Type = TAG_LONG; return Tag_Long = v; } 
float& McVariable::operator=( float v )
  { Type = TAG_FLOAT; return Tag_Float = v; } 
double& McVariable::operator=( double v )
  { Type = TAG_DOUBLE; return Tag_Double = v; } 
QByteArray& McVariable::operator=( QByteArray v )
  { Type = TAG_BYTE_ARRAY; return Tag_Byte_Array = v; } 
QString& McVariable::operator=( QString v )
  { Type = TAG_STRING; return Tag_String = v; } 
McList<McVariable>& McVariable::operator=( McList<McVariable> v )
  { Type = TAG_LIST; return Tag_List = v; } 
McHash<McVariable>& McVariable::operator=( McHash<McVariable> v )
  { Type = TAG_COMPOUND; return Tag_Hash = v; } 

  //Push an item onto my list
void McVariable::push( McVariable v )
{
  if ( Type == TAG_LIST )
    Tag_List.push_back( v );
}

  //Push an item into my hash
void McVariable::insert( QString k, McVariable v )
{
  if ( Type == TAG_COMPOUND )
    Tag_Hash.insert( k, v );
}

  //Gives list access
McVariable& McVariable::operator[]( int idx )
{
  return (Type == TAG_LIST)? Tag_List[idx]: *this;
}

McVariable& McVariable::operator[]( QString k )
{
  return (Type == TAG_COMPOUND)? Tag_Hash[k]: *this;
}

  //Give the size if any
int McVariable::size()
{
  if ( Type == TAG_LIST )
    return Tag_List.size();
  else if ( Type == TAG_COMPOUND )
    return Tag_Hash.size();
  else
    return -1;
}
