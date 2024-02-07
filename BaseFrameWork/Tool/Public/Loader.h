   #pragma once

#include "Tool_Defines.h"

BEGIN(Tool)

class CLoader final
{
public:

	CLoader();
	~CLoader();

public:
	HRESULT Initialize(LEVEL _eNextLevel);
	HRESULT LoadingSelector();
	_bool isFinished() const {
		return m_isFinished;
	}

#ifdef _DEBUG
public:
	void Output_LoadingText();
#endif

private:
	wstring m_strLoadingText = {};
	_bool m_isFinished = { false };

private:
	HANDLE m_hThread = { 0 };
	CRITICAL_SECTION m_CriticalSection = {};
	LEVEL m_eNextLevel = { LEVEL_END };

private:
	HRESULT LoadingForLogo();
	HRESULT LoadingForArcadeMap();
	HRESULT LoadingForEdit();

private:
	wrl::ComPtr<ID3D11Device> m_pDevice;
	wrl::ComPtr<ID3D11DeviceContext> m_pContext;

public:
	static CLoader* Create(LEVEL eNextLevel);
	void Free();

};

END
