#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "rbtree.h"
using namespace std;

template <typename T>
ostream &operator <<(ostream &out, const list<T> &sample) {
	for (const auto &item : sample) {
		out << item << " ";
	}
	return out;
}

bool is_ident(const string &sample) {
	if (isdigit(sample[0]))
		return false;
	return true;
}

bool is_div(const char sample) {
	string divs = "\t !(){}[]<>|&?:;.,+-*/%^=#";
	for (const char &item : divs)
		if (sample == item)
			return true;
	
	return false;
}

bool nw_pos(const string &sample, size_t &pos) {
	while (pos < sample.length()) {
		char curr = sample[pos];
		if (is_div(curr))
			++pos;
		else if (curr == '\"' || curr == '\'') {
			auto at = sample.find(curr, pos + 1);
			if (at != -1)
				pos = at + 1;
			else
				return false;
		}
		else
			return true;
	}
	return false;
}

size_t nw_len(const string &sample, int pos) {
	size_t len = 0;
	while (pos + len < sample.length()) {
		char curr = sample[pos + len];
		if (!is_div(curr) && curr != '\"' && curr != '\'')
			++len;
		else
			return len;
	}
}

void line_analyzer(size_t line, const string &buf, rbtree<string, list<size_t>> &sample) {
	string curr_name;
	size_t pos = 0, len = 0;
	while (nw_pos(buf, pos)) {
		len = nw_len(buf, pos);
		curr_name = buf.substr(pos, len);
		if (is_ident(curr_name)) {
			if (auto lst = sample.search(curr_name))
				lst->push_back(line);
			else
				sample.insert(curr_name, { line });
		}
		pos += len + 1;
	}
}

void readprogram(const char *filepath, rbtree<string, list<size_t>> &sample) {
	ifstream fin;
	try {
		fin.open(filepath);
		string buffer;
		size_t line = 1;
		while (!fin.eof()) {
			getline(fin, buffer);
			line_analyzer(line, buffer, sample);
			++line;
		}
		fin.close();
	}
	catch (const ifstream::failure &excpt) {
		cout << excpt.what() << endl;
		cout << excpt.code() << endl;
	}
}

int main() {
	rbtree<string, list<size_t>> table;
	readprogram("main.cpp", table);
	ofstream fout("ident_table.txt");
	table.for_every([&fout](const string &ky, list<size_t> &dt) {fout << ky << " : " << dt << "\n"; });
	fout.close();

	system("pause");
	return 0;
}
