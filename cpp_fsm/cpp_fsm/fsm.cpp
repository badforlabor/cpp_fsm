#include "fsm.h"
using namespace fsm;

static const int INVALID_ID = -1;

StateMachine::StateMachine(void* InOwner)
	: Owner(InOwner), CurrentState(NULL), NextStateId(INVALID_ID)
{

}
StateMachine::~StateMachine()
{
	for (auto it = AllStateMap.begin(); it != AllStateMap.end(); it++)
	{
		BaseState* ptr = it->second;
		DeleteState(ptr);
	}
	AllStateMap.clear();
	TransferStateMap.clear();
	CurrentState = NULL;
}

void StateMachine::Update()
{
	if (CurrentState != NULL)
	{
		CurrentState->Update(*this);
	}
	if (NextStateId != INVALID_ID)
	{
		int NextState = NextStateId;
		NextStateId = INVALID_ID;

		auto it = TransferStateMap.find(std::make_pair(CurrentState->GetId(), NextState));
		if (it == TransferStateMap.end())
		{
			//assert(false);
			OnSetNextStateFailed(CurrentState->GetId(), NextState);
		}
		else
		{
			CurrentState->OnExit(*this);
			CurrentState = it->second;
			CurrentState->OnEnter(*this);
		}
	}
}
