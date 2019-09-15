#pragma once

#include <iostream>
using namespace std;

template <typename T>
class cQueue {
	struct queue_obj {
		T DATA;
		queue_obj *NEXT;
	};
	queue_obj *HEAD;
	queue_obj *TAIL;
public:
	cQueue()
	{
		HEAD = TAIL = nullptr;
	}
	void push(T data_obj)
	{
		queue_obj *tmp = new queue_obj;
		tmp->DATA = data_obj;
		tmp->NEXT = nullptr;
		if (!HEAD)
			HEAD = TAIL = tmp;
		else
		{
			TAIL->NEXT = tmp;
			TAIL = TAIL->NEXT;
		}
	}
	bool is_empty()
	{
		if (!HEAD)
			return true;
		else
			return false;
	}
	T pop()
	{
		if (!HEAD)
		{
			cout << "ERROR: Can't pop <empty queue>" << endl;
			system("pause");
			exit(1);
		}
		else
		{
			queue_obj *tmp = HEAD;
			T res = HEAD->DATA;
			HEAD = HEAD->NEXT;
			delete tmp;
			return res;
		}
	}
	T peek()
	{
		if (!HEAD)
		{
			cout << "ERROR: Can't peek <empty queue>" << endl;
			system("pause");
			exit(1);
		}
		else
			return HEAD->DATA;
	}
	~cQueue()
	{
		queue_obj *tmp = HEAD;
		while (tmp)
		{
			queue_obj *tmp2 = tmp;
			tmp = tmp->NEXT;
			delete tmp2;
		}
	}
};