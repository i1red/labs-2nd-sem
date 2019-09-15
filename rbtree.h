#ifndef RBTREE_H
#define RBTREE_H

#include <iostream>
#include <functional>
#include <stack>
#include <utility>

template <typename T1, typename T2>
class rbtree {
	enum colors : bool { black, red };
	typedef struct node {
		T1 key;
		T2 data;
		bool color;
		node *left, *right;
		node(T1 ky, T2 dt, bool dye = red, node *l = nullptr, node *r = nullptr);
		void copy(const node*);
	} *nodeptr;
	nodeptr root;
	size_t bh;
public:
	rbtree();
	size_t treebh();
	void insert(T1, T2);
	T2* search(T1);
	bool remove(T1);
	void for_every(std::function<void(const T1&, T2&)>);
	void print(const char*);
	~rbtree();
private:
	void printer(nodeptr, const char*);
	void destruct(nodeptr&);

	void insertfixer(nodeptr, nodeptr);
	void left_rotation(nodeptr);
	void right_rotation(nodeptr);

	void removenode(nodeptr, nodeptr);
	void removefixer(nodeptr, nodeptr);


	bool isred(nodeptr);
	bool isright(nodeptr, nodeptr);
	void paintnode(nodeptr, bool);

	nodeptr fmin(nodeptr curr) {
		if (curr->left)
			fmin(curr->left);
		else
			return curr;
	}
	nodeptr father(nodeptr curr) {
		nodeptr tmp = root, dad = nullptr;
		T1 keyval = curr->key;
		while (tmp->key != keyval) {
			dad = tmp;
			if (tmp->key < keyval)
				tmp = tmp->right;
			else
				tmp = tmp->left;
		}
		return dad;
	}
	nodeptr brother(nodeptr curr, nodeptr dad) {
		if (!dad)
			return nullptr;
		else
			return dad->left == curr ? dad->right : dad->left;
	}
};
//сторення вузла дерева
template <typename T1, typename T2>
rbtree<T1, T2>::node::node(T1 ky, T2 dt, bool dye, node *l, node *r) {
	key = ky; data = dt; color = dye; left = l; right = r;
}

template <typename T1, typename T2>
void rbtree<T1, T2>::node::copy(const node *other) {
	key = other->key; data = other->data;  color = other->color; left = other->left; right = other->right;
}
//дерево
template <typename T1, typename T2>
rbtree<T1, T2>::rbtree() {
	root = nullptr; bh = 0;
}

template <typename T1, typename T2>
size_t rbtree<T1, T2>::treebh() {
	return bh;
}

template <typename T1, typename T2>
void rbtree<T1, T2>::print(const char *div) {
	if (root)
		printer(root, div);
	else
		std::cout << "Empty container" << std::endl;
}

template <typename T1, typename T2>
void rbtree<T1, T2>::insert(T1 ky, T2 dt) {
	if (!root) {
		root = new node(ky, dt, black); ++bh; return;
	}
	nodeptr tmp = root, dad = nullptr;
	while (true) {
		dad = tmp;
		if (ky < tmp->key) {
			if (!tmp->left) {
				tmp->left = new node(ky, dt); tmp = tmp->left; break;
			}
			tmp = tmp->left;
		}
		else if (ky == tmp->key)
			return;
		else {
			if (!tmp->right) {
				tmp->right = new node(ky, dt); tmp = tmp->right; break;
			}
			tmp = tmp->right;
		}
	}
	if (!dad || !isred(dad))
		return;
	else
		insertfixer(tmp, dad);

	if (isred(root))
		paintnode(root, black), ++bh;
}

template <typename T1, typename T2>
void rbtree<T1, T2>::for_every(std::function<void(const T1&, T2&)> work) {
	typedef std::pair<nodeptr, bool> couple;
	std::stack<couple> container;
	if (root)
		container.push(couple(root, 0));
	while (!container.empty()) {
		nodeptr curr = container.top().first;
		bool &state = container.top().second;
		switch (state) {
		case 0: {
			if (nodeptr next = curr->left)
				container.push(couple(next, 0));
			++state;
			break; }
		case 1: {
			work(curr->key, curr->data);
			container.pop();
			if (nodeptr next = curr->right)
				container.push(couple(next, 0));
			break; }
		}
	}
}

