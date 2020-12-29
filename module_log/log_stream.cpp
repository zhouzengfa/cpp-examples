#include "log_stream.h"

LogStream::LogStream(const char* szFun, const int nLine, const int nLevel):
	m_Function(szFun),
	m_nLine(nLine),
	m_nLevel(nLevel)
{
	switch (m_nLevel)
	{
	case ELEVEL_INFO:
		m_Data << "\033[33m";
		break;
	case ELEVEL_ERROR:
		m_Data << "\033[31m";
		break;
	default:
		break;
	}
}

LogStream::~LogStream()
{
	switch (m_nLevel)
	{
	case ELEVEL_ERROR:
	case ELEVEL_INFO:
		m_Data << "\033[0m";
		break;
	default:
		break;
	}

	std::cout << m_Function.c_str() << " line:" << m_nLine << " " << m_Data.str().c_str() << std::endl;
}

LogStream& LogStream::operator<<(const char* t)
{
	if (nullptr != t)
		m_Data << t;

	return *this;
}
