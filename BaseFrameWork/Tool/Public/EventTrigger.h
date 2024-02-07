#pragma once

#include "Tool_Defines.h"
#include "GameObject.h"
#include "Collider.h"

BEGIN(Engine)

class CEventTrigger : public CGameObject
{
public:
	enum class ETRIGGER_TYPE {
		TRI_PORTAL,
		TRI_CUTSCENE,
		TRI_MONSTER,
		TRI_MINIGAME,
		TRI_END
	};


public:
	CEventTrigger();
	virtual ~CEventTrigger() = default;

public:
	virtual HRESULT Initialize(CCollider::COLLIDER_DESC _desc, _float3 _vTriggerPos);
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;


protected:
	ETRIGGER_TYPE m_eTriggerType = ETRIGGER_TYPE::TRI_END;

	//활성화 방식 true : 터치 / false: 타격
	_bool m_IsTouchAble = false;

protected:
	shared_ptr<CCollider> m_pCollider = nullptr;

};

END