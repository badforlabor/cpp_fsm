#pragma once

/************************************************************************/
/* 
	一个简单的状态机                                                     
	用ID标识一个状态
	状态跳转过程，必须先注册。

	使用思路：
		新建一个宿主类，就是正常你分状态处理的类。
		新建一个StateMachine对象，传入宿主类指针，通过GetOwner<T>获取真正的速度类。StateMachine中，不建议存储信息，所有信息建议存储在宿主类中。
		新建一个State，譬如class State1 : public State<EState1>。同样，不需要在State类中记录任何信息，所有信息都放在宿主类中。
			State相当于逻辑处理组件。仅用来处理逻辑的。
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
		// 不允许拷贝
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
		std::map<int, BaseState*> AllStateMap;	// 所有的状态
		std::map<std::pair<int, int>, BaseState*> TransferStateMap;	// 状态跳转图
		int NextStateId;
		void* Owner;
	};
}