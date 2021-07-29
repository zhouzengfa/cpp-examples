#include "vfactor.h"
#include <gtest/gtest.h>
#include "../module_log/log_controller.h"

DEFINE_LOG(VFactor)

static const VFactor zero(0L, 1L);
static const VFactor one(1L, 1L);
static const VFactor pi(31416L, 10000L);
static const VFactor twoPi(62832L, 10000L);


TEST(VFactor, testAdd)
{
	VFactor f1(23124L, 1000L);
	VFactor f2(34392L, 10000L);

	VFactor f3(0,0);
	for (int i = 0; i < 20000000; ++i)
	{
		f3 = f1 + f2;
	}
}

TEST(VFactor, testMinus)
{
	VFactor f1(23124L, 1000L);
	VFactor f2(34392L, 10000L);

	VFactor f3(0, 0);
	for (int i = 0; i < 20000000; ++i)
	{
		f3 = f1 - f2;
	}
}

TEST(VFactor, testMulti)
{
	VFactor f1(23124L, 1000L);
	VFactor f2(34392L, 10000L);

	VFactor f3(0, 0);
	for (int i = 0; i < 20000000; ++i)
	{
		f3 = f1 * f2;
	}
}

TEST(VFactor, testDivid)
{
	VFactor f1(23124L, 1000L);
	VFactor f2(34392L, 10000L);

	VFactor f3(0, 0);
	for (int i = 0; i < 20000000; ++i)
	{
		f3 = f1 / f2;
	}
}


