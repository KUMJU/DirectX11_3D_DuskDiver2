#pragma once

#include "Tool_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CNavigation;
class CCollider;
class CShader;
class CModel;
END

BEGIN(Tool)

class CHockeyBall : public CGameObject
{
public:
	CHockeyBall();
	virtual ~CHockeyBall() = default;

public:
	virtual HRESULT Initialize( _fvector _vStartPos);
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

protected:
	virtual void OnCollide(EObjType _eObjType, shared_ptr<CCollider> _pCollider) override;

private:
	HRESULT BindShaderResources();

private:
	//벽에 충돌시 플레이어 재추격
	void ComputeLookVector();

private:
	shared_ptr<class CPlayer> m_pPlayer = nullptr;
	shared_ptr<CTransform> m_pPlrTransform = nullptr;

private:
	shared_ptr<CNavigation> m_pNavigation = nullptr;
	shared_ptr<CCollider> m_pCollider = nullptr;
	shared_ptr<CModel> m_pModel = nullptr;
	shared_ptr<CShader> m_pShader = nullptr;

private:
	_float m_fColRadius = 0.f;

public:
	static shared_ptr<CHockeyBall> Create(_fvector _vStartPos);


};

END