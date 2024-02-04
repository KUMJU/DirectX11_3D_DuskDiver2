#pragma once

#include "MapObject.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)

class CCoin : public CMapObject
{
public:
	CCoin();
	virtual ~CCoin() = default;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void OnCollide(EObjType _eObjType, shared_ptr<CCollider> _pCollider) override;

private:
	shared_ptr<CCollider> m_pCollider = nullptr;

public:
	static shared_ptr<CCoin> Create();

};

END