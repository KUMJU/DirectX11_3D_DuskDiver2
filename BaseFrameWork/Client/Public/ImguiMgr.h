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
	void ObjectLoader();
	void SelectObjectInfo();
	void ImgZmoUpdate();

	void DrawNaviMesh();

	void SaveData();

private:
	void SetObjectList();

private:
	POINT GetMouse();
	void TransFloat4x4ToFloatArray(_float4x4 _baseMat, _float* _pOut);

private:
	wrl::ComPtr<ID3D11Device> m_pDevice;
	wrl::ComPtr<ID3D11DeviceContext> m_pContext;

private:

	/*ImguiZmo*/
	_bool m_IsZmoUsing = false;

	/*Object Select*/
	_int m_iCurrentLevelIdx = 0;
	_int m_ObjListIdx = 0;
	_int m_iCurrentObjIdx = -1;

	_char m_SaveFileName[256]= "";

public:
	void SetMapObject(shared_ptr<CGameObject> _pMapObj) { m_pMapMesh = _pMapObj;}

private:
	//현재 레벨에서 생성 가능한 오브젝트 리스트
	vector<char*> m_ObjectList;

	//둘이 한 세트
	//m_CurrentObjectList : 생성한 Object Index 관리 (편하게 listbox화 시키기 위해 별도 vector에 보관)
	//m_Objects : 실질적인 Object 객체를 보관하고 있는 리스트 , 위 벡터와 번호 동일
	vector<char*> m_CurrentObjectList;
	list<shared_ptr<CGameObject>> m_Objects;

	//현재 내가 선택하고 있는 오브젝트 -> imguizmo 사용하려고 따로 저장해둠
	shared_ptr<CGameObject> m_pCurrentSelectObj = nullptr;
	shared_ptr<CGameObject> m_pMapMesh = nullptr;

///////////SelectObjectInfo////////////
private:
	_float m_vObjectScale[3] = { 0.f , 0.f, 0.f };
	_float m_vObjectTranslate[3] = { 0.f , 0.f, 0.f };
	_float m_vObjectRotate[3] = {0.f , 0.f, 0.f};
	

/////////////Picking ////////

private:
	_bool m_KeyDeb = false;
	_float3 m_vCurrentPicking = _float3();

	_float m_fDebTime = 0.f;

////////ImguiZmo Option/////////
private:
	ImGuizmo::OPERATION m_CurrentGizmoOperation;
	ImGuizmo::MODE m_CurrentGizmoMode = ImGuizmo::WORLD;
};

END

