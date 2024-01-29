#pragma once

#include "MapObject.h"
#include "MonsterPool.h"

BEGIN(Engine)
class CCollider;
END


BEGIN(Client)

class CMonsterTower : public CMapObject
{

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


public:
	void TowerActive();

private:
	_bool m_IsActived = false;

private:
	shared_ptr<CModel> m_pRingModel = nullptr;
	shared_ptr<CCollider> m_pCollider = nullptr;
	shared_ptr<CTransform> m_pRingTransform = nullptr;

	list<CMonsterPool::SPAWN_INFO> m_SpawnMonsterList;

public:
	static shared_ptr<CMonsterTower> Create();

};

END