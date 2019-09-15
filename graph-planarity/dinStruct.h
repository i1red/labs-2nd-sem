#ifndef DINSTRUCT_H
#define DINSTRUCT_H

#include <iostream>
#include <stack>
using namespace std;

void ERROR(const char* log, int code)
{
    cout << "ERROR: " << log << endl;
    system("pause");
    exit(code);
}

template <typename T>
class cList {
    struct node {
        T DATA;
        node *NEXT;
        node();
        node(const node &other);
        node(T, node *set_next = nullptr);
        void copy(const node &other);
        void set(T, node *set_next = nullptr);
    };
    node *HEAD;
    node *TAIL;
    int SIZE;
public:
    cList();
    cList(const cList &);
    size_t size();
    void push(T);
    void insert(T, const int);
    void pop_index(const int);
    bool pop_key(const T);
    T& operator[](const int);
	cList& operator=(const cList&);
    ~cList();
	friend ostream &operator << <T>(ostream&, const cList&);

private:
	void output(ostream&);
};

template <typename T>
cList<T>::node::node()
{
    DATA = T(); NEXT = nullptr;
}

template <typename T>
cList<T>::node::node(const node &other)
{
    DATA = other.DATA; NEXT = nullptr;
}

template <typename T>
cList<T>::node::node(T set_data, node *set_next)
{
    DATA = set_data; NEXT = set_next;
}

template <typename T>
void cList<T>::node::copy(const node &other)
{
    DATA = other.DATA; NEXT = other.NEXT;
}

template <typename T>
void cList<T>::node::set(T set_data, node *set_next)
{
    DATA = set_data; NEXT = set_next;
}

template <typename T>
cList<T>::cList()
{
    HEAD = TAIL = nullptr;
    SIZE = 0;
}

template <typename T>
cList<T>::cList(const cList &other)
{
	HEAD = TAIL = nullptr;
	SIZE = 0;
    node *tmp = other.HEAD;
    while (tmp)
    {
        push(tmp->DATA);
        tmp = tmp->NEXT;
    }
}

template <typename T>
size_t cList<T>::size()
{
    return SIZE;
}

template <typename T>
void cList<T>::push(T push_data)
{
    if (!TAIL)
        HEAD = TAIL = new node(push_data);
    else
    {
		if (HEAD == TAIL) {
			HEAD->NEXT = new node(push_data);
			TAIL = HEAD->NEXT;
		}
		else {
			TAIL->NEXT = new node(push_data);
			TAIL = TAIL->NEXT;
		}
    }
    SIZE++;
}

template <typename T>
void cList<T>::insert(T insert_data, const int index)
{
    size_t smart_index;
    if (index < 0)
        smart_index = SIZE + index;
    else
        smart_index = index;
    if (smart_index == SIZE)
        push(insert_data);
    else if (smart_index == 0)
    {
        HEAD = new node(insert_data, HEAD);
        SIZE++;
    }
    else if (smart_index > 0 && smart_index < SIZE)
    {
        size_t counter = 0;
        node *tmp = HEAD;
        while (counter < smart_index - 1)
        {
            tmp = tmp->NEXT;
            counter++;
        }
        tmp->NEXT = new node(insert_data, tmp->NEXT);
        SIZE++;
    }
    else ERROR("Index is out of range", 1);

}

template <typename T>
ostream &operator << (ostream &os, const cList<T> &sample) {
	typedef cList<T>::node* nodeptr;
	nodeptr tmp = sample.HEAD;
	while (tmp) {
	os << tmp->DATA;
		if (tmp = tmp->NEXT)
			os << ", ";
	}
	return os;
}

