#pragma once

#include <iostream>
#include <vector>
using namespace std;

class LogModule
{
public:
	LogModule();
	~LogModule();

	static LogModule& getInst();
	int RegisterModule(std::string& moduleName);
	bool needLog(int moduleIndex);

private:
	std::vector<std::string> m_modules;
	char* m_bitData = nullptr;
	int m_length = 1024;
};

#define DEFINE_LOG(module)	\
class logDefine##module \
{ \
public: \
	logDefine##module() \
	{ \
		index = LogModule::getInst().RegisterModule(##module); \
	} \
	bool needLog() \
	{ \
		return LogModule::getInst().needLog(index); \
	} \
private: \
	int index = -1; \
}; \
logDefine##module a##module; \
#define DEBUG !a##module.needLog() ? void(0) : std::cout << "["<<##module <<"]"

