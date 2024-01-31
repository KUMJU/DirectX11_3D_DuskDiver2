#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CCollider;
class CModel;
class CShader;

END


BEGIN(Client)

class CPlayer;
 
class CMole : public CGameObject
{
public:
	CMole();
	virtual ~CMole() = default;

public:
	virtual HRESULT Initialize(_uint _iTypeNum);
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void OnCollide(EObjType _eObjType, shared_ptr<CCollider> _pCollider) override;

public:
	void SetMinigameMgr(shared_ptr<class CMinigameMole> _pMinigame) {m_pMinigame = _pMinigame;}
	void StateReset();

	void SetSlotIdx(_uint _iIdx) { m_iCurrentSlotNum = _iIdx; }
	_uint GetSlotIdx() { return m_iCurrentSlotNum; }

private:
	HRESULT BindShaderResources();

private:	
	_uint m_MoleTypeNum = 0;
	_float m_fSpeed = 3.f;
	_float m_MovingTime = 0.f;
	_float m_fDir = 1.f;

	weak_ptr<class CMinigameMole> m_pMinigame;

	_bool m_bCollLock = false;
	_uint m_iCurrentSlotNum = 100;

private:
	shared_ptr<CCollider> m_pColider = nullptr;
	shared_ptr<CModel> m_pModelCom = nullptr;
	shared_ptr<CShader> m_pShader = nullptr;

private:
	shared_ptr<CPlayer> m_pPlayer = nullptr;

public:

	static shared_ptr<CMole> Create(_uint _iTypeNum);


};

END