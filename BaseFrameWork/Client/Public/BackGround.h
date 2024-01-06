#pragma once

#include "Client_Defines.h"
#include "GameObject.h"


BEGIN(Engine)
class CShader;
class CVIRect;
class CTexture;
END

BEGIN(Client)

class CBackGround final : public CGameObject
{
public:
	CBackGround();
	CBackGround(const CBackGround& _rhs);
	virtual ~CBackGround() = default;

public:
	virtual void PreInitialize() override;
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	shared_ptr<CShader> m_pShaderCom = nullptr;
	shared_ptr<CTexture> m_pTextureCom = nullptr;
	shared_ptr<CVIRect> m_pVIRect = nullptr;


private:
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4			m_ViewMatrix, m_ProjMatrix;

private:
	HRESULT AddComponent();

public:
	static shared_ptr<CBackGround> Create();


};

END