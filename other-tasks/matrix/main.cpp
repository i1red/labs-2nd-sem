#include <iostream>
using namespace std;

void ERROR(const char *log)
{
	cout << "ERROR: " << log << endl;
	system("pause");
	exit(1);
}

class matrix {
	struct node {
		int DATA;
		int I; int J;
		node *RIGHT; node *DOWN;
		node(int set_data, int set_i, int set_j)
		{
			DATA = set_data; I = set_i; J = set_j; RIGHT = DOWN = nullptr;
		}
	};
	class jList {
		node *HEAD;
		node *TAIL;
		int SIZE;
	public:
		jList()
		{
			HEAD = TAIL = nullptr;
			SIZE = 0;
		}
		void push_node(node *data)
		{
			if (!data)
				SIZE++;
			else if (!HEAD)
			{
				HEAD = TAIL = data;
				SIZE++;
			}
			else
			{
				TAIL->RIGHT = data;
				TAIL = TAIL->RIGHT;
				SIZE++;
			}
		}
		int at(const int index)
		{
			if (0 <= index && index < SIZE)
			{
				node *tmp = HEAD;
				while (tmp && tmp->I <= index)
				{
					if (tmp->I == index)
						return tmp->DATA;
					else
						tmp = tmp->RIGHT;
				}
				return 0;
			}
			else
				ERROR("jList index is out of range");
		}
		void print()
		{
			if (!HEAD)
				for (int i = 0; i < SIZE; i++)
					cout << 0 << " ";
			else
			{
				for (int i = 0; i < HEAD->I; i++)
					cout << 0 << " ";
				cout << HEAD->DATA << " ";
				node *tmp = HEAD->RIGHT;
				int lastIndex = HEAD->I;
				while (tmp)
				{
					for (int i = lastIndex + 1; i < tmp->I; i++)
						cout << 0 << " ";
					cout << tmp->DATA << " ";
					lastIndex = tmp->I;
					tmp = tmp->RIGHT;
				}
				for (int i = lastIndex + 1; i < SIZE; i++)
					cout << 0 << " ";
			}
		}
		~jList()
		{
			node *tmp = HEAD;
			while (tmp)
			{
				node *tmp_del = tmp;
				tmp = tmp->RIGHT;
				delete tmp_del;
			}
		}

	};
	class iList
	{
		node *HEAD;
		node *TAIL;
		int SIZE;
	public:
		iList()
		{
			HEAD = TAIL = nullptr;
			SIZE = 0;
		}
		void push_node(node *data)
		{
			if (!data)
				SIZE++;
			else if (!HEAD)
			{
				HEAD = TAIL = data;
				SIZE++;
			}
			else
			{
				TAIL->DOWN = data;
				TAIL = TAIL->DOWN;
				SIZE++;
			}
		}
		int at(const int index)
		{
			if (0 <= index && index < SIZE)
			{
				node *tmp = HEAD;
				while (tmp && tmp->J <= index)
				{
					if (tmp->J == index)
						return tmp->DATA;
					else
						tmp = tmp->DOWN;
				}
				return 0;
			}
			else
				ERROR("iList index is out of range");
		}
		~iList()
		{
			node *tmp = HEAD;
			while (tmp)
			{
				node *tmp_del = tmp;
				tmp = tmp->DOWN;
				delete tmp_del;
			}
		}
	};
	iList *HOR;
	jList *VER;
	int nI; int nJ;
	int M; int N;
public:
	matrix(int set_m, int set_n)
	{
		HOR = new iList[M = set_m];
		VER = new jList[N = set_n];
		nI = nJ = 0;
	}
	matrix(const matrix &other)
	{
		HOR = new iList[M = other.M];
		VER = new jList[N = other.N];
		nI = nJ = 0;
		for (int i = 0; i < other.N; i++)
			for (int j = 0; j < other.M; j++)
				push(other.VER[i].at(j));
	}
	void push(int data)
	{
		if (data == 0)
			HOR[nI].push_node(nullptr), VER[nJ].push_node(nullptr), moveIndex();
		else
		{
			node *tmp = new node(data, nI, nJ);
			HOR[nI].push_node(tmp), VER[nJ].push_node(tmp), moveIndex();
		}
	}
	void print()
	{
		for (int j = 0; j < N; j++)
			VER[j].print(), cout << endl;
	}
	matrix operator*(const matrix other)
	{
		if (M == other.N)
		{
			matrix res(N, other.M);
			for (int j = 0; j < N; j++)
			{
				for (int i = 0; i < other.M; i++)
				{
					int tmp = 0;
					for (int k = 0; k < M; k++)
					{
						tmp += VER[j].at(k)*other.HOR[i].at(k);
					}
					res.push(tmp);
				}
			}
			return res;
		}
		else
			ERROR("Matrices can't be mltiplied");
	}
private:
	void moveIndex()
	{
		if (nJ == N)
			ERROR("Matrix is full");
		else if (++nI == M)
			nJ++, nI = 0;
	}
};

int main()
{
	matrix test(3, 4);
	for (int i = 1; i < 7; i++)
		test.push(i), test.push(0);

	matrix test2(4, 3);
	for (int i = 7; i < 13; i++)
		test2.push(i), test2.push(0);

	matrix res2 = test * test2;
	
	test.print();
	cout << endl;
	test2.print();
	cout << endl;
	res2.print();
	
	cout << endl;
	system("pause");
	return 0;
}