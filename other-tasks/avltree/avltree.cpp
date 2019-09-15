#include "avltree.h"

template<typename T1, typename T2>
constexpr auto MAX(T1 a, T2 b) { return (a > b ? a : b); }

avltree::node::node(int ky = 0, short bf = 0, node* lt = nullptr, node* rt = nullptr) {
	key = ky, bFactor = bf, left = lt, right = rt;
}

void avltree::node::set(int ky, short bf, node* lt, node* rt) {
	key = ky, bFactor = bf, left = lt, right = rt;
}

avltree::relation::relation(nodeptr dd = nullptr, bool rt = false) {
	dad = dd, isRight = rt;
}

avltree::subtreeHt::subtreeHt(short hf = 0) {
	hl = hf - 1, hr = hf - 1;
}

avltree::avltree() {
	root = nullptr;
}

void avltree::insert(int ky) {
	insertKy(root, ky);
}

void avltree::remove(int ky) {
	nodeptr tmp = root, res = nullptr;
	while (tmp) {
		if (tmp->key == ky) {
			res = tmp;
			break;
		}
		else if (tmp->key < ky)
			tmp = tmp->right;
		else
			tmp = tmp->left;
	}
	if (res)
		removeNode(res);
}

void avltree::print(const char* div) {
	printNode(root, div);
}

void avltree::printNode(nodeptr curr, const char* div) {
	if (curr) {
		std::cout << curr->key << div;
		printNode(curr->left, div);
		printNode(curr->right, div);
	}
}

void avltree::insertKy(nodeptr& curr, int ky) {
	if (!curr) {
		curr = new node(ky);
		relation ndInfo = ndPos(curr);
		if (ndInfo.dad) {
			if (ndInfo.isRight)
				insertChangeBF(ndInfo.dad, 1);
			else
				insertChangeBF(ndInfo.dad, -1);
		}
	}
	else {
		if (ky < curr->key)
			insertKy(curr->left, ky);
		else if (ky > curr->key)
			insertKy(curr->right, ky);
	}
}

void avltree::removeNode(nodeptr curr) {
	if (!curr->left && !curr->right) {
		relation ndInfo = ndPos(curr);
		if (ndInfo.dad) {
			if (ndInfo.isRight) {
				delete ndInfo.dad->right;
				ndInfo.dad->right = nullptr;
				removeChangeBF(ndInfo.dad, -1);
			}
			else {
				delete ndInfo.dad->left;
				ndInfo.dad->left = nullptr;
				removeChangeBF(ndInfo.dad, 1);
			}
		}
		else
			root = nullptr;
	}
	else if (!curr->right) {
		curr->key = curr->left->key;
		delete curr->left;
		curr->left = nullptr;
		removeChangeBF(curr, 1);
	}
	else if (!curr->left) {
		curr->key = curr->right->key;
		delete curr->right;
		curr->right = nullptr;
		removeChangeBF(curr, -1);
	}
	else {
		nodeptr delNode = fmin(curr->right);
		int ky = delNode->key;
		removeNode(delNode);
		curr->key = ky;
	}
}

void avltree::insertChangeBF(nodeptr curr, short val) {
	curr->bFactor += val;
	if (abs(curr->bFactor) == 2)
		fixBalance(curr);
	else if (abs(curr->bFactor) == 1) {
		relation ndInfo = ndPos(curr);
		short changeVal = -1;
		if (ndInfo.dad) {
			if (ndInfo.isRight)
				changeVal = 1;
			insertChangeBF(ndInfo.dad, changeVal);
		}
			
	}
}

void avltree::removeChangeBF(nodeptr curr, short val) {
	curr->bFactor += val;
	short changeVal = 1;
	relation ndInfo = ndPos(curr);
	if (abs(curr->bFactor) == 0) {
		if (ndInfo.dad) {
			if (ndInfo.isRight)
				changeVal = -1;
			removeChangeBF(ndInfo.dad, changeVal);
		}
	}
	else if (abs(curr->bFactor) == 2) {
		if (!fixBalance(curr)) {
			if (ndInfo.dad) {
				if (ndInfo.isRight)
					changeVal = -1;
				removeChangeBF(ndInfo.dad, changeVal);
			}
		}
	}
}

bool avltree::fixBalance(nodeptr curr) {
	bool res = true;
	if (curr->bFactor == 2) {
		if (curr->right->bFactor < 0)
			rotateRight(curr->right);
		return rotateLeft(curr);
	}
	else {
		if (curr->left->bFactor > 0)
			rotateLeft(curr->left);
		return rotateRight(curr);
	}
}

bool avltree::rotateLeft(nodeptr curr) {
	subtreeHt currNd = subHt(curr, 0), rtNd = subHt(curr->right, currNd.hr);
	//Calculating new node heights/balance factors
	short nwLeftBF, nwLeftHt, nwDadBF, nwDadHt;
	nwLeftBF = rtNd.hl - currNd.hl, nwLeftHt = MAX(rtNd.hl, currNd.hl) + 1;
	nwDadBF = rtNd.hr - nwLeftHt, nwDadHt = MAX(rtNd.hr, nwLeftHt) + 1;
	//Defining new node relations
	nodeptr rt = curr->right, nwLeft, nwRight;
	nwRight = rt->right;
	nwLeft = new node(curr->key, nwLeftBF, curr->left, rt->left);
	//Setting difined relations
	curr->set(rt->key, nwDadBF, nwLeft, nwRight);
	delete rt;
	//If node height hasn't decreased returns true
	return nwDadHt == 0 ? 1 : 0;
}

bool avltree::rotateRight(nodeptr curr) {
	subtreeHt currNd = subHt(curr, 0), ltNd = subHt(curr->left, currNd.hl);
	//Calculating new tree heights/balance factors
	short nwRightBF, nwRightHt, nwDadBF, nwDadHt;
	nwRightBF = currNd.hr - ltNd.hr, nwRightHt = MAX(currNd.hr, ltNd.hr) + 1;
	nwDadBF = nwRightHt - ltNd.hl, nwDadHt = MAX(nwRightHt, ltNd.hl) + 1;
	//Defining new node relations
	nodeptr lt = curr->left, nwLeft, nwRight;
	nwLeft = lt->left;
	nwRight = new node(curr->key, nwRightBF, lt->right, curr->right);
	//Setting difined relations
	curr->set(lt->key, nwDadBF, nwLeft, nwRight);
	delete lt;
	//If node height hasn't decreased returns true
	return nwDadHt == 0 ? 1 : 0;
}

avltree::nodeptr avltree::fmin(nodeptr curr) {
	if (curr->left)
		fmin(curr->left);
	else
		return curr;
}

const avltree::relation& avltree::ndPos(nodeptr curr) {
	relation ndInfo;
	nodeptr tmp = root;
	int ky = curr->key;
	while (tmp) {
		if (ky > tmp->key) {
			ndInfo.dad = tmp;
			ndInfo.isRight = true;
			tmp = tmp->right;
		}
		else if (ky < tmp->key) {
			ndInfo.dad = tmp;
			ndInfo.isRight = false;
			tmp = tmp->left;
		}
		else
			break;
	}
	return ndInfo;
}

const avltree::subtreeHt& avltree::subHt(nodeptr curr, short hf) {
	subtreeHt ndInfo(hf);
	if (curr->bFactor > 0)
		ndInfo.hl -= curr->bFactor;
	else
		ndInfo.hr += curr->bFactor;
	return ndInfo;
}
