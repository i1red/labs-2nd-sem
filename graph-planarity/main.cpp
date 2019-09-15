#include <iostream>
#include "dinStruct.h"
using namespace std;

struct subgraph;

class graph {
	cList<int> *vrtx; 
	size_t vn;
	int *e3, *e4; 
	size_t v3, v4;
public:
	graph(int*, size_t);
	void print();
	void showE3();
	void showE4();
	bool planar();
	~graph();
private:
	bool fK5(int, int, int, int, int); 
	bool fK33(int, int, int, int, int, int); 
	void sub(bool&, subgraph);
	void findpath(bool&, stack<subgraph>&, subgraph, int, cQueue<int>);


int main() {

	int mtrx[] =
	{ 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0,
0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0,
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1,
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0
	};
	graph sample(mtrx, 17);
	sample.print();
	cout << "degu3+: ";
	sample.showE3();
	cout << "degu4+: ";
	sample.showE4();

	cout << "\n==================================\n" << endl;
	bool check = sample.planar();
	cout << "RESULT: Graph is ";
	if (!check)
		cout << "NOT ";
	cout << "planar" << endl;
	cout << "\n==================================" << endl;
	system("pause");
	return 0;
}

//================subgraph=================

struct subgraph {
	int v[6];//âåðøèíè ï³äãðàôà, ùî ðîçãëÿäàºòüñÿ
	int st, dst;//âåðøèíè ì³æ ÿêèìè øóêàºìî øëÿõ
	bool *vstd;//â³äì³òêè äëÿ âåðøèí, ùî áóëè ïåðåãëÿíóò³ ó ðàìêàõ ïîøóêó îäíîãî øëÿõó(çàïîá³ãàííÿ öèêë³÷íîñò³)
	bool *lckd;//âåðøèíè, ùî íàëåæàòü ïåâíîìó ³ç øëÿõ³â ãðàôà, ùî áóëè çíàéäåí³(çàáîðîíà ïîâòîðíîãî âèêðèñòàííÿ äëÿ ïîøóêó íîâèõ øëÿõ³â)
	size_t cycle_count;//âèêîðèñòîâóºòñÿ ó ôóíêö³¿ block(ðàõóºìî ê³ëüê³ñòü ðàç³â âèêîðèñòàííÿ âåðøèíè v[st])
	size_t vs;//ê³ëüê³ñòü âåðøèí ãðàôà, ç ÿêîãî âèä³ëåíî ï³äãðàô
	size_t subvs;//ê³ëüê³ñòü âåðøèí ï³äãðàôà(äëÿ òîãî, ùîá â³äð³çíÿòè ÷è øóêàºìî Ê5, ÷è Ê33)
	subgraph(size_t vnum, int v0, int v1, int v2, int v3, int v4, int v5 = -1) {
		v[0] = v0, v[1] = v1, v[2] = v2, v[3] = v3, v[4] = v4, v[5] = v5;

		if (v5 == -1)
			subvs = 5, st = 0, dst = 1;
		else
			subvs = 6, st = 0, dst = 3;

		vstd = new bool[vs = vnum]();
		lckd = new bool[vs]();
		cycle_count = 0;
	}
	subgraph(const subgraph &other) {
		vs = other.vs;
		subvs = other.subvs;
		st = other.st;
		dst = other.dst;
		cycle_count = other.cycle_count;
		vstd = new bool[vs];
		lckd = new bool[vs];
		for (size_t i = 0; i < vs; i++)
		{
			vstd[i] = other.vstd[i];
			lckd[i] = other.lckd[i];
		}
		for (size_t i = 0; i < 6; i++)
			v[i] = other.v[i];
	}
	int start() {
		return v[st];
	}
	int dest() {
		return v[dst];
	}
	bool block(int curr) {
		if (curr == v[st])
		{
			if (cycle_count)
				return true;
			else
				cycle_count++;
		}
		else
		{
			for (size_t i = 0; i < subvs; i++)
			{
				if (i == st || i == dst)
					continue;
				if (curr == v[i])
					return true;
			}
		}
		return false;
	}
	void nextEdge() {
		if (subvs == 5)
		{
			if (++dst == 5)
				++st, dst = st + 1;
		}
		else
		{
			if (++dst == 6)
				++st, dst = 3;
		}
		cycle_count = 0;
	}
	~subgraph() {
		delete[] vstd;
		delete[] lckd;
	}

