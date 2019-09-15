#include <iostream>
#include <cstdio>
using namespace std;

int file_sort(const char *filepath);

void divide(FILE *fp, const char *filepath, int &errors);

void merge(FILE *res, FILE *a, FILE *b, int &errors);

void file_copy(const char *filepath);

int main()
{
	file_copy("testSort.txt");
	if (int number_of_errors = file_sort("testSort.txt") != 0)
	{
		cout << "Number of errors occured: " << number_of_errors << endl;
		cout << "Data might be damaged. The copy is saved" << endl;
	}
	else
	{
		char choice;
		cout << "Data has been sorted successfully. Enter 'e' to delete the copy/not 'e' to save it" << endl;
		cin >> choice;
		if (choice == 'e')
			remove("copy_testSort.txt");
	}

	system("pause");
	return 0;
}

int file_sort(const char *filepath)
{
	FILE *src;
	fopen_s(&src, filepath, "r+");
	if (src == NULL)
	{
		cout << "Can't open file: " << filepath << endl;
		system("pause");
		exit(1);
	}
	else
	{
		int err_occurred = 0;
		divide(src, filepath, err_occurred);
		fclose(src);
		return err_occurred;
	}
}

void divide(FILE *src, const char *filepath, int &errors)
{
	FILE *p1, *p2, *tmp;
	fopen_s(&p1, "p1.bin", "w+b");
	fopen_s(&p2, "p2.bin", "w+b");
	tmp = p1;

	int key, prev = -1;
	bool endSort = true;
	do
	{
		if (fscanf_s(src, "%d ", &key) != 1)
			errors++;
		if (key < prev)
		{
			if (tmp == p1)
				tmp = p2;
			else
				tmp = p1;
			endSort = false;
		}
		if (fwrite(&key, sizeof(int), 1, tmp) != 1)
			errors++;
		prev = key;
	} while (!feof(src));

	if (endSort)
	{
		fclose(p1);
		fclose(p2);
		remove("p1.bin");
		remove("p2.bin");
		return;
	}
	else
	{
		fclose(src);
		fopen_s(&src, filepath, "w+");

		merge(src, p1, p2, errors);

		fclose(p1);
		fclose(p2);
		rewind(src);
		divide(src, filepath, errors);
	}
}

void merge(FILE *res, FILE *a, FILE *b, int &errors)
{
	rewind(a);
	rewind(b);

	int key_a, key_b;
	if (fread(&key_a, sizeof(int), 1, a) != 1 && !feof(a))
		errors++;
	if (fread(&key_b, sizeof(int), 1, b) != 1 && !feof(b))
		errors++;
	while (!feof(a) && !feof(b))
	{
		if (key_a < key_b)
		{
			if (fprintf_s(res, "%d ", key_a) < 2)
				errors++;
			if (fread(&key_a, sizeof(int), 1, a) != 1 && !feof(a))
				errors++;
		}
		else
		{
			if (fprintf_s(res, "%d ", key_b) < 2)
				errors++;
			if (fread(&key_b, sizeof(int), 1, b) != 1 && !feof(b))
				errors++;
		}
	}

	FILE *tmp;
	int left;
	if (feof(a))
		tmp = b, left = key_b;
	else
		tmp = a, left = key_a;
	while (!feof(tmp))
	{
		if (fprintf_s(res, "%d ", left) < 2)
			errors++;
		if (fread(&left, sizeof(int), 1, tmp) != 1 && !feof(tmp))
			errors++;
	}
}

void file_copy(const char *filepath)
{
	char copy_name[128] = { "copy_" };
	strcat_s(copy_name, filepath);

	FILE *src, *copy;
	fopen_s(&src, filepath, "r+");
	fopen_s(&copy, copy_name, "w+");

	if (src == NULL)
	{
		cout << "Can't open source file: " << filepath << endl;
		system("pause");
		exit(1);
	}
	else
	{
		while (!feof(src))
		{
			char buff = getc(src);
			if (buff >= 0)
				putc(buff, copy);
		}
		fclose(src);
		fclose(copy);
		cout << "Source file copy has been created successfully!" << endl;
	}
}