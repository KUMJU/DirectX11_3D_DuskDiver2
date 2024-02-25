#pragma once

#include "Client_Defines.h"
#include "Minigame.h"

BEGIN(Client)

class CMinigameMole : public CMinigame
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
	virtual void GameStart();
	virtual void GameEnd();

	//밖에서 두더지가 호출해줘야할듯
	void GetScore();
	void SpawnMole();

public:
	virtual void SetCameraEvent();
	virtual void StartEventCutScene();
	virtual void EndEventCutScene();
	
	void ProcessingEvent(_float _fTimeDelta);
	void ProcessingEndEvent(_float _fTimeDelta);

private:
	_float m_fSpawnCoolTime = 0.f;

private:
	_uint m_iCurrentScore = 0;
	 

private:
	vector<shared_ptr<class CMole>> m_Moles[3]; //Mole Pool
	list<shared_ptr<class CMole>> m_ActiveMoles; // 실질적으로 게임에서 보이는 녀석들 

	_float4 m_vMolePos[9];
	_bool m_HoleSlot[9];


private:

	_bool m_bEndEventStart = false;

	//EndEvent용 
	shared_ptr<class CBear> m_pBear = nullptr;
	shared_ptr<class CUI_SequenceTex> m_pDustImg = nullptr;

private:

	_vector vCutEventPos1 = { 0.f, 0.f, 0.f, 0.f };
	_vector vCutEventPos2 = { 0.f, 0.f, 0.f, 0.f };
	_vector vCutEventPos3 = { 0.f, 0.f, 0.f, 0.f };

	_vector vEndCutScenePos = _vector();



public:
	static shared_ptr<CMinigameMole> Create();
};

END