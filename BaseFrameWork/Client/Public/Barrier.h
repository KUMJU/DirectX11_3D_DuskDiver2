#pragma once

#include "MapObject.h"

BEGIN(Client)

class CBarrier : public CMapObject
{
public:
	CBarrier();
	virtual ~CBarrier() = default;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void OnCollide(EObjType _eObjType, shared_ptr<CCollider> _pCollider) override;

private:


public:
	static shared_ptr<CBarrier> Create();

};

END
