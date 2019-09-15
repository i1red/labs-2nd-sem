#ifndef _AVLTREE_H
#define _AVLTREE_H
#include <iostream>
#include <cmath>

class avltree {
	typedef struct node {
		int key;
		short bFactor;
		node *left, *right;
		node(int, short, node*, node*);
		void set(int, short, node*, node*);
	} *nodeptr;
	struct relation {
		nodeptr dad;
		bool isRight;
		relation(nodeptr, bool);
	};
	struct subtreeHt {
		short hl, hr;
		subtreeHt(short);
	};

	nodeptr root;
public:
	avltree();
	void insert(int);
	void remove(int);
	void print(const char*);
private:
	void printNode(nodeptr, const char*);
	void insertKy(nodeptr&, int);
	void removeNode(nodeptr);
	void insertChangeBF(nodeptr, short);
	void removeChangeBF(nodeptr, short);
	//void insertFixer(nodeptr);
	//void removeFixer(nodeptr);
	bool fixBalance(nodeptr);
	bool rotateLeft(nodeptr);
	bool rotateRight(nodeptr);
	nodeptr fmin(nodeptr);
	const relation& ndPos(nodeptr);
	const subtreeHt& subHt(nodeptr, short);
};

#endif 
