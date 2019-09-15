#include "monom.h"

monom::monom()
{
	k = 0, X = 0, Y = 0, Z = 0;
}
monom::monom(const monom &other)
{
	k = other.k;
	X = other.X;
	Y = other.Y;
	Z = other.Z;
}
monom::monom(double coef, int xDgr, int yDgr, int zDgr)
{
	k = coef, X = xDgr, Y = yDgr, Z = zDgr;
}
void monom::print()
{
	if (k == 0)
		return;
	else
	{
		if (k != 1)
		{
			if (k < 0)
				cout << " - " << abs(k);
			else
				cout << k;
		}
	}
	if (X != 0)
	{
		cout << "X";
		if (X != 1)
			cout << "^(" << X << ")";
	}
	if (Y != 0)
	{
		cout << "Y";
		if (Y != 1)
			cout << "^(" << Y << ")";
	}
	if (Z != 0)
	{
		cout << "Z";
		if (Z != 1)
			cout << "^(" << Z << ")";
	}
}
double monom::value(double xV, double yV, double zV)
{
	return k * pow(xV, X)*pow(yV, Y)*pow(zV, Z);
}
monom monom::derX()
{
	monom res;
	if (X == 0)
		res.k = 0;
	else
	{
		res.k = k * X;
		res.X = X-1;
		res.Y = Y;
		res.Z = Z;
	}
	return res;
}
monom monom::derY()
{
	monom res;
	if (Y == 0)
		res.k = 0;
	else
	{
		res.k = k * Y;
		res.X = X;
		res.Y = Y-1;
		res.Z = Z;
	}
	return res;
}
monom monom::derZ()
{
	monom res;
	if (Z == 0)
		res.k = 0;
	else
	{
		res.k = k * Z;
		res.X = X;
		res.Y = Y;
		res.Z = Z-1;
	}
	return res;
}
bool monom::ifAdd(monom other)
{
	if (X == other.X && Y == other.Y && Z == other.Z)
		return true;
	else
		return false;
}
monom monom::operator*(monom &other)
{
	monom res;
	res.k = k * other.k;
	res.X = X + other.X;
	res.Y = Y + other.Y;
	res.Z = Z + other.Z;
	return res;
}
monom monom::operator+(monom &other)
{
	if (ifAdd(other))
	{
		monom res;
		res.k = k + other.k;
		res.X = X;
		res.Y = Y;
		res.Z = Z;
		return res;
	}
	else
	{
		cout << "ERROR: Can't be added" << endl;
		exit(1);
	}
}
void monom::inputMonom()
{
	cout << "Coeficient=";
	cin >> k;
	cout << "X degree=";
	cin >> X;
	cout << "Y degree=";
	cin >> Y;
	cout << "Z degree=";
	cin >> Z;
}