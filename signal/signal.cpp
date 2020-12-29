#include "signal.h"
#include <iostream>
#include <signal.h>
#include "../module_log/log_controller.h"

DEFINE_LOG(SignalMgr)

void handler(int sigNum);

bool SignalDefaultHandler::process(int sigNum)
{
	DEBUG_LOG << "sigNum:" << sigNum;
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
	DEBUG_LOG << "receive signal:" << sigNum;
	signalsReceived_.push_back(sigNum);
	process();
}

bool SignalMgr::installSignal(int sigNum, SignalHandler* pHandler)
{
	auto iter = signalHandlers_.find(sigNum);
	if (iter != signalHandlers_.end())
	{
		DEBUG_LOG << "signal:" << sigNum << " was override";
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

	DEBUG_LOG << "install signal:" << sigNum;

	return true;
}

bool SignalMgr::uninstallSignal(int sigNum)
{
	auto iter = signalHandlers_.find(sigNum);
	if (iter != signalHandlers_.end())
	{
		DEBUG_LOG << "signal:" << sigNum << " was uninstalled.";
		delete (iter->second);
		signalHandlers_.erase(iter);

		return true;
	}

	DEBUG_LOG << "can't find signal:" << sigNum;
	return false;
}

bool SignalMgr::ignoreSignal(int sigNum)
{
	if (signal(sigNum, SIG_IGN) == SIG_ERR)
		return false;

	DEBUG_LOG << "signal:" << sigNum << " was ignore.";
	return true;
}

bool SignalMgr::process()
{
	for (int sig : signalsReceived_)
	{
		auto iter = signalHandlers_.find(sig);
		if (iter == signalHandlers_.end())
		{
			DEBUG_LOG << "can't find signal(" << sig << ") handler.";
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
