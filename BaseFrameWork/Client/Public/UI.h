#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Renderer.h"

BEGIN(Engine)
class CShader;
class CVIBuffer_UI;
class CTexture;
END

BEGIN(Client)

class CUI abstract : public CGameObject
{
public:
	struct tagUIInfo {
		_float fX;
		_float fY;
		_float fSizeX;
		_float fSizeY;
	};

public:
	CUI();
	virtual ~CUI() = default;

public:
	virtual HRESULT Initialize(tagUIInfo _UIinfo, _int _iPriorityIndex);
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	shared_ptr<CShader> m_pShader = nullptr;
	shared_ptr<CVIBuffer_UI> m_VIRectCom = nullptr;
	shared_ptr<CTexture> m_pTextureCom = nullptr;

	CRenderer::UIGROUP m_eUIGroup = CRenderer::UIGROUP::UI_BACKGROUND;
protected:
	void AddBaseComponent();

protected:
	tagUIInfo m_pUIDefaultInfo;
	bool m_IsVisible = false;
	_int m_iPriorityIndex = 0;

};

END