	void RESET_VISITED() {
		for (size_t i = 0; i < vs; i++)
			vstd[i] = false;
	}
};

//*****************************************

//=================graph===================

graph::graph(int *matrix, size_t msize) {
	vrtx = new cList<int>[vn = msize];
	cStack<int> edges3, edges4;
	for (size_t i = 0; i < msize; i++)
		for (size_t j = 0; j < msize; j++)
			if (matrix[j + i * msize] == 1)
			{
				vrtx[i].push(j);
				int sz = vrtx[i].size();
				if (sz == 3)
					edges3.push(i);
				else if (sz == 4)
					edges4.push(i);
			}
	if (v3 = edges3.size())
	{
		e3 = new int[v3];
		for (size_t i = v3 - 1; !edges3.is_empty(); i--)
			e3[i] = edges3.pop();
	}
	else
		e3 = nullptr;
	if (v4 = edges4.size())
	{
		e4 = new int[v4];
		for (size_t i = v4 - 1; !edges4.is_empty(); i--)
			e4[i] = edges4.pop();
	}
	else
		e4 = nullptr;
}

void graph::print() {
	for (size_t i = 0; i < vn; i++)
	{
		cout << i << " : ";
		cout << vrtx[i] << endl;
	}
}

void graph::showE3() {
	if (!e3)
	{
		cout << "0 vertices deg>=3" << endl;
		return;
	}
	for (size_t i = 0; i < v3 - 1; i++)
		cout << e3[i] << ", ";
	cout << e3[v3 - 1] << endl;
}

void graph::showE4() {
	if (!e4)
	{
		cout << "0 vertices deg>=4" << endl;
		return;
	}
	for (size_t i = 0; i < v4 - 1; i++)
		cout << e4[i] << ", ";
	cout << e4[v4 - 1] << endl;
}

bool graph::planar() {
	if (v4 >= 5)
	{
		for (size_t i = 0; i < v4; i++)
			for (size_t j = i + 1; j < v4; j++)
				for (size_t m = j + 1; m < v4; m++)
					for (size_t n = m + 1; n < v4; n++)
						for (size_t p = n + 1; p < v4; p++)
							if (fK5(e4[i], e4[j], e4[m], e4[n], e4[p])) {
								cout << "Found subgraph K5" << endl;
								cout << e4[i] << ", " << e4[j] << ", " << e4[m] << ", " << e4[n] << ", " << e4[p] << endl;
								return false;
							}
	}
	if (v3 >= 6)
	{
		for (size_t i = 0; i < v3; i++)
			for (size_t j = i + 1; j < v3; j++)
				for (size_t m = j + 1; m < v3; m++)
					for (size_t n = m + 1; n < v3; n++)
						for (size_t p = n + 1; p < v3; p++)
							for (size_t q = p + 1; q < v3; q++)
								if (fK33(e3[i], e3[j], e3[m], e3[n], e3[p], e3[q])) 
									return false;
	}
	return true;
}

bool graph::fK5(int v0, int v1, int v2, int v3, int v4)
{
	bool res = false;
	sub(res, subgraph(vn, v0, v1, v2, v3, v4));
	return res;
}

bool graph::fK33(int v0, int v1, int v2, int v3, int v4, int v5) {
	bool res = false;
	int v[] = { v0, v1, v2, v3, v4, v5 };
	for (size_t i = 1; i < 6; i++)
		for (size_t j = i + 1; j < 6; j++)
		{
			int tmp[6];
			for (size_t k = 0; k < 6; k++)
				tmp[k] = v[k];
			swap(tmp[1], tmp[i]);
			swap(tmp[2], tmp[j]);
			sub(res, subgraph(vn, tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5]));
			if (res)
			{
				cout << "Found subgraph K33" << endl;
				cout << tmp[0] << ", " << tmp[1] << ", " << tmp[2] << ", " << tmp[3] << ", " << tmp[4] << ", " << tmp[5] << endl;
				return true;
			}
		}
	return false;
}


void graph::sub(bool &res, subgraph grph) {
	if (grph.subvs == 5 && grph.st == 4)
		res = true;
	else if (grph.subvs == 6 && grph.st == 3)
		res = true;
	else
	{
		stack<subgraph> cont;
		bool flag = false;
		findpath(flag, cont, grph, grph.start(), *new cQueue<int>);
		if (!flag)
			return;
		else
		{
			while (cont.size())
			{
				sub(res, cont.top());
				cont.pop();
			}
		}
	}
}

void graph::findpath(bool &res, stack<subgraph> &container, subgraph grph, int curr, cQueue<int> path) {
	if (curr == grph.dest())
	{
		res = true;
		while (!path.is_empty())
			grph.lckd[path.pop()] = true;
		grph.RESET_VISITED();
		grph.nextEdge();
		container.push(grph);
	}
	else if (grph.vstd[curr] || grph.lckd[curr] || grph.block(curr))
		return;
	else
	{
		if (curr != grph.start())
		{
			grph.vstd[curr] = true;
			path.push(curr);
		}
		for (size_t i = 0; i < vrtx[curr].size(); i++)
			findpath(res, container, grph, vrtx[curr][i], path);
	}
}


graph::~graph() {
	delete[] vrtx;
	delete[] e3;
	delete[] e4;
}

//******************************************
