#pragma once

#include "Client_Defines.h"

#include "GameObject.h"

BEGIN(Client)

class CSkill abstract : public CGameObject
{
public:
	enum class EOWNER_TYPE {
		OWNER_PLAYER,
		OWNER_MONSTER,
		OWNER_END
	};


public:
	CSkill();
	~CSkill() = default;

public:
	virtual HRESULT Initialize(CTransform::TRANSFORM_DESC* _pDesc);

	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	//Render 필요없음
	virtual HRESULT Render();

public:
	EOWNER_TYPE GetOwnerType() { return m_eSkillOwner; }
	_uint GetDamage() { return m_iDamage; }

public:
	void ActiveSkill();

private:
	_float m_fRadius = 0.f; // 스킬 콜라이더 범위
	_float m_fSkillDuration = 0.f; //스킬 사용시간
	_float m_fSkillActiveTime = 0.f;

	_uint m_iDamage = 0; //데미지
	_float m_fSkillCoolTime = 0.f; //필요할까? 필요없으면 나중에 지우기

	EOWNER_TYPE m_eSkillOwner = EOWNER_TYPE::OWNER_END;

private:
	//colider component (sphere + aabb + obb)

private:
	_bool m_bAirBorne = false; //에어본 공격인지 판별
	
};

END