template <typename T>
void cList<T>::pop_index(const int index)
{
    size_t smart_index;
    if (index < 0)
        smart_index = SIZE + index;
    else
        smart_index = index;
    if (smart_index == 0 && SIZE == 1)
    {
        delete HEAD;
        HEAD = TAIL = nullptr;
        SIZE--;
    }
    else if (smart_index >= 0 && smart_index < SIZE)
    {
        size_t counter = 0;
        node *tmp = HEAD, *ptr_to_del = nullptr;
        while (counter < smart_index - 1)
        {
            tmp = tmp->NEXT;
            counter++;
        }
        ptr_to_del = tmp->NEXT;
        tmp->NEXT = ptr_to_del->NEXT;
        if (ptr_to_del == TAIL)
            TAIL = tmp;
        delete ptr_to_del;
        ptr_to_del = nullptr;
        SIZE--;
    }
    else ERROR("Index is out of range", 1);
}

template <typename T>
bool cList<T>::pop_key(const T key_data)
{
    node *ptr_to_del = HEAD, *tmp = nullptr;
    while (ptr_to_del && ptr_to_del->DATA != key_data)
    {
        tmp = ptr_to_del;
        ptr_to_del = ptr_to_del->NEXT;
    }
    if (!ptr_to_del)
        return false;
    else
    {
        if (tmp)
        {
            tmp->NEXT = ptr_to_del->NEXT;
            if (ptr_to_del == TAIL)
                TAIL = tmp;
            delete ptr_to_del;
            ptr_to_del = nullptr;
        }
        else
        {
            delete HEAD;
            HEAD = TAIL = nullptr;
        }
        SIZE--;
        return true;
    }
}

template <typename T>
T& cList<T>::operator[](const int index)
{
    size_t smart_index;
    if (index < 0)
        smart_index = SIZE + index;
    else
        smart_index = index;
    if (smart_index >= 0 && smart_index < SIZE)
    {
        size_t counter = 0;
        node *tmp = HEAD;
        while (counter < smart_index)
        {
            tmp = tmp->NEXT;
            counter++;
        }
        return tmp->DATA;
    }
    else ERROR("Index is out of range", 1);
}

template <typename T>
cList<T>& cList<T>::operator=(const cList<T> &other) {
	node *tmp = HEAD, *tmpother = other.HEAD;
	if (SIZE >= other.SIZE) {
		node *tmplast = nullptr;
		while (tmpother) {
			tmp->DATA = tmpother->DATA;
			tmplast = tmp;
			tmp = tmp->NEXT, tmpother = tmpother->NEXT;
		}
		if (tmplast)
			tmplast->NEXT = nullptr;
		while (tmp) {
			node *tmp2 = tmp;
			tmp = tmp->NEXT;
			delete tmp2;
		}
	}
	else {
		while (tmp) {
			tmp->DATA = tmpother->DATA;
			tmp = tmp->NEXT, tmpother = tmpother->NEXT;
		}
		while (tmpother) {
			push(tmpother->DATA);
			tmpother = tmpother->NEXT;
		}
	}
	SIZE = other.SIZE;
	return *this;
}

template <typename T>
cList<T>::~cList() {
    node *tmp = HEAD;
    while (tmp) {
        node *tmp2 = tmp;
        tmp = tmp->NEXT;
        delete tmp2;
    }

}

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
    cQueue(const cQueue& other)
    {
        HEAD = TAIL = nullptr;
        queue_obj *tmp = other.HEAD;
        while (tmp)
        {
            push(tmp->DATA);
            tmp = tmp->NEXT;
        }
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

template <typename T>
struct cStack {
private:
    struct cell {
        T DATA;
        cell *NEXT;
    };
    cell *HEAD;
    int SIZE;
public:
    cStack()
    {
        HEAD = nullptr;
        SIZE = 0;
    }
    cStack(const cStack &other)
    {
        HEAD = nullptr;
        SIZE = 0;
        cell *tmp = other.HEAD;
        while (tmp)
        {
            push(tmp->DATA);
            tmp = tmp->NEXT;
        }
    }
    int size()
    {
        return SIZE;
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
        SIZE++;
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
            SIZE--;
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
#endif 
