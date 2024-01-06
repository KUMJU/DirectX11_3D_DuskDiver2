#pragma once

#include "Engine_Defines.h"
/*
[생각해봐야할 점]
1. SuperClass로 생성하여 다른 오브젝트풀을 상속받아 만들어 쓸 것인가?
2. 아니면 객체 매니저(예를들면 effect manager) 이런 거에 멤버 변수로 이 클래스 자체를 만들어서 사용할 것인지?

->다형성 고려하는게 어렵다.... 좀 더 생각해보기
*/

BEGIN(Engine)

class CObjectPool
{
public:
	CObjectPool();
	~CObjectPool() = default;

public:
	void Initialize();
	shared_ptr<class CGameObejct> GetEnableObject();
	void AddNewObject();

public:
	shared_ptr<CObjectPool> Create();

private:
	
	_int m_iMaxNum;
	list<shared_ptr<class CGameObejct>> m_ObjectList;
};

END