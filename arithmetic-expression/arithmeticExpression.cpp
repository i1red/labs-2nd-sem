#include "arithmeticExpression.h"
//Definition file

bool is_sym(char check_sym, string symbols)
{
	for (int i = 0; i < symbols.length(); i++)
		if (check_sym == symbols[i]) return true;
	return false;
}

void ERROR(string log, int code)
{ cout << "ERROR: " << log << endl; system("pause"); exit(code); }

//Tree node definition
expression::node::node()
{ PART = 0, LEFT = RIGHT = nullptr; }

expression::node::node(double set_part, node *set_left = nullptr, node *set_right = nullptr)
{ PART = set_part, LEFT = set_left, RIGHT = set_right; }

expression::node::node(const node &other)
{ PART = other.PART, LEFT = other.LEFT, RIGHT = other.RIGHT; }

void expression::node::set(double set_part, node *set_left = nullptr, node *set_right = nullptr)
{ PART = set_part, LEFT = set_left, RIGHT = set_right; }

void expression::node::copy(const node &other)
{ PART = other.PART, LEFT = other.LEFT, RIGHT = other.RIGHT; }

inline void expression::node::sub_With_Left()
{	if(node *SUB = LEFT) 
		{ delete RIGHT; RIGHT = nullptr; copy(*SUB); delete SUB; SUB = nullptr; }
	else 
		ERROR("Left leave doesn't exist", 1);
}

inline void expression::node::sub_With_Right()
{
	if (node *SUB = RIGHT)
		{ delete LEFT; LEFT = nullptr; copy(*SUB); delete SUB; SUB = nullptr; }
	else
		ERROR("Right leave doesn't exist", 1);
}

double expression::node::count(double Xv, double Yv, double Zv)
{
	if (PART < X) {
		if (PART == SUM)
			return LEFT->count(Xv, Yv, Zv) + RIGHT->count(Xv, Yv, Zv);
		else if (PART == DIF)
			return LEFT->count(Xv, Yv, Zv) - RIGHT->count(Xv, Yv, Zv);
		else if (PART == MLT)
			return LEFT->count(Xv, Yv, Zv) * RIGHT->count(Xv, Yv, Zv);
		else if (PART == DIV) {
			if (double rVal = RIGHT->count(Xv, Yv, Zv) != 0)
				return LEFT->count(Xv, Yv, Zv) / rVal;
			else
				ERROR("Incorrect expression. Dividing by 0", 1);
		}
	}
	else {
		if (PART == X) return Xv;
		else if (PART == Y) return Yv;
		else if (PART == Z) return Zv;
		else return PART;
	}
}

void expression::node::simplify()
{
	if (PART < X) {
		if (PART == SUM) {
			if (LEFT->PART == 0) sub_With_Right();
			else if (RIGHT->PART == 0) sub_With_Left();
			else if (LEFT->PART > 0 && RIGHT->PART > 0) 
				{ PART = LEFT->PART + RIGHT->PART; delete LEFT; delete RIGHT; LEFT = RIGHT = nullptr; }
		}
		else if (PART == DIF) {
			if (LEFT->PART == RIGHT->PART && LEFT->PART >= -3) 
				{ PART = 0; delete LEFT; delete RIGHT; LEFT = RIGHT = nullptr; }
			else if (RIGHT->PART == 0) sub_With_Left();
			else if (LEFT->PART > 0 && RIGHT->PART > 0) {
				if (LEFT->PART > RIGHT->PART) 
					{ PART = LEFT->PART - RIGHT->PART; delete LEFT; delete RIGHT; LEFT = RIGHT = nullptr; }
				else 
					{ RIGHT->PART = RIGHT->PART - LEFT->PART; LEFT->PART = 0; }
			}
		}
		else if (PART == MLT) {
			if (LEFT->PART == 0 || RIGHT->PART == 0) 
				{ PART = 0; delete LEFT; delete RIGHT; LEFT = RIGHT = nullptr;}
			else if (LEFT->PART == 1) sub_With_Right();
			else if (RIGHT->PART == 1) sub_With_Left();
			else if (LEFT->PART > 0 && RIGHT->PART > 0)
				{ PART = LEFT->PART * RIGHT->PART; delete LEFT; delete RIGHT; LEFT = RIGHT = nullptr; }
		}
		else if (PART == DIV) {
			if (RIGHT->PART == 0) 
				ERROR("Incorrect expression. Dividing by 0", 1);
			else if (LEFT->PART == 0) 
				{ PART = 0; delete LEFT; delete RIGHT; LEFT = RIGHT = nullptr; }
			else if (RIGHT->PART == 1) sub_With_Left();
			else if (LEFT->PART == RIGHT->PART && LEFT->PART >= X) 
				{ PART = 1; delete LEFT; delete RIGHT; LEFT = RIGHT = nullptr; }
			else if (LEFT->PART > 0 && RIGHT->PART > 0)
				{ PART = LEFT->PART / RIGHT->PART; delete LEFT; delete RIGHT; LEFT = RIGHT = nullptr; }
		}
	}
	else return;
}

//Tree definition
expression::expression() 
{ EXPR_FATHER = nullptr; }

expression::expression(const expression &other)
{
	EXPR_FATHER = GETCOPY(other.EXPR_FATHER);
}

