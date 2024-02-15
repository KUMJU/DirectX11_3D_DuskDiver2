#pragma once

#include "Engine_Defines.h"
#include "Tool_Defines.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "ImGuizmo.h"

#include "GameObject.h"


BEGIN(Engine)
class CModel;
class CAnimation;
END

BEGIN(Tool)

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

	void SaveData();

private:
	void SetObjectList();

private:
	//����Ʈ ������
	void EffectSetting();
	//����Ʈ ����Ʈ�� + ���� �� ���� 
	void EffectListView();

private:

	void BasicEffectSetting();
	
	void ParticleEffectSetting();
	void TextureEffectSetting();
	void MeshEffectSetting();

	void CreateEffect();
	void CreatePreset();

	void EditEffect();

private:
	/**���� ����Ʈ �̸�**/
	vector<char*> m_EffectNameList;
	//Current ViewPreset
	shared_ptr<class CEffectPreset> m_pEffectPreset = nullptr;

	//�����״� Preset ���帮��Ʈ 
	vector<shared_ptr<class CEffectPreset>> m_Presets;
	vector<char*> m_PresetNameList;

	_int m_iEffectSelectIdx = -1;
	_int m_iPrevEffectIdx = -1;

	_int m_iPresetSelectIdx = -1;
	_int m_iPrevPresetIdx = -1;

	_char m_CurrentName[256] = "";
	_char m_CurrentPresetName[256] = "";

//Effect Save Parameter//
private:
	// 0 : Particle 1: Texture 2:Mesh
	_int m_iEffectType = 0;

	_bool m_IsLoop = false;
	_float2 m_fTotalDuration = _float2(0.f, 0.f);

	//Texture Name 
	_uint m_iTextureNum = 0;

	_bool m_bGlow = false;
	_float4 m_vGlowColor = _float4(0.f, 0.f, 0.f, 1.f);

	/*Particle*/
	_int m_iInstanceNum = 0;

	_float3 m_vPivot = _float3();
	_float3 m_vCenter = _float3();
	_float3 m_vRange = _float3();
	_float2 m_vSize = _float2();
	_float2 m_vSpeed = _float2();
	_float4 m_vColor = _float4(0.f, 0.f, 0.f, 1.f);
	_float3 m_vDirMin = _float3();
	_float3 m_vDirMax = _float3();
	_float3 m_vStartPosMin = _float3();
	_float3 m_vStartPosMax = _float3();
	_float2 m_vDuration = _float2();


	_float m_fScaleChangeTime = 0.f;
//Texture
private:
	_float3 m_vRotation = _float3();
	_float m_fTurnSpeed = _float();
	_float4 m_vAxis = _float4(0.f, 0.f, 0.f, 0.f);

	_float2 m_vTexStartScale = _float2();
	_float2 m_vTexMiddleScale = _float2();
	_float2 m_vTexEndScale = _float2();


//Mesh 
private:
	_float3 m_vScale = _float3();
	_float3 m_vMiddleScale = _float3();
	_float3 m_vEndScale = _float3();
	_float4 m_vLerpColor = _float4(0.f, 0.f, 0.f, 1.f);


	_bool m_IsMaskTex = false;
	_bool m_IsNoiseTex = false;

	_bool m_bMaskUVLoop = false;

	_float m_fMaskSpeed = _float();
	_float m_fNoiseSpeed = _float();
	
	_float2 m_vMaskDirection = _float2();
	_float2 m_vNoiseDirection = _float2();

private:
	const wstring& m_strBasePath = TEXT("../../Client/Bin/Resources/Base/Effect/");
	const wstring& m_strSavePath = TEXT("../../Client/Bin/DataFiles/EffectData/");

	//���� findData�ؼ� ���� �̸��� ��������


//-----------����Ʈ �޽� & ����Ʈ �̹��� �̸����� ����Ʈ �� -----------
private:
	
	vector<char*> m_ImagesList;
	vector<char*> m_NoiseList;
	vector<char*> m_MaskList;
	vector<char*> m_MeshesList;

	_int m_iImageNum = 0;
	_int m_iMeshNum = 0;
	_int m_iMaskNum = 0;
	_int m_iNoiseNum = 0;

private:

	//����Ʈ�ڽ��� ���� 
	void SettingImageData();
	void SettingMaskData();
	void SettingNoiseData();
	void SettingMeshData();

	void LoadPreset();

	_bool IsFileOrDir(_wfinddata_t _fd);
	wstring EraseExt(const wstring& _strFileName);

//-----------------------------------------------------------------------

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

	shared_ptr<CTransform> m_pMapTransform = nullptr;


///////////SelectObjectInfo////////////
private:
	_float m_vObjectScale[3] = { 0.f , 0.f, 0.f };
	_float m_vObjectTranslate[3] = { 0.f , 0.f, 0.f };
	_float m_vObjectRotate[3] = {0.f , 0.f, 0.f};
	

/////////////Picking ////////

	vector<shared_ptr<CGameObject>> m_PickingModelList;

private:
	_bool m_KeyDeb = false;
	_float3 m_vCurrentPicking = _float3();

	_float m_fDebTime = 0.f;

////////ImguiZmo Option/////////
private:
	ImGuizmo::OPERATION m_CurrentGizmoOperation;
	ImGuizmo::MODE m_CurrentGizmoMode = ImGuizmo::WORLD;

private:
	void TestModelTool();


private:

	_int m_iAnimNum = 1;

	shared_ptr<class CEffectTestModel> m_pTestModel = nullptr;
	shared_ptr<CModel> m_pPlrModel = nullptr;

	vector<shared_ptr<CAnimation>> m_pAnimList;

};

END

