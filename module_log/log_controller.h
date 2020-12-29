#pragma once

#include <iostream>
#include <vector>
#include "log_stream.h"

using namespace std;

class LogModule
{
public:
	LogModule();
	~LogModule();

	static LogModule& getInst();
	int RegisterModule(const char* moduleName);
	bool UnregisterModule(const char* moduleName);
	bool needLog(int moduleIndex);

private:
	std::vector<std::string> m_modules;
	char* m_bitData = nullptr;
	int m_length = 1024;
};

#define TO_STR(module) #module
#define DEFINE_LOG(module)															\
class logDefine##module																\
{																					\
public:																				\
	logDefine##module()																\
	{																				\
		moduleName=std::string(TO_STR(module));										\
		index = LogModule::getInst().RegisterModule(TO_STR(module));				\
	}																				\
	bool needLog()																	\
	{																				\
		return LogModule::getInst().needLog(index);									\
	}																				\
	const string& getModuleName() { return moduleName;}								\
private:																			\
	int index = -1;																	\
	std::string moduleName;															\
};																					\
static logDefine##module logLevel;

#define DEBUG_LOG !logLevel.needLog() ? void(0) : LogMessageVoidify() & LogStream( __FUNCTION__, __LINE__, ELEVEL_DEBUG) << "["<<logLevel.getModuleName()<<"]"
#define ERROR_LOG !logLevel.needLog() ? void(0) : LogMessageVoidify() & LogStream( __FUNCTION__, __LINE__, ELEVEL_ERROR) << "["<<logLevel.getModuleName()<<"]"
