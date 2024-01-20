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
	//���� �������� ���� ������ ������Ʈ ����Ʈ
	vector<char*> m_ObjectList;

	//���� �� ��Ʈ
	//m_CurrentObjectList : ������ Object Index ���� (���ϰ� listboxȭ ��Ű�� ���� ���� vector�� ����)
	//m_Objects : �������� Object ��ü�� �����ϰ� �ִ� ����Ʈ , �� ���Ϳ� ��ȣ ����
	vector<char*> m_CurrentObjectList;
	list<shared_ptr<CGameObject>> m_Objects;

	//���� ���� �����ϰ� �ִ� ������Ʈ -> imguizmo ����Ϸ��� ���� �����ص�
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

