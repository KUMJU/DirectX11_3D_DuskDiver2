#pragma once
#include "Tool_Defines.h"
#include "Engine_Defines.h"
#include "UI.h"

BEGIN(Tool)

class CUIBackGround final : public CUI
{
public:
	CUIBackGround();
	virtual ~CUIBackGround() = default;

public:
	virtual HRESULT Initialize(tagUIInfo _info, const wstring& _TextureKey, _int _iPriorityIndex);
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void SetSelectChange() { m_IsSelect = !m_IsSelect; }

private:
	shared_ptr<CTexture> m_pTextureCom = nullptr;

private:
	void AddComponent(const wstring& _TextureKey);

private:
	/*logo select Àü¿ë*/
	_bool m_IsSelect = false;

public:
	static shared_ptr<CUIBackGround> Create(tagUIInfo _info, const wstring& _TextureKey, _int _iPriorityIndex);
};

END
