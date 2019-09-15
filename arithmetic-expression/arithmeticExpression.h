#ifndef ARITHMETICEXPRESSION_H
#define ARITHMETICEXPRESSION_H


#include <iostream>
#include <string>
using namespace std;

const double	SUM = -6.234779405, DIF = -5.234779405, MLT = -4.234779405, DIV = -3.234779405, 
				X = -2.234779405, Y = -1.234779405, Z = -0.234779405;

class expression {
	//����� ������
	struct node {
		//���� �����
		double PART; node *RIGHT; node *LEFT;
		//��������� �����(������������)
		node(); node(double, node*, node*); node(const node&);
		//������ ��� ������ � ������
		void set(double, node*, node*);
		inline void sub_With_Left(); inline void sub_With_Right();
		double count(double, double, double);
		void copy(const node&);
		void simplify();
	};
	node *EXPR_FATHER;
public:
	//��������� ������
	expression(); expression(const expression&);
	//�������� ��� ����������� ������
	void get(string);
	void print();
	void simplify();
	void empty();
	expression devirative(double);
	double count(double, double, double);   
	//����������(��������)
	~expression();
private:
	//��������� ������� ��� ������ � �������
	void SIMPLIFY(node*);
	void DESTRUCT(node**);
	node* GETCOPY(node*);
	void DEVIRATIVE(node*, double);
	void PRINT(node*);
	void GET(node*, string);
};


#endif