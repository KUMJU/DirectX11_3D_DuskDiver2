#pragma once

#include "Engine_Defines.h"
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public std::enable_shared_from_this<CGameObject>
{
public:
	enum EObjType{ OBJ_PLAYER, OBJ_MONSTER, OBJ_UI, OBJ_ENV, OBJ_END };

public:
	CGameObject();
	CGameObject(const CGameObject& _rhs);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize(CTransform::TRANSFORM_DESC* _pDesc);

	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	bool IsActive() { return m_IsActive; }
	bool IsEnabled() { return m_IsEnabled; }

	void SetInActive() { m_IsActive = false; }
	void SetPosition(_vector _vPos);

public:
	shared_ptr<CComponent> GetComponent(const wstring& _strComTag);

protected:
	shared_ptr<CTransform> m_pTransformCom = nullptr;

public:
	virtual void Free();

protected:
	map<const wstring, shared_ptr<class CComponent>> m_Components;

protected:
	HRESULT AddComponent(_uint _iLevelIndex, const wstring& _strProtoTag, const wstring& _strComTag, shared_ptr<CComponent>* _ppOut, shared_ptr<void> _pArg = nullptr);

private:
	bool m_IsActive = true; //true일시 Delete
	bool m_IsEnabled = true; //false일시 Update+Render 돌리지 않음

	EObjType m_eObjType = EObjType::OBJ_END;
	wstring m_strObjKey;

};

END