#pragma once

#ifdef __linux__

#include <map>
#include <list>

class SignalHandler
{
public:
	virtual ~SignalHandler() {}
	virtual bool process(int sigNum) = 0;
};

class SignalDefaultHandler : public SignalHandler
{
public:
	virtual bool process(int sigNum);
};

class SignalMgr
{
public:
	virtual ~SignalMgr();
	static SignalMgr& getInst();
	void onSignal(int sigNum);

	bool installSignal(int sigNum, SignalHandler* pHandler);
	bool uninstallSignal(int sigNum);
	bool ignoreSignal(int sigNum);

	bool process();

protected:
	SignalMgr();

private:
	std::list<int> signalsReceived_;
	std::map<int, SignalHandler*> signalHandlers_;
};

#endif
