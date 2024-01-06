#pragma once

#include "Engine_Defines.h"
#include "Client_Defines.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "ImGuizmo.h"

BEGIN(Client)

class CImguiMgr
{
	DECLARE_SINGLETON(CImguiMgr)

public:
	enum EMouseState {MS_DEFAULT, MS_TERRAIN, MS_OBJECT, MS_END };

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

	void TerrainEditor();
	void PickingNavi();
	void ObjectLoader();
	void ImgZmoTest();


private:
	void SetObjectList();

private:
	void GetMouse();

private:
	wrl::ComPtr<ID3D11Device> m_pDevice;
	wrl::ComPtr<ID3D11DeviceContext> m_pContext;

private:
	EMouseState m_eMouseState = EMouseState::MS_DEFAULT;
	shared_ptr<class CTerrain> m_pCurrentTerrain = nullptr;

	_float3 m_PickingPos = _float3();
	_bool m_IsZmoUsing = false;

	vector<char*> m_ObjectList;

};

END

