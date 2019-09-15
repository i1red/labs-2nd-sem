#pragma once
#include <iostream>
using namespace std;

template <typename T>
struct cStack {
private:
	struct cell {
		T DATA;
		cell *NEXT;
	};
	cell *HEAD;
public:
	cStack()
	{
		HEAD = nullptr;
	}
	bool is_empty()
	{
		if (!HEAD)
			return true;
		else
			return false;
	}
	void push(T object)
	{
		cell *tmp = new cell;
		tmp->DATA = object;
		tmp->NEXT = HEAD;
		HEAD = tmp;
	}
	T pop()
	{
		if (!HEAD)
		{
			cout << "ERROR: Can't pop element <stack is empty> " << endl;
			system("pause");
			exit(1);
		}
		else
		{
			T cellData = HEAD->DATA;
			cell *tmp = HEAD;
			HEAD = HEAD->NEXT;
			delete tmp;
			return cellData;
		}
	}
	T& peek()
	{
		if (!HEAD)
		{
			cout << "ERROR: Can't peek element <stack is empty> " << endl;
			system("pause");
			exit(1);
		}
		else
			return HEAD->DATA;
	}
	~cStack()
	{
		cell *tmp = HEAD;
		while (tmp)
		{
			cell *tmp2 = tmp;
			tmp = tmp->NEXT;
			delete tmp2;
		}
	}
};