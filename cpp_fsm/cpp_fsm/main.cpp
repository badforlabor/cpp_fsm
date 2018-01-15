#include <iostream>
#include "fsm.h"
using namespace fsm;

enum
{
	EStateNone,
	EState1,
	EState2,
	EState3,
};

class State1 : public State<EState1>
{
public:
	virtual void OnEnter(StateMachine& sm)
	{
		std::cout << "enter state1" << std::endl;
	}
	virtual void OnExit(StateMachine& sm)
	{
		std::cout << "exit state1" << std::endl;
	}
	virtual void Updata(StateMachine& sm)
	{
	
	}
};
class State2 : public State<EState2>
{
	virtual void OnEnter(StateMachine& sm)
	{
		std::cout << "enter state2" << std::endl;
	}
	virtual void OnExit(StateMachine& sm)
	{
		std::cout << "exit state2" << std::endl;
	}
	virtual void Updata(StateMachine& sm)
	{

	}
};
class State3 : public State<EState3>
{
	virtual void OnEnter(StateMachine& sm)
	{
		std::cout << "enter state3" << std::endl;
	}
	virtual void OnExit(StateMachine& sm)
	{
		std::cout << "exit state3" << std::endl;
	}
	virtual void Updata(StateMachine& sm)
	{

	}
};


int main()
{
	StateMachine sm(NULL);
	sm.AddState<State1, State2>();
	sm.AddState<State1, State3>();
	sm.AddState<State2, State3>();

	sm.Run<State1>();

	sm.SetNextState< State2 > ();
	sm.Update();

	sm.SetNextState< State1 > ();
	sm.Update();

	sm.SetNextState< State3 > ();
	sm.Update();
	return 0;
}