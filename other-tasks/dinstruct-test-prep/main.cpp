#include <iostream>
#include <stack>
#include <utility>
#include <set>
#include <initializer_list>
#include <stack>
#include <queue>
using namespace std;

short MAX(short a, short b) { 
	return a > b ? a : b; 
}

typedef struct Node {
	int key;
	Node *lt, *rt;
	Node(int ky = 0, Node* nl = nullptr, Node* nr = nullptr) {
		key = ky, lt = nl, rt = nr;
	}
} *Nodeptr;

typedef pair<Nodeptr, bool> couple;
typedef char Tiny;
short height(Nodeptr curr) {
	if (!curr)
		return 0;
	else
		return MAX(height(curr->lt), height(curr->rt)) + 1;
}

bool checkAVL(Nodeptr root) {
	short lh = height(root->lt), rh = height(root->rt);
	
	if (abs(rh - lh) > 1)
		return false;
	if (root->lt && root->lt->key >= root->key)
		return false;
	if (root->rt && root->rt->key <= root->key)
		return false;
	
	return true;
}

bool balanceCheck(Nodeptr root) {
	stack<couple> container;
	if (root)
		container.push(couple(root, 0));
	else
		return true;
	while (container.size()) {
		Nodeptr& nd = container.top().first;
		bool& state = container.top().second;
		if (state++ == 0) {
			if (nd->lt)
				container.push(couple(nd->lt, 0));
		}
		else {
			if (!checkAVL(nd))
				return false;
			else if(nd->rt)
				container.push(couple(nd->rt, 0));
			container.pop();
		}
	}
	return true;
}

class GraphList {
	set<int>* vertex;
	int size;
public:
	GraphList(const initializer_list<initializer_list<int>>& init) {
		vertex = new set<int>[size = init.size()];
		int it = 0;
		for (const auto &item : init)
			vertex[it++] = item;
	}
	void print() {
		for(int i =0; i < size; ++i) { 
			cout << i << " : ";
			for (const auto &item : vertex[i])
				cout << item << " ";
			cout << endl;
		}
	}
	friend void bfs(const GraphList&, int);
};

typedef pair<int, int> two;

void bfs(const GraphList& g, int st) {
	bool* vstd = new bool[g.size]();
	vstd[st] = true;
	queue<two> container;
	container.push(two(st, 0));
	two front;
	while (!container.empty()) {
		front = container.front();
		cout << front.first << " : " << front.second << endl;
		container.pop();
		for (const auto &v : g.vertex[front.first]) {
			if(!vstd[v]) {
				container.push(two(v, front.second + 1));
				vstd[v] = true;
			}
		}
	}
}

class GraphMatrix {
	bool** matrix;
	int size;
public:
	GraphMatrix(const initializer_list<bool>& init) {
		size = sqrt(init.size());
		matrix = new bool*[size];
		for (int i = 0; i < size; ++i)
			matrix[i] = new bool[size];
		auto it = init.begin();
		for (int i = 0; i < size; ++i)
			for (int j = 0; j < size; ++j)
				matrix[i][j] = *it++;
	}
	void print() {
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j)
				cout << matrix[i][j] << " ";
			cout << endl;
		}
	}
	friend bool acyclic(const GraphMatrix&);
	
};


enum State : char {N, CURR, VSTD};

bool acyclic(const GraphMatrix& g) {
	int gs = g.size;
	bool* vstd = new bool[gs]();
	stack<int> container;
	for (int i = 0; i < gs; ++i) {
		for (int j = 0; j < gs; ++j) {
			if (g.matrix[i][j]) {
				for (int c = 0; c < gs; ++c)
					if (g.matrix[j][c] && c != i)
						container.push(c);
				int top;
				while (container.size()) {
					top = container.top();
					if (!g.matrix[i][top]) {
						container.pop();
						vstd[top] = true;
						for (int c = 0; c < gs; ++c)
							if (g.matrix[top][c] && c != j && !vstd[c])
								container.push(c);
					}
					else
						return false;
				}
				for (int c = 0; c < gs; ++c)
					vstd[c] = false;
			}
		 }

	}
	return true;
}




int main() {
	GraphList nw = { 
	{1, 3},
	{0, 3, 4},
	{0, 1, 3, 4},
	{0, 1, 4},
	{0, 1, 2, 3} };

	GraphMatrix nk = {
	0, 0, 0, 0, 0, 0, 0,
0, 0, 1, 1, 0, 0, 0,
0, 1, 0, 0, 0, 0, 0,
0, 1, 0, 0, 0, 0, 1,
0, 0, 0, 0, 0, 0, 1,
0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 1, 1, 0, 0
	};

	cout << acyclic(nk) << endl;
	bfs(nw, 0);
	system("pause");
	return 0;
}