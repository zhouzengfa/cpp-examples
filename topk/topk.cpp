#include "topk.h"
#include <gtest/gtest.h>
#include "../module_log/log_controller.h"

DEFINE_LOG(TopK)

class A
{
public:
	A(int n) { a = n; }
	~A() {};
	int a = 0;

	void print()
	{
		std::cout << a << ",";
	}
};

/*由于STL自带优先队列是默认最大优先的，所以自己写了一个比较函数，将其改为最小优先*/
class MaxComp
{
public:
	bool operator()(A* a, A* b)
	{
		return a->a > b->a;		//最小值优先
	}
};

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

class MinComp
{
public:
	bool operator()(A* a, A* b)
	{
		return a->a < b->a;		//最小值优先
	}
};

TEST(TopK, testMinTopK)
{
	TopK<A*, MinComp> topk(3);

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
	ASSERT_EQ(3,list[0]->a);
	ASSERT_EQ(2,list[1]->a);
	ASSERT_EQ(1,list[2]->a);
}

