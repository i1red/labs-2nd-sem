#include <iostream>
#include <string>
#include "arithmeticExpression.h"
using namespace std;

int main()
{
	expression test;
	//test input
	string input1 = "((X*Z)+(0+(1*(6-6))))";
	string input2 = "((6+7)/(8*2))";
	string input3 = "((Z/Z)*((2-8)*X))";
	string input4 = "82";
	string incorrect_input1 = "8*X";
	string incorrect_input2 = "(8-X";
	string incorrect_input3 = "(K86*Y)";
	string incorrect_input4 = "(((X*Y)/Z)/0)";
	test.get(input3);
	
	//test output
	cout << "Exp:" << endl;
	test.print();
	//----------

	//test simplify
	cout << "Simplified exp:" << endl;
	test.simplify();
	test.print();
	//----------

	//test value(a,b,c)
	double a = 3, b = 1, c = 2;
	cout << "Value (" << a << ',' << b << ',' << c << "):" << endl;
	cout << test.count(a, b, c) << endl;
	//----------

	//test devirative
	expression devX = test.devirative(X);
	cout << "dev by X:" << endl;
	devX.print();
	expression devY = test.devirative(Y);
	cout << "dev by Y:" << endl;
	devY.print();
	expression devZ = test.devirative(Z);
	cout << "dev by Z:" << endl;
	devZ.print();
	//----------

	

	system("pause");
	return 0;
}