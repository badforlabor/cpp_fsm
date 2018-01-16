#pragma once

/************************************************************************/
/* 
	һ���򵥵�״̬��                                                     
	��ID��ʶһ��״̬
	״̬��ת���̣�������ע�ᡣ

	ʹ��˼·��
		�½�һ�������࣬�����������״̬������ࡣ
		�½�һ��StateMachine���󣬴���������ָ�룬ͨ��GetOwner<T>��ȡ�������ٶ��ࡣStateMachine�У�������洢��Ϣ��������Ϣ����洢���������С�
		�½�һ��State��Ʃ��class State1 : public State<EState1>��ͬ��������Ҫ��State���м�¼�κ���Ϣ��������Ϣ�������������С�
			State�൱���߼���������������������߼��ġ�
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

	template<class T>
	T* NewState()
	{
		return new T;
	}
	inline void DeleteState(BaseState* ptr)
	{
		delete ptr;
	}

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
	private:
		// ��������
		StateMachine(const StateMachine& sm) {}
		StateMachine& operator=(const StateMachine& sm) {}
	public:
		template<class LeftState, class RightState>
		void AddState()
		{
			int LeftId = LeftState::GetStateId();
			int RightId = RightState::GetStateId();

			auto LeftIt = AllStateMap.find(LeftId);
			if (LeftIt == AllStateMap.end())
			{
				AllStateMap[LeftId] = NewState<LeftState>();
			}
			auto RightIt = AllStateMap.find(RightId);
			if (RightIt == AllStateMap.end())
			{
				AllStateMap[RightId] = NewState<RightState>();
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