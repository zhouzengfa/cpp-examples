#include <gtest/gtest.h>
#include "topk/topk.h"
#include "signal/signal.h"
#include "argp_parse/argp_parse.h"

int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);//将命令行参数传递给gtest
	int ret = RUN_ALL_TESTS();   //RUN_ALL_TESTS()运行所有测试案例

	/*
	{
		SignalMgr::getInst().installSignal(SIGTERM, new SignalDefaultHandler);
		SignalMgr::getInst().installSignal(SIGINT, new SignalDefaultHandler);
		SignalMgr::getInst().installSignal(SIGSEGV, new SignalDefaultHandler);

		auto p = new SignalDefaultHandler;
		delete p;
		p = nullptr;
		p->process(0);
		while (true)
		{
			SignalMgr::getInst().process();
			sleep(1);
		}
	}
	*/

	test_argp_parse(argc, argv);
	return ret;
}