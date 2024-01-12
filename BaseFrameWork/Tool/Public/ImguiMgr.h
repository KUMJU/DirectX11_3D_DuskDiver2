#pragma once

#include "Engine_Defines.h"
#include "Client_Defines.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "ImGuizmo.h"

#include "GameObject.h"

BEGIN(Client)

class CImguiMgr
{
	DECLARE_SINGLETON(CImguiMgr)
private:
	CImguiMgr();
	~CImguiMgr();

public:
	HRESULT Initialize();
	HRESULT StartFrame();

public:
	HRESULT Render(void);

public:
	void ShutDown();



private:
	wrl::ComPtr<ID3D11Device> m_pDevice;
	wrl::ComPtr<ID3D11DeviceContext> m_pContext;

};

END

