#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CCollider;
class CModel;
class CShader;

END


BEGIN(Client)

//콜리전을 몬스터 그룹에 넣으면 편할것 같음
 
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

private:
	HRESULT BindShaderResources();

private:	
	_uint m_MoleTypeNum = 0;
	weak_ptr<class CMinigameMole> m_pMinigame;

private:
	shared_ptr<CCollider> m_pColider = nullptr;
	shared_ptr<CModel> m_pModelCom = nullptr;
	shared_ptr<CShader> m_pShader = nullptr;

public:

	static shared_ptr<CMole> Create(_uint _iTypeNum);


};

END