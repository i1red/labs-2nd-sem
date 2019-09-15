#include <iostream>
#include <utility>
using namespace std;

struct Node {
	int key;
	Node *lt, *rt;
	Node(int ky = 0, Node* left = nullptr, Node* right = nullptr) {
		key = ky, lt = left, rt = right;
	}
};

typedef Node* Nodeptr;
typedef pair<Nodeptr, Nodeptr> couple;

void print(Nodeptr tree) {
	if (tree) {
		print(tree->lt);
		cout << tree->key << " ";
		print(tree->rt);
	}
}

void insert(Nodeptr& curr, int val) {
	if (curr) {
		if (val < curr->key)
			insert(curr->lt, val);
		else if (val > curr->key)
			insert(curr->rt, val);
	}
	else
		curr = new Node(val);
}

Nodeptr moveRt(Nodeptr curr, int k) {
	while (curr->rt && curr->rt->key <= k)
		curr = curr->rt;
	return curr;
}
Nodeptr moveLt(Nodeptr curr, int k) {
	while (curr->lt && curr->lt->key > k)
		curr = curr->lt;
	return curr;
}

couple divide(Nodeptr root, int k) {
	if (!root)
		return couple(nullptr, nullptr);
	
	Nodeptr first, second, tmpF, tmpS;
	if (root->key <= k) {
		first = root, tmpF = moveRt(root, k);
		second = tmpS = tmpF->rt;
	}
	else {
		second = root, tmpS = moveLt(root, k);
		first = tmpF = tmpS->lt;
		if (tmpF) {
			tmpF = moveRt(tmpF, k);
			tmpS->lt = tmpF->rt;
			tmpS = tmpS->lt;
		}
	}
	while (tmpF && tmpS) {
		tmpS = moveLt(tmpS, k);
		tmpF->rt = tmpS->lt;
		tmpF = tmpF->rt;
		if(tmpF) {
			tmpF = moveRt(tmpF, k);
			tmpS->lt = tmpF->rt;
			tmpS = tmpS->lt;
		}
	}
	return couple(first, second);
}

int main() {

	Nodeptr tree = new Node(190);
	for (int i = 0; i < 75; ++i)
		insert(tree, rand() % 300);

	print(tree);
	cout << endl;
	couple x = divide(tree, 120);
	print(x.first);
	cout << endl;
	print(x.second);
	cout << endl;

	system("pause");
	return 0;
}
