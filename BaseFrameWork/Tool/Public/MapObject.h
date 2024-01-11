#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CMapObject : public CGameObject
{
public:
	CMapObject();
	virtual ~CMapObject() = default;

public:
	virtual HRESULT Initialize(const wstring& _ModelKey, CTransform::TRANSFORM_DESC* _pDesc);
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	shared_ptr<CShader> m_pShader = nullptr;
	shared_ptr<CModel> m_pModelCom = nullptr;

private:
	HRESULT AddComponent();
	HRESULT BindShaderResources();

public:
	static shared_ptr<CMapObject> Create(const wstring& _ModelKey, CTransform::TRANSFORM_DESC* _pDesc = nullptr);

};

END