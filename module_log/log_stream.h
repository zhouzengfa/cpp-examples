#pragma once

#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

enum
{
	ELEVEL_DEBUG = 1,
	ELEVEL_INFO = 2,
	ELEVEL_ERROR = 3,
};

class LogStream
{
public:
	LogStream(const char* szFun, const int nLine, const int nLevel);
	~LogStream();

	template<typename T>
	LogStream& operator << (const T& t);
	LogStream& operator << (const char* t);

private:
	std::string			m_Function;
	int					m_nLine;
	int					m_nLevel;
	std::ostringstream	m_Data;

};

template<typename T>
LogStream& LogStream::operator<<(const T& t)
{
	m_Data << t;
	return *this;
}

class LogMessageVoidify {
public:
	LogMessageVoidify() { }
	void operator&(LogStream&) { }
};
