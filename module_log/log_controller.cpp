#include "log_controller.h"
#include <gtest/gtest.h>

DEFINE_LOG(LogModule);

// 测试用例
TEST(LogModule, testLogModule)
{
	DEBUG_LOG << "test0 ";
	DEBUG_LOG << "test1 ";
	LogModule::getInst().UnregisterModule("LogModule");
	DEBUG_LOG << "test2 ";
	LogModule::getInst().RegisterModule("LogModule");
	ERROR_LOG << "test3 ";
}

LogModule::LogModule()
{
	m_modules.clear();
	m_bitData = new char[m_length];
	memset(m_bitData, 0, m_length);
}

LogModule::~LogModule()
{
	delete m_bitData;
}


LogModule& LogModule::getInst()
{
	static LogModule inst;
	return inst;
}

int LogModule::RegisterModule(const char* szName)
{
	std::string moduleName(szName);
	int index = -1;
	for (size_t i= 0; i < m_modules.size(); ++i)
	{
		if (moduleName == m_modules[i])
		{
			index = i;
			break;
		}
	}

	if (index < 0)
	{
		index = m_modules.size();
		m_modules.push_back(moduleName);
	}

	if (index < m_length)
	{
		m_bitData[index] = 1;
	}
	else
	{
		char* p = new char[m_length * 2];
		memcpy(p, m_bitData, m_length);
		delete m_bitData;
		m_bitData = p;
		m_length *= 2;
	}

	return index;
}

bool LogModule::UnregisterModule(const char* szName)
{
	std::string moduleName(szName);
	int index = -1;
	for (size_t i = 0; i < m_modules.size(); ++i)
	{
		if (moduleName != m_modules[i])
			continue;

		index = i;
		break;
	}

	if (index < 0)
		return false;

	m_bitData[index] = 0;
	return true;
}

bool LogModule::needLog(int moduleIndex)
{
	if (moduleIndex >= m_length)
		return false;

	return m_bitData[moduleIndex] > 0;
}

