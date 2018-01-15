#pragma once

/************************************************************************/
/* 
	һ���򵥵�״̬��                                                     
	��ID��ʶһ��״̬
	״̬��ת���̣�������ע�ᡣ
*/
/************************************************************************/

#include <map>

namespace fsm
{
	class StateMachine;

	class BaseState
	{
	public:
		virtual void OnEnter(StateMachine& sm) {};
		virtual void OnExit(StateMachine& sm) {};
		virtual void Update(StateMachine& sm) {};
		virtual int GetId() const = 0;
	};

	template<int StateId>
	class State : public BaseState
	{
	public:
		virtual int GetId() const { return GetStateId(); };
	public:
		static int GetStateId() { return StateId; }
	};


	class StateMachine
	{
	public:
		StateMachine(void* Owner);
		~StateMachine();
	public:
		template<class LeftState, class RightState>
		void AddState()
		{
			int LeftId = LeftState::GetStateId();
			int RightId = RightState::GetStateId();

			auto LeftIt = AllStateMap.find(LeftId);
			if (LeftIt == AllStateMap.end())
			{
				AllStateMap[LeftId] = (new LeftState);
			}
			auto RightIt = AllStateMap.find(RightId);
			if (RightIt == AllStateMap.end())
			{
				AllStateMap[RightId] = (new RightState);
			}
			TransferStateMap[std::make_pair(LeftId, RightId)] = AllStateMap[RightId];
		}
		template<class BeginState>
		void Run()
		{
			int LeftId = BeginState::GetStateId();
			CurrentState = AllStateMap[LeftId];
			CurrentState->OnEnter(*this);
		}
		template<class NextStateClass>
		void SetNextState() 
		{ 
			NextStateId = NextStateClass::GetStateId(); 
		}

		void Update();

		BaseState* GetCurrentState() { return CurrentState; }

		template<class T>
		T* GetOwner() { return static_cast<T*>(Owner); }
	private:
		BaseState* CurrentState;
		std::map<int, BaseState*> AllStateMap;	// ���е�״̬
		std::map<std::pair<int, int>, BaseState*> TransferStateMap;	// ״̬��תͼ
		int NextStateId;
		void* Owner;
	};
}