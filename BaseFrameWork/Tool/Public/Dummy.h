#pragma once

#include "Tool_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Tool)

class CDummy final: public CGameObject
{
public:
	CDummy();
	virtual ~CDummy() = default;

public:
	virtual HRESULT Initialize(const wstring& _strKey);
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
	static shared_ptr<CDummy> Create(const wstring& _strKey);

};

END