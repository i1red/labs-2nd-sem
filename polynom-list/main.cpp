#include <iostream>
#include <Windows.h>
#include <cstdio>
#include "monom.h"
using namespace std;

typedef struct cList {
private:
	struct object
	{
		monom DATA;
		object *NEXT;
	}; 
	object *FIRST;
	int SIZE;
public:
	cList()
	{
		FIRST = nullptr;
		SIZE = 0;
	}
	cList(const cList &other)
	{
		if (other.SIZE == 0)
		{
			FIRST = nullptr;
			SIZE = 0;
		}
		else
		{
			SIZE = other.SIZE;
			FIRST = new object;
			object *tmp = FIRST, *otherTmp = other.FIRST;
			tmp->DATA = otherTmp->DATA;
			while (otherTmp->NEXT != nullptr)
			{
				otherTmp = otherTmp->NEXT;
				object *tmp2 = new object;
				tmp2->DATA = otherTmp->DATA;
				tmp->NEXT = tmp2;
				tmp = tmp->NEXT;
			}
			tmp->DATA = otherTmp->DATA;
			tmp->NEXT = nullptr;
		}
	}
	cList(int n)
	{
		if (n == 0)
		{
			FIRST = nullptr;
			SIZE = 0;
		}
		else
		{
			object *tmp = nullptr;
			for (int i = n; i > 1; i--)
			{
				object* tmp2 = new object; tmp2->NEXT = tmp;
				tmp = tmp2;
			}
			FIRST = new object;
			FIRST->NEXT = tmp;
			SIZE = n;
		}
	}
	void add(monom obj)
	{
		if (FIRST == nullptr)
		{
			FIRST = new object;
			FIRST->DATA = obj;
			FIRST->NEXT = nullptr;
		}
		else
		{
			object *tmp = FIRST;
			while (tmp->NEXT != nullptr)
			{
				tmp = tmp->NEXT;
			}
			tmp->NEXT = new object;
			tmp->NEXT->DATA = obj;
			tmp->NEXT->NEXT = nullptr;
		}
		SIZE++;
	}
	void del(int index)
	{
		if (index >= SIZE || index < 0)
		{
			cout << "ERROR: Out of range" << endl;
			system("pause");
			exit(1);
		}
		else if (index == 0)
		{
			object *tmp;
			tmp = FIRST->NEXT;
			FIRST->DATA = tmp->DATA;
			FIRST->NEXT = tmp->NEXT;
			delete tmp;
		}
		else
		{
			object *tmp, *prev;
			int i = 0;
			prev = FIRST;
			while (i != index - 1)
			{
				prev = prev->NEXT;
				i++;
			}
			tmp = prev->NEXT;
			prev->NEXT = tmp->NEXT;
			delete tmp;
		}
		SIZE--;
	}
	void input()
	{
		do
		{
			cout << "TO FINISH: type <Z degree> and then press SHIFT+ENTER" << endl;
			cout << "Your polynomial: ";
			print();
			monom tmp;
			tmp.inputMonom();
			add(tmp);
			system("cls");
		} while (GetKeyState(VK_LSHIFT) >= 0);
	}
	void print()
	{
		if (SIZE == 0)
		{
			cout << "0" << endl;
			return;
		}
		if (SIZE == 1 && FIRST->DATA.k == 0)
		{
			cout << "0" << endl;
			return;
		}
		object *tmp = FIRST;
		while (tmp != nullptr)
		{
			tmp->DATA.print();
			if (tmp->NEXT != nullptr && tmp->NEXT->DATA.k > 0)
				cout << " + ";
			tmp = tmp->NEXT;
		}
		cout << endl;
	}
	void get_file_data(const char *filepath)
	{
		FILE *fp;
		fopen_s(&fp, filepath, "r");
		if (feof(fp))
		{
			cout << "ERROR: Can't open file" << endl;
			system("pause");
			exit(1);
		}
		else
		{
			while (!feof(fp))
			{
				monom cont;
				fscanf_s(fp, "%lf%d%d%d", &cont.k, &cont.X, &cont.Y, &cont.Z);
				add(cont);
			}
			fclose(fp);
		}
	}
	void similar()
	{
		for (int i = 0; i < SIZE - 1; i++)
		{
			for (int j = i + 1; j < SIZE; j++)
			{
				if (operator[](i).DATA.ifAdd(operator[](j).DATA))
				{
					operator[](i).DATA = operator[](i).DATA + operator[](j).DATA;
					del(j);
					j--;
				}
			}
		}
	}
	double value(double xV, double yV, double zV)
	{
		object *tmp = FIRST;
		double sum = 0;
		while (tmp != nullptr)
		{
			sum+=tmp->DATA.value(xV, yV, zV);
			tmp = tmp->NEXT;
		}
		return sum;
	}
	cList derX()
	{
		cList res;
		object *tmp = FIRST;
		while (tmp != nullptr)
		{
			if (tmp->DATA.derX().k != 0)
			{
				res.add(tmp->DATA.derX());
			}
			tmp = tmp->NEXT;
		}
		res.similar();
		return res;
	}
	cList derY()
	{
		cList res;
		object *tmp = FIRST;
		while (tmp != nullptr)
		{
			if (tmp->DATA.derY().k != 0)
			{
				res.add(tmp->DATA.derY());
			}
			tmp = tmp->NEXT;
		}
		res.similar();
		return res;
	}
	cList derZ()
	{
		cList res;
		object *tmp = FIRST;
		while (tmp != nullptr)
		{
			if (tmp->DATA.derZ().k != 0)
			{
				res.add(tmp->DATA.derZ());
			}
			tmp = tmp->NEXT;
		}
		res.similar();
		return res;
	}
	cList operator*(cList &other)
	{
		cList res;
		if (SIZE == 0 && other.SIZE != 0)
		{
			res = other;
			return res;
		}
		else if (other.SIZE == 0 && SIZE != 0)
		{
			res = *this;
			return res;
		}
		else if (SIZE + other.SIZE == 0)
		{
			res.FIRST = nullptr;
			res.SIZE = 0;
			return res;
		}
		else
		{
			object *tmp = FIRST, *otherTmp = other.FIRST;
			while (tmp != nullptr)
			{
				while (otherTmp != nullptr)
				{
					res.add(tmp->DATA*otherTmp->DATA);
					otherTmp = otherTmp->NEXT;
				}
				tmp = tmp->NEXT;
				otherTmp = other.FIRST;
			}
			res.similar();
		}
		return res;
	}
	cList operator+(cList &other)
	{
		cList res(SIZE + other.SIZE);
		if (SIZE == 0 && other.SIZE != 0)
		{
			res = other;
			return res;
		}
		else if (other.SIZE == 0 && SIZE != 0)
		{
			res = *this;
			return res;
		}
		else if (SIZE  + other.SIZE == 0)
			return res;
		else
		{
			object *tmp = FIRST, *otherTmp = other.FIRST, *resTmp = res.FIRST;
			while (tmp != nullptr)
			{
				resTmp->DATA = tmp->DATA;
				resTmp = resTmp->NEXT;
				tmp = tmp->NEXT;
			}
			while (otherTmp != nullptr)
			{
				resTmp->DATA = otherTmp->DATA;
				resTmp = resTmp->NEXT;
				otherTmp = otherTmp->NEXT;
			}
			res.similar();
			return res;
		}
	}
	object &operator[](int index)
	{
		if (0 <= index && index < SIZE)
		{
			int i = 0;
			object *tmp = FIRST;
			while (i != index)
			{
				tmp = tmp->NEXT;
				i++;
			}
			return *tmp;
		}
		else
		{
			cout << "ERROR: Out of range" << endl;
			system("pause");
			exit(1);
		}
	}
	cList &operator=(cList &other)
	{
		if (other.SIZE == 0)
		{
			object *tmp = FIRST;
			while (tmp != nullptr)
			{
				object *tmp2 = tmp;
				tmp = tmp->NEXT;
				delete tmp2;
			}
			FIRST = nullptr;
			SIZE = 0;
		}
		if (SIZE > other.SIZE)
		{
			object *tmp = FIRST, *otherTmp = other.FIRST, *toDel = nullptr;
			for (int i = 0; i < other.SIZE; i++)
			{
				tmp->DATA = otherTmp->DATA;
				otherTmp = otherTmp->NEXT;
				if (otherTmp != nullptr)
					tmp = tmp->NEXT;
				else
				{
					toDel = tmp->NEXT;
					tmp->NEXT = nullptr;
				}
			}
			while (toDel != nullptr)
			{
				object *tmp2 = toDel;
				toDel = toDel->NEXT;
				delete tmp2;
			}
			SIZE = other.SIZE;
		}
		if (SIZE == other.SIZE)
		{
			object *tmp = FIRST, *otherTmp = other.FIRST;
			for (int i = 0; i < other.SIZE; i++)
			{
				tmp->DATA = otherTmp->DATA;
				tmp = tmp->NEXT, otherTmp = otherTmp->NEXT;
			}
		}
		if (SIZE < other.SIZE)
		{
			object *tmp = FIRST, *otherTmp = other.FIRST;
			for (int i = 0; i < SIZE; i++)
			{
				tmp->DATA = otherTmp->DATA;
				tmp = tmp->NEXT, otherTmp = otherTmp->NEXT;
			}
			while (otherTmp != nullptr)
			{
				this->add(otherTmp->DATA);
				otherTmp = otherTmp->NEXT;
			}
			SIZE = other.SIZE;
		}
		return *this;
	}
	~cList()
	{
		object *tmp = FIRST;
		while (tmp != nullptr)
		{
			object *tmp2 = tmp;
			tmp = tmp->NEXT;
			delete tmp2;
		}
	}
} polynom;

int main()
{
	polynom a, b;
	a.input();
	b.get_file_data("data.txt");
	cout << "------------ ++++++++++ -----------" << endl;
	cout << "A: ";
	a.print();
	cout << "Value(2.1, 3, 0.5): " << a.value(2.1, 3, 0.5) << endl;
	cout << "B: ";
	b.print();
	cout << "Value(2.1, 3, 0.5): " << b.value(2.1, 3, 0.5) << endl;
	polynom res1 = a + b;
	cout << "RESULT: ";
	res1.print();
	cout << "Value(2.1, 3, 0.5): " << res1.value(2.1, 3, 0.5) << endl;
	cout << "------------ ********** -----------" << endl;
	cout << "A: ";
	a.print();
	cout << "B: ";
	b.print();
	polynom res2 = a * b;
	cout << "RESULT: ";
	res2.print();
	cout << "Value(2.1, 3, 0.5): " << res2.value(2.1, 3, 0.5) << endl;
	cout << "------------ DERIVATIVE -----------" << endl;
	cout << "POLYNOMIAL: ";
	a.print();
	cout << "by X: ";
	a.derX().print();
	cout << "by Y: ";
	a.derY().print();
	cout << "by Z: ";
	a.derZ().print();

	system("pause");
	return 0;
}