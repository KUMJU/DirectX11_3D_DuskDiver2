#pragma once

#include "Client_Defines.h"

#include "GameObject.h"

BEGIN(Engine)
class CCollider;
END

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
	virtual ~CSkill() = default;


public:
	virtual HRESULT Initialize();

	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	void SetOwnerTransform(shared_ptr<CTransform> _pOwnerTransform) { m_pOwnerTransform = _pOwnerTransform; }


public:
	_bool GetCancleAble() { return m_bCancle; }

public:
	EOWNER_TYPE GetOwnerType() { return m_eSkillOwner; }
	_uint GetDamage() { return m_iDamage; }

public:
	void ActiveSkill();
	void SkillReset();
	void SetSkillIndex(_uint _iSkillIdx) { iSkillIndex = _iSkillIdx; }
	_uint GetSkillIndex() { return iSkillIndex; }

protected:
	_float m_fSkillDuration = 0.f; //스킬 사용시간
	_float m_fSkillActiveTime = 0.f; //스킬 전체시간
	_float m_fStartDelayTime = 0.f; //스킬 준비자세 시간
	_float m_fCurrentAnimTime = 0.f; //시작 이후 시간 재주는 중 

	_uint m_iDamage = 0; //데미지
	_float m_fSkillCoolTime = 0.f; //필요할까? 필요없으면 나중에 지우기

	EOWNER_TYPE m_eSkillOwner = EOWNER_TYPE::OWNER_END;

	shared_ptr<CTransform> m_pOwnerTransform = nullptr;

//Collide
protected:
	//shared_ptr<CCollider> m_pCollider = nullptr;
	void OnCollide(CGameObject::EObjType _eObjType, shared_ptr<CCollider> _pCol) override;

public:
	_bool GetIsKnokUp() { return m_bKnokUp; }
	_bool GetIsKnokBack() { return m_bKnokBack; }

	_bool GetIsDownAttack() { return m_bDownAtk; }

	_float GetKnokUpDistance() { return m_fKnockUpDistance; }
	_float GetKnokBackDistance() { return m_fKnockBackDistance; }

protected:
	_bool m_bKnokUp = false; //에어본 공격인지 판별
	_bool m_SlowTime = 0.f; //느려지는 효과가 있으면 시간보정값? 
	_bool m_bKnokBack = false;
	_bool m_bCancle = false;
	_bool m_bDownAtk = false;

	_uint iSkillIndex = 0;

	_float m_fKnockBackDistance = 0.f;
	_float m_fKnockUpDistance = 0.f;


	wstring m_strCutSceneName = TEXT(""); // 이벤트 카메라에 넘겨줄 이벤트씬 이름

	//충돌체 정보 
	vector<shared_ptr<class CCollider>> m_Colliders;


};

END