template <typename T1, typename T2>
T2* rbtree<T1, T2>::search(T1 ky) {
	T2 *res = nullptr;
	nodeptr tmp = root;
	while (tmp && tmp->key != ky) {
		if (tmp->key > ky)
			tmp = tmp->left;
		else
			tmp = tmp->right;
	}
	if (tmp)
		res = &tmp->data;

	return res;
}

template <typename T1, typename T2>
bool rbtree<T1, T2>::remove(T1 ky) {
	nodeptr tmp = root, dad = nullptr;
	while (tmp && tmp->key != ky) {
		dad = tmp;
		if (ky > tmp->key)
			tmp = tmp->right;
		else
			tmp = tmp->left;
	}
	if (tmp) {
		removenode(tmp, dad);
		return true;
	}
	return false;
}

template <typename T1, typename T2>
rbtree<T1, T2>::~rbtree() {
	destruct(root);
}

//допоміжні функції
template <typename T1, typename T2>
void rbtree<T1, T2>::printer(nodeptr curr, const char *div)
{
	if (curr) {
		printer(curr->left, div);
		std::cout << curr->key << " : " << curr->data << div;
		printer(curr->right, div);
	}
	else
		return;
}

template <typename T1, typename T2>
void rbtree<T1, T2>::destruct(nodeptr &curr) {
	if (curr) {
		destruct(curr->left);
		destruct(curr->right);
		delete curr;
		curr = nullptr;
	}
	else
		return;
}

//видалення вузла за покажчиками(на сам вузол, на батька)
template <typename T1, typename T2>
void rbtree<T1, T2>::removenode(nodeptr curr, nodeptr dad) {
	if (curr->left && curr->right) {
		nodeptr ndremove = fmin(curr->right);
		nodeptr ndremove_dad = father(ndremove);
		curr->key = ndremove->key;
		curr->data = ndremove->data;
		removenode(ndremove, ndremove_dad);
	}
	else if (curr->right) {
		curr->key = curr->right->key;
		curr->data = curr->right->data;
		delete curr->right;
		curr->right = nullptr;
	}
	else if (curr->left) {
		curr->key = curr->left->key;
		curr->data = curr->left->data;
		delete curr->left;
		curr->left = nullptr;
	}
	else {
		bool nd_is_red = isred(curr);
		if (dad) {
			bool nd_is_right = isright(curr, dad);
			delete curr;
			if (nd_is_right)
				dad->right = nullptr;
			else
				dad->left = nullptr;
			if (!nd_is_red)
				removefixer(nullptr, dad);
		}
		else {
			delete curr;
			root = nullptr;
			bh = 0;
		}
	}
}

//балансування дерева після вставки та видалення
template <typename T1, typename T2>
void rbtree<T1, T2>::insertfixer(nodeptr curr, nodeptr dad) {
	nodeptr grandp = father(dad);
	nodeptr uncle = brother(dad, grandp);
	if (isred(uncle)) {
		paintnode(dad, black);
		paintnode(uncle, black);
		paintnode(grandp, red);
		nodeptr pregran = father(grandp);
		if (isred(pregran))
			insertfixer(grandp, pregran);
	}
	else {
		if (isright(dad, grandp)) {
			if (isright(curr, dad))
				left_rotation(grandp);
			else {
				right_rotation(dad);
				left_rotation(grandp);
			}
			paintnode(grandp->left, red);
		}
		else {
			if (!isright(curr, dad))
				right_rotation(grandp);
			else {
				left_rotation(dad);
				right_rotation(grandp);
			}
			paintnode(grandp->right, red);
		}
		paintnode(grandp, black);
	}
}

