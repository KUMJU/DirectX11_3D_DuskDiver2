#pragma once

#include "Engine_Defines.h"
#include "Transform.h"

BEGIN(Engine)

class CCollider;

class ENGINE_DLL CGameObject abstract : public std::enable_shared_from_this<CGameObject>
{
public:
	enum EObjType{ 
		OBJ_PLAYER, 
		OBJ_MONSTER, 
		OBJ_UI, 
		OBJ_ENV, 
		OBJ_PROJ, 
		OBJ_WALL, 
		OBJ_HOCKEYBALL,
		OBJ_END };

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
	void SetEnable(_bool _bState) { m_IsEnabled = _bState; }
	void SetPosition(_vector _vPos);

	EObjType GetObjectType() { return m_eObjType; }
public:
	_matrix GetWorldMatrix();
	//ImguiZmo 전용//
	_float* GetWoldMatFloatArr();
	void SetWorldMatrix(_float4x4 _worldMat);


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

protected:
	bool m_IsActive = true; //true일시 Delete
	bool m_IsEnabled = true; //false일시 Update+Render 돌리지 않음

	EObjType m_eObjType = EObjType::OBJ_END;
	wstring m_strObjKey;

	wstring strObjectKeyName = TEXT("");

public:
	//충돌이 필요한 객체들은 각 객체 내에서 재정의해서 사용한다 
	virtual void OnCollide(EObjType _eObjType, shared_ptr<CCollider> _pCollider) {};
		
};

END