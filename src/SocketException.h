// SocketException class


#ifndef SocketException_class
#define SocketException_class

#include <string>

class SocketException
{
 public:
  SocketException ( int c,  std::string s ) : m_c ( c ), m_s ( s ) {};
  ~SocketException (){};
  std::string description() { return m_s; }
  int code() { return m_c; }
  int returnMessage( char * ret, size_t limit);
 private:
  int m_c;
  std::string m_s;
};

#endif
