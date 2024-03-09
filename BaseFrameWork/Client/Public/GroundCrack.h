#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
END

BEGIN(Client)

class CGroundCrack : public CGameObject
{
public:
	CGroundCrack();
	virtual ~CGroundCrack() = default;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	void ActiveSkill();

private:
	shared_ptr<CModel> m_pModel = nullptr;
	shared_ptr<CShader> m_pShader = nullptr;

private:
	HRESULT BindAnimShader();

private:
	_float m_fAccTime = 0.f;

public:
	static shared_ptr<CGroundCrack> Create();

};

END