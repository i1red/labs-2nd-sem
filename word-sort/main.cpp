#include <iostream>
#include <string>
#include <fstream>
#include "cQueue.h"
using namespace std;

void w_sort(string datafile, const int w_size)
{
	cQueue<string> *words = new cQueue<string>[w_size];
	int max = 0;
	ifstream data(datafile);
	if (!data.is_open())
	{
		cout << "ERROR: Can't open file: " << datafile << endl;
		system("pause");
		exit(1);
	}
	else
	{
		while (!data.eof())
		{
			string get_data;
			getline(data, get_data);
			int index = get_data.size() - 1;
			if (index > max)
				max = index;
			words[index].push(get_data);
		}
		data.close();
	}
	const int alph_s = 26;
	cQueue<string> scoops[alph_s];
	int opcount = 0;
	for (int k = max; k >= 0; k--)
	{
		while (!words[k].is_empty())
		{
			string tmp = words[k].pop();
			scoops[tmp[k] - 'a'].push(tmp);
		}
		int f;
		if (k > 0)
			f = k - 1;
		else
			f = k;
		for (int i = 0; i < alph_s; i++)
		{
			while (!scoops[i].is_empty())
			{
				words[f].push(scoops[i].pop());
			}
		}
	}
	string result = "sorted_" + datafile;
	ofstream data_res(result);
	if (!data_res.is_open())
	{
		cout << "ERROR: Can't create result file" << endl;
		system("pause");
		exit(1);
	}
	else
	{
		while (!words[0].is_empty())
			data_res << words[0].pop() + "\n";
		cout << "Sorted successfully. Result in file: " << result << endl;
	}
	delete[] words;
}

int main()
{
	w_sort("words.txt", 30);

	system("pause");
	return 0;
}
