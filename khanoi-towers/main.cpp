#include <iostream>
#include "cStack.h"
using namespace std;

enum state_list{BEGIN_TASK, REC_SHIFT, POP_STACK};

struct task {
	int STEP;
	int DISK_N;
	int POLE_FROM;
	int POLE_TO;
	int POLE_TMP;
	task()
	{
		STEP = 0, DISK_N = 0, POLE_FROM = 0, POLE_TO = 0, POLE_TMP = 0;
	}
	task(int stepVal, int diskVal, int pFromVal, int pToVal, int pTmpVal)
	{
		STEP = stepVal, DISK_N = diskVal, POLE_FROM = pFromVal, POLE_TO = pToVal, POLE_TMP = pTmpVal;
	}
};

void towers(int height)
{
	if (height <= 0)
	{
		cout << "ERROR: Incorrect height" << endl;
		system("pause");
		exit(1);
	}
	if (height == 64)
	{
		cout << "Do you want to start the apocalypse?" << endl << "It will take thousands years of time. Enter 'e' to continue or something else to not" << endl;
		char choice;
		cin >> choice;
		if (choice != 'e')
			return;
	}
	cStack<task> task_stack;
	task main_call(0, height, 1, 2, 3);
	task_stack.push(main_call);

	while (!task_stack.is_empty())
	{
		task &state = task_stack.peek();
		switch (state.STEP)
		{
			case BEGIN_TASK:
			{
				if (state.DISK_N == 0)
					task_stack.pop();
				else
				{
					state.STEP = REC_SHIFT;
					task n_task(0, state.DISK_N - 1, state.POLE_FROM, state.POLE_TMP, state.POLE_TO);
					task_stack.push(n_task);
				}
				break;
			}
			case REC_SHIFT:
			{
				cout << "Move DISK #" << state.DISK_N << " from POLE " << state.POLE_FROM << " to POLE " << state.POLE_TO << endl;
				state.STEP = POP_STACK;
				task n_task(0, state.DISK_N - 1, state.POLE_TMP, state.POLE_TO, state.POLE_FROM);
				task_stack.push(n_task);
				break;
			}
			case POP_STACK:
			{
				task_stack.pop();
				break;
			}
		}
	}
}


int main()
{
	towers(4);
	
	system("pause");
	return 0;
}