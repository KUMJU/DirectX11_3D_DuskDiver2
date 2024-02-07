#pragma once

#include "Tool_Defines.h"
#include "Level.h"

BEGIN(Tool)

class CLogo final: public CLevel
{
public:
	CLogo();
	virtual ~CLogo();

public:
	virtual void PreInit();
	virtual HRESULT Initialize();
	virtual void Tick(_float _fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Layer_BackGround(const wstring& strLayerTag);
	void KeyInput();

private:
	_bool m_bKeyDeb = false;
	_float m_fKeyDebTime = 0.f;

	_int m_iSelectIndex = 0;

private:
	shared_ptr<class CUIBackGround> m_pFirstMenu = nullptr;
	shared_ptr<class CUIBackGround> m_pSecondMenu = nullptr;

public:
	static shared_ptr<CLogo> Create();
};

END