void expression::print()
{
	if (EXPR_FATHER)
		{ PRINT(EXPR_FATHER); cout << endl; }
	else ERROR("Empty expression", 1);
}

void expression::get(string exp)
{
	if(!EXPR_FATHER) 
		GET(EXPR_FATHER = new node, exp);
	else ERROR("Expression is full", 1);
}

expression expression::devirative(double difByVAR)
{
	expression exp_dev = *this;
	DEVIRATIVE(exp_dev.EXPR_FATHER, difByVAR);
	exp_dev.simplify();
	return exp_dev;
}

double expression::count(double Xval, double Yval, double Zval)
{
	if(EXPR_FATHER) return EXPR_FATHER->count(Xval, Yval, Zval);
	else ERROR("Empty expression", 1);
}

void expression::simplify() 
{ SIMPLIFY(EXPR_FATHER); }

void expression::empty()
{ DESTRUCT(&EXPR_FATHER);}

expression::~expression() 
{ DESTRUCT(&EXPR_FATHER); }

void expression::SIMPLIFY(node* curr)
{
	if (!(curr)) return;

	SIMPLIFY((curr)->LEFT);
	SIMPLIFY((curr)->RIGHT);
	(curr)->simplify();
}

void expression::DESTRUCT(node** curr)
{
	if (!(*curr)) return;
	DESTRUCT(&(*curr)->LEFT); DESTRUCT(&(*curr)->RIGHT); delete *curr; *curr = nullptr;
}

expression::node* expression::GETCOPY(node* copy)
{
	if (!copy) return nullptr;
	else return new node(copy->PART, GETCOPY(copy->LEFT), GETCOPY(copy->RIGHT));
}

void expression::DEVIRATIVE(node* curr, double difVAR)
{
	if (!curr) return;

	double MARKER = curr->PART;
	if (MARKER == difVAR)
		curr->PART = 1;
	else if (MARKER == DIF || MARKER == SUM)
	{
		DEVIRATIVE(curr->LEFT, difVAR);
		DEVIRATIVE(curr->RIGHT, difVAR);
	}
	else if (MARKER == MLT)
	{
		node *devL = new node(MLT, GETCOPY(curr->LEFT), curr->RIGHT);
		node *devR = new node(MLT, GETCOPY(curr->RIGHT), curr->LEFT);
		DEVIRATIVE(devL->LEFT, difVAR);
		DEVIRATIVE(devR->LEFT, difVAR);

		curr->set(SUM, devL, devR);
	}
	else if (MARKER == DIV)
	{
		node *devL = new node(MLT, GETCOPY(curr->LEFT), curr->RIGHT);
		node *devR = new node(MLT, GETCOPY(curr->RIGHT), curr->LEFT);
		DEVIRATIVE(devL->LEFT, difVAR);
		DEVIRATIVE(devR->LEFT, difVAR);

		curr->LEFT = new node(DIF, devL, devR);
		curr->RIGHT = new node(MLT, GETCOPY(curr->RIGHT), GETCOPY(curr->RIGHT));
	}
	else curr->PART = 0;
}

void expression::PRINT(node *curr)
{
	if (!curr) return;
	else {
		if (curr->PART < X)
		{
			bool check_top = curr == EXPR_FATHER;
			if (!check_top) cout << "(";
			PRINT(curr->LEFT);
			if (curr->PART == SUM) cout << '+';
			else if (curr->PART == DIF) cout << '-';
			else if (curr->PART == MLT) cout << '*';
			else if (curr->PART == DIV) cout << '/';
			PRINT(curr->RIGHT);
			if (!check_top) cout << ')';
		}
		else
		{
			if (curr->PART == X) cout << 'X';
			else if (curr->PART == Y) cout << 'Y';
			else if (curr->PART == Z) cout << 'Z';
			else cout << curr->PART;
		}
	}
}

void expression::GET(node *curr, string expr)
{
	if (expr.length() == 0) ERROR("Incorrect expression", 1);
	else if (expr.front() != '(' && expr.back() != ')')
	{
		if (expr == "X") curr->PART = X;
		else if (expr == "Y") curr->PART = Y;
		else if (expr == "Z") curr->PART = Z;
		else {
			try
			{
				curr->PART = stod(expr);
			}
			catch (const exception &error)
			{
				ERROR(error.what(), 1);
			}
		}
	}
	else if (expr.front() == '(' && expr.back() == ')') {
		expr.erase(0, 1);
		expr.pop_back();
		int brackets = 0;
		for (size_t i = 0; i < expr.length(); i++)
		{
			if (expr[i] == '(') brackets++;
			else if (expr[i] == ')') brackets--;
			else if (brackets == 0) { 
				bool call_error = true;
				while (i < expr.length()) {
					if (is_sym(expr[i], "+-*/")) {
						call_error = false; break;
					}
					else i++;
				}
				if (call_error) ERROR("Incorrect expression", 1);
				switch (expr[i]) {
					case '+':curr->PART = SUM; break;
					case '-':curr->PART = DIF; break;
					case '*':curr->PART = MLT; break;
					case '/':curr->PART = DIV; break;
					default: ERROR("Incorrect expression", 1); break;
				}
				GET(curr->LEFT = new node, expr.substr(0, i));
				GET(curr->RIGHT = new node, expr.substr(i + 1, expr.length() - i - 1));
				break;
			}
		}
	}
	else ERROR("Incorrect expression", 1);
}