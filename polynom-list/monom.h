#pragma once
#include <iostream>
#include <cmath>
using namespace std;

struct monom {
	double k;
	int X;
	int Y;
	int Z;
	monom();
	monom(const monom&);
	monom(double, int, int, int);
	void print();
	void inputMonom();
	double value(double, double, double);
	monom derX();
	monom derY();
	monom derZ();
	bool ifAdd(monom);
	monom operator*(monom&);
	monom operator+(monom&);
};