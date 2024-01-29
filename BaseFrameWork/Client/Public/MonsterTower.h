#pragma once

#include "MapObject.h"

BEGIN(Engine)
class CCollider;
END


BEGIN(Client)

class CMonsterTower : public CMapObject
{
public:
	struct tag_MonsterSpawn 
	{
		_uint iMonsterType;
		_float3 vMonsterPos;

	};


public:
	CMonsterTower();
	virtual ~CMonsterTower() = default;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void OnCollide(EObjType _eObjType, shared_ptr<CCollider> _pCollider) override;

private:
	shared_ptr<CModel> m_pRingModel = nullptr;
	shared_ptr<CCollider> m_pCollider = nullptr;

public:
	static shared_ptr<CMonsterTower> Create();

};

END