template <typename T1, typename T2>
void rbtree<T1, T2>::removefixer(nodeptr dblack, nodeptr dad) {
	if (!dad) {
		--bh; return;
	}
	nodeptr sibling = brother(dblack, dad);
	bool sib_is_right = isright(sibling, dad);

	if (isred(dad)) {
		if (!isred(sibling->right) && !isred(sibling->left)) {
			paintnode(dad, black);
			paintnode(sibling, red);
			return;
		}

		if (sib_is_right) {
			if (!isred(sibling->right))
				right_rotation(sibling);
			left_rotation(dad);
		}
		else {
			if (!isred(sibling->left))
				left_rotation(sibling);
			right_rotation(dad);
		}
		paintnode(dad, red);
		paintnode(dad->right, black);
		paintnode(dad->left, black);
	}
	else {
		if (!isred(sibling)) {
			if (!isred(sibling->left) && !isred(sibling->right)) {
				paintnode(sibling, red);
				removefixer(dad, father(dad));
				return;
			}

			if (sib_is_right) {
				if (!isred(sibling->right))
					right_rotation(sibling);
				left_rotation(dad);
			}
			else {
				if (!isred(sibling->left))
					left_rotation(sibling);
				right_rotation(dad);
			}
			paintnode(dad, black);
			paintnode(dad->right, black);
			paintnode(dad->left, black);
		}
		else {
			if (sib_is_right) {
				nodeptr neph = sibling->left;
				if (!isred(neph->left) && !isred(neph->right)) {
					paintnode(sibling, black);
					paintnode(neph, red);
					left_rotation(dad);
					return;
				}

				if (isred(neph->right)) {
					paintnode(neph->right, black);
					right_rotation(sibling);
					left_rotation(dad);
				}
				else {
					paintnode(neph->left, black);
					right_rotation(neph);
					right_rotation(sibling);
					left_rotation(dad);
				}

			}
			else {
				nodeptr neph = sibling->right;
				if (!isred(neph->left) && !isred(neph->right)) {
					paintnode(sibling, black);
					paintnode(neph, red);
					right_rotation(dad);
					return;
				}

				if (isred(neph->left)) {
					paintnode(neph->left, black);
					left_rotation(sibling);
					right_rotation(dad);
				}
				else {
					paintnode(neph->right, black);
					left_rotation(neph);
					left_rotation(sibling);
					right_rotation(dad);
				}
			}
		}
	}
}

//повороти піддерева
template <typename T1, typename T2>
void rbtree<T1, T2>::left_rotation(nodeptr curr) {
	nodeptr nwleft = new node(curr->key, curr->data, curr->color, curr->left, curr->right->left);
	nodeptr nwright = curr->right->right;
	curr->key = curr->right->key;
	curr->data = curr->right->data;
	curr->color = curr->right->color;
	delete curr->right;
	curr->left = nwleft;
	curr->right = nwright;
}

template <typename T1, typename T2>
void rbtree<T1, T2>::right_rotation(nodeptr curr) {
	nodeptr nwleft = curr->left->left;
	nodeptr nwright = new node(curr->key, curr->data, curr->color, curr->left->right, curr->right);
	curr->key = curr->left->key;
	curr->data = curr->left->data;
	curr->color = curr->left->color;
	delete curr->left;
	curr->left = nwleft;
	curr->right = nwright;
}

//ф-ї для роботи з вузлом
template <typename T1, typename T2>
bool rbtree<T1, T2>::isred(nodeptr curr) {
	if (!curr)
		return black;
	else
		return curr->color;
}

template <typename T1, typename T2>
bool rbtree<T1, T2>::isright(nodeptr curr, nodeptr dad) {
	if (dad)
		return dad->right == curr ? true : false;
	else
		return false;
}

template <typename T1, typename T2>
void rbtree<T1, T2>::paintnode(nodeptr curr, bool dye) {
	if (curr)
		curr->color = dye;
}

#endif
