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
	//Render �ʿ����
	virtual HRESULT Render();

public:
	EOWNER_TYPE GetOwnerType() { return m_eSkillOwner; }
	_uint GetDamage() { return m_iDamage; }

public:
	void ActiveSkill();

private:
	_float m_fRadius = 0.f; // ��ų �ݶ��̴� ����
	_float m_fSkillDuration = 0.f; //��ų ���ð�
	_float m_fSkillActiveTime = 0.f;

	_uint m_iDamage = 0; //������
	_float m_fSkillCoolTime = 0.f; //�ʿ��ұ�? �ʿ������ ���߿� �����

	EOWNER_TYPE m_eSkillOwner = EOWNER_TYPE::OWNER_END;

private:
	//colider component (sphere + aabb + obb)

private:
	_bool m_bAirBorne = false; //��� �������� �Ǻ�
	
};

END