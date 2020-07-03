#include "signal.h"
#include <iostream>
#include <signal.h>

#define debug_log std::cout<< __FUNCTION__ <<" "
#define error_log std::err<<__FUNCTION__<<" "

void handler(int sigNum);

bool SignalDefaultHandler::process(int sigNum)
{
	debug_log << "sigNum:" << sigNum << std::endl;
	return true;
}

SignalMgr::~SignalMgr()
{
	for (auto& entry : signalHandlers_)
	{
		auto pHandler = entry.second;
		if (nullptr != pHandler)
		{
			delete pHandler;
		}
	}

	signalHandlers_.clear();
	signalsReceived_.clear();
}

SignalMgr& SignalMgr::getInst()
{
	static SignalMgr instance;
	return instance;
}

void SignalMgr::onSignal(int sigNum)
{
	debug_log << "receive signal:" << sigNum << std::endl;
	signalsReceived_.push_back(sigNum);
	process();
}

bool SignalMgr::installSignal(int sigNum, SignalHandler* pHandler)
{
	auto iter = signalHandlers_.find(sigNum);
	if (iter != signalHandlers_.end())
	{
		debug_log << "signal:" << sigNum << " was override"<<std::endl;
		delete (iter->second);
		signalHandlers_.erase(iter);
	}

	signalHandlers_[sigNum] = pHandler;

	struct sigaction action;
	action.sa_flags = 0;
	//action.sa_flags = SA_RESETHAND;
	action.sa_handler = &handler;
	sigfillset(&(action.sa_mask));
	::sigaction(sigNum, &action, NULL);

	debug_log << "install signal:" << sigNum << std::endl;

	return true;
}

bool SignalMgr::uninstallSignal(int sigNum)
{
	auto iter = signalHandlers_.find(sigNum);
	if (iter != signalHandlers_.end())
	{
		debug_log << "signal:" << sigNum << " was uninstalled."<< std::endl;
		delete (iter->second);
		signalHandlers_.erase(iter);

		return true;
	}

	debug_log << "can't find signal:" << sigNum << std::endl;
	return false;
}

bool SignalMgr::ignoreSignal(int sigNum)
{
	if (signal(sigNum, SIG_IGN) == SIG_ERR)
		return false;

	debug_log << "signal:" << sigNum << " was ignore." << std::endl;
	return true;
}

bool SignalMgr::process()
{
	for (int sig : signalsReceived_)
	{
		auto iter = signalHandlers_.find(sig);
		if (iter == signalHandlers_.end())
		{
			debug_log << "can't find signal(" << sig << ") handler.";
			continue;
		}

		auto pHandler = iter->second;
		pHandler->process(sig);
	}

	signalsReceived_.clear();
	return true;
}

SignalMgr::SignalMgr()
{

}

void handler(int sigNum)
{
	SignalMgr::getInst().onSignal(sigNum);
}
