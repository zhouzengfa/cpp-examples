#pragma once

/*
*代码采用STL中的最小优先队列实现，由于STL中自带最小优先队列，其底层就是二叉堆实现，
*所以就不再手写二叉堆了。最小优先队列顶层元素总是队列中最小的元素，也就是二叉堆堆顶。
*/

#include <iostream>
#include <queue>
#include <vector>
using namespace std;

template<class ObjType, class Comparator>
class TopK
{
public:
	TopK(int k)
	{
		m_k = k;
	}

	bool push(ObjType& obj)
	{
		if (minHeap.size() < m_k)
		{
			minHeap.push(obj);
			return true;
		}

		if (Comparator()(obj, minHeap.top()))
		{
			minHeap.pop();
			minHeap.push(obj);
			return true;
		}

		return false;
	}

	std::vector<ObjType> getTopK()
	{
		std::vector<ObjType> list;
		while (!minHeap.empty())
		{
			list.push_back(minHeap.top());
			minHeap.top()->print();
			minHeap.pop();
		}

		return std::move(list);
	}

private:
	int m_k = 0;
	priority_queue<ObjType, vector<ObjType>, Comparator> minHeap;			//建立最小优先队列
};

