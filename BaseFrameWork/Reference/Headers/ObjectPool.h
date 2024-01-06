#pragma once

#include "Engine_Defines.h"
/*
[�����غ����� ��]
1. SuperClass�� �����Ͽ� �ٸ� ������ƮǮ�� ��ӹ޾� ����� �� ���ΰ�?
2. �ƴϸ� ��ü �Ŵ���(������� effect manager) �̷� �ſ� ��� ������ �� Ŭ���� ��ü�� ���� ����� ������?

->������ ����ϴ°� ��ƴ�.... �� �� �����غ���
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