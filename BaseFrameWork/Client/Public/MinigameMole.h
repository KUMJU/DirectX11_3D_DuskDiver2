#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CMinigameMole : public CGameObject
{
public:
	CMinigameMole();
	virtual ~CMinigameMole() = default;

public:
	virtual HRESULT Initialize(CTransform::TRANSFORM_DESC* _pDesc);

	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();


public:
	void GameStart();
	void GameEnd();

	//�ۿ��� �δ����� ȣ��������ҵ�
	void GetScore();
	void SpawnMole();

private:
	_float m_fSpawnCoolTime = 0.f;


private:
	_bool m_bDone = false;
	_bool m_bProcessing = false;

	_uint m_iCurrentScore = 0;
	 

private:
	vector<shared_ptr<class CMole>> m_Moles[3]; //Mole Pool
	list<shared_ptr<class CMole>> m_ActiveMoles; // ���������� ���ӿ��� ���̴� �༮�� 

	_float3 m_vMolePos[9];
	_bool m_HoleSlot[9];

public:
	static shared_ptr<CMinigameMole> Create();
};

END