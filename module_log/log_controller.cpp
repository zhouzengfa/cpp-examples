#include "log_controller.h"
#include <gtest/gtest.h>

TEST(TopK, testMaxTopK)
{
	TopK<A*, MaxComp> topk(3);

	A* p = new A(2);
	topk.push(p);
	p = new A(3);
	topk.push(p);
	p = new A(1);
	topk.push(p);
	p = new A(4);
	topk.push(p);
	p = new A(6);
	topk.push(p);
	p = new A(5);
	topk.push(p);
	auto list = topk.getTopK();
	std::cout << std::endl;
	ASSERT_EQ(3,list.size());
	ASSERT_EQ(4,list[0]->a);
	ASSERT_EQ(5,list[1]->a);
	ASSERT_EQ(6,list[2]->a);
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

int LogModule::RegisterModule(std::string& moduleName)
{
	for (int i= 0; i < m_modules.size(); ++i)
	{
		if (moduleName == m_modules[i])
			return i;
	}

	int index = m_modules.size();
	m_modules.push_back(moduleName);

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

bool LogModule::needLog(int moduleIndex)
{
	if (moduleIndex >= m_length)
		return false;

	return m_bitData[moduleIndex] > 0;
}

