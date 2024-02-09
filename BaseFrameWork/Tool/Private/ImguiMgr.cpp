#include "pch.h"
#include "ImguiMgr.h"
#include "GameInstance.h"
#include "NaviToolMgr.h"

#include "Terrain.h"
#include "Dummy.h"

#include "GameObject.h"

#include "Mesh.h"
#include "Model.h"

#include "EffectParticle.h"
#include "EffectTexture.h"

#include <fstream>

#include "Escalator.h"
#include "VIBufferInstancing.h"
#include "EffectPreset.h"

#include <io.h>

IMPLEMENT_SINGLETON(CImguiMgr)

static const float identityMatrix[16] =
{ 1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f };

CImguiMgr::CImguiMgr()
{
}

CImguiMgr::~CImguiMgr()
{
    ShutDown();
}

HRESULT CImguiMgr::Initialize()
{
    m_pDevice = CGameInstance::GetInstance()->GetDeviceInfo();
    m_pContext = CGameInstance::GetInstance()->GetDeviceContextInfo();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(g_hWnd);
    ImGui_ImplDX11_Init(m_pDevice.Get(), m_pContext.Get());

    //이펙트를 담을 프리셋 만들어두기

    m_pEffectPreset = CEffectPreset::Create();
    CGameInstance::GetInstance()->AddObject(LEVEL_EDIT, TEXT("Layer_Effect"), m_pEffectPreset);


    //메쉬& 텍스쳐 리스트 세팅
    SettingImageData();
    


    return S_OK;
}

HRESULT CImguiMgr::StartFrame()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    return S_OK;
}

HRESULT CImguiMgr::Render(void)
{

    if (m_KeyDeb) {
        m_fDebTime += 0.01f;
        
        if (m_fDebTime > 1.f) {
            m_KeyDeb = false;
            m_fDebTime = 0.f;
        }

    }


    StartFrame();

    EffectSetting();
    EffectListView();

   // ObjectLoader();

    //if(m_IsZmoUsing)
       // ImgZmoUpdate();


    //Reset
    if (-1 == m_iCurrentObjIdx) {
        m_vObjectScale[0] = 0.f;
        m_vObjectScale[1] = 0.f;
        m_vObjectScale[2] = 0.f;

        m_vObjectTranslate[0] = 0.f;
        m_vObjectTranslate[1] = 0.f;
        m_vObjectTranslate[2] = 0.f;

        m_vObjectRotate[0] = 0.f;
        m_vObjectRotate[1] = 0.f;
        m_vObjectRotate[2] = 0.f;

        m_pCurrentSelectObj = nullptr;
        m_IsZmoUsing = false;
    }

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return S_OK;
}

void CImguiMgr::ShutDown()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}


void CImguiMgr::EffectSetting() {
    ImGui::Begin("Effect Setting");

    ImGui::Text("Common Setting");

    BasicEffectSetting();


    //Effect 폴더 내에 있는 리스트 쭉 읽어오기 

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("Effect Type", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("Particle"))
        {
            m_iEffectType = 0;
            ParticleEffectSetting();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Texture"))
        {
            m_iEffectType = 1;
            TextureEffectSetting();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Mesh"))
        {
            m_iEffectType = 2;
            MeshEffectSetting();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::Separator();

    if (ImGui::Button("Create")) {

        CreateEffect();

    }ImGui::SameLine();

    if (ImGui::Button("Edit")) {

        //CreateEffect();

    }


    ImGui::End();

}

void CImguiMgr::EffectListView()
{
    //이펙트 리스트박스
    ImGui::Begin("Effect ListView");

    ImGui::ListBox("Object List", &m_iEffectSelectIdx, m_EffectNameList.data(), (_int)m_EffectNameList.size(), 5);

    if (ImGui::Button("Delete")) {
       
    } ImGui::SameLine();

    if (ImGui::Button("Clear")) {
        
    }

    if (ImGui::Button("Preset Play")) {

        m_pEffectPreset->PlayEffect();

    }

        
    ImGui::Text("Preset");
    ImGui::Separator();
    

    ImGui::InputText(" ", m_CurrentPresetName, sizeof(_char) * 256);
    ImGui::SameLine();

    if (ImGui::Button("Create Preset")) {
        CreatePreset();
    }

    //프리셋 리스트박스

    ImGui::ListBox("Preset List", &m_iPresetSelectIdx, m_PresetNameList.data(), (_int)m_PresetNameList.size(), 5);

    if (m_iPrevPresetIdx != m_iPresetSelectIdx) {
        m_iPrevPresetIdx = m_iPresetSelectIdx;

        if (m_iPresetSelectIdx != -1) {


            vector<shared_ptr<CEffect>> pList = *(m_Presets[m_iPresetSelectIdx]->GetEffectList());
            m_EffectNameList.clear();

            for (auto& iter : pList) {

                m_EffectNameList.push_back(iter->GetEffectName());

            }

        }


    }


    ImGui::End();

}

void CImguiMgr::BasicEffectSetting()
{
 
    ImGui::Checkbox("Loop", &m_IsLoop);
    ImGui::InputFloat2("Total Duration", &m_fTotalDuration.x);

    ImGui::InputText("Effect Name", m_CurrentName, sizeof(char) * 256);

    //추후에 Noise 텍스쳐랑 Base Diffuse랑 구분해두기 
    if (ImGui::BeginCombo("combo 1", m_ImagesList[m_iImageNum]))
    {
        for (int n = 0; n < m_ImagesList.size(); n++)
        {
            const bool is_selected = (m_iImageNum == n);
            if (ImGui::Selectable(m_ImagesList[n], is_selected))
                m_iImageNum = n;

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    //BaseTexture DropDown

    ImGui::Separator();


}

void CImguiMgr::ParticleEffectSetting()
{
    ImGui::Text("This is Paritcle Effect Setting");

    ImGui::InputInt("Instance Number", &m_iInstanceNum);

    ImGui::InputFloat3("Center", &m_vCenter.x);
    ImGui::InputFloat2("Size", &m_vSize.x); // x : MinSpeed y: MaxSpeed
    ImGui::InputFloat2("Speed", &m_vSpeed.x);
    ImGui::InputFloat4("Color", &m_vColor.x);
    ImGui::InputFloat3("Direction Min", &m_vDirMin.x);
    ImGui::InputFloat3("Direction Max", &m_vDirMax.x);
    ImGui::InputFloat3("Start Position Min", &m_vStartPosMin.x);
    ImGui::InputFloat3("Start Position Max", &m_vStartPosMax.x);
    ImGui::InputFloat2("Duration", &m_vDuration.x);

}

void CImguiMgr::TextureEffectSetting()
{
    ImGui::Text("This is Texture Effect Setting");

    ImGui::InputFloat3("Center", &m_vCenter.x);
    ImGui::InputFloat2("Size", &m_vSize.x); // x : MinSpeed y: MaxSpeed
    ImGui::InputFloat4("Color", &m_vColor.x);
    ImGui::InputFloat2("Rotation", &m_vRotation.x);
    ImGui::InputFloat2("Duration", &m_vDuration.x);

}

void CImguiMgr::MeshEffectSetting()
{
    ImGui::Text("This is Mesh Effect Setting");

}


void CImguiMgr::SettingImageData()
{
    _wfinddata_t fd;
    intptr_t lHandle;
    const wstring& strFullPath = m_strBasePath + TEXT("Texture/*.*");

    if (-1 == (lHandle = _wfindfirst(strFullPath.c_str(), &fd)))
        return;

    while (0 == _wfindnext(lHandle, &fd))
    {
        const wstring& strName = fd.name;

        if (IsFileOrDir(fd) && fd.size != 0 && fd.name[0] != '.') {
            wstring strName = fd.name;
            wstring strExt = EraseExt(strName);

            char* strMultiByte = new char[256];
            WideCharToMultiByte(CP_ACP, 0, strExt.c_str(), -1, strMultiByte, 256, NULL, NULL);
            m_ImagesList.push_back(const_cast<char*>(strMultiByte));
           
        }
    }

    _findclose(lHandle);

}


void CImguiMgr::SettingMeshData()
{
}

_bool CImguiMgr::IsFileOrDir(_wfinddata_t _fd)
{
    if (_fd.attrib & _A_SUBDIR)
        return false; //디렉토리
    else
        return true; //파일

}

wstring CImguiMgr::EraseExt(const wstring& _strFileName)
{
    size_t iCount = _strFileName.find_last_of(TEXT("."));
    return _strFileName.substr(0, iCount);
}


void CImguiMgr::CreateEffect()
{   

    if (0 == m_iEffectType) {
        CVIBufferInstancing::INSTANCE_DESC InstanceDesc = {};

        InstanceDesc.vCenter = m_vCenter;
        InstanceDesc.vSize = m_vSize;
        InstanceDesc.vSpeed = m_vSpeed;
        InstanceDesc.vLifeTime = m_fTotalDuration;
        InstanceDesc.isLoop = m_IsLoop;
        InstanceDesc.vColor = m_vColor;
        InstanceDesc.vDirectionMin = m_vDirMin;
        InstanceDesc.vDirectionMax = m_vDirMax;
        InstanceDesc.vStartPointMin = m_vStartPosMin;
        InstanceDesc.vStartPointMax = m_vStartPosMax;
        InstanceDesc.vDuration = m_vDuration;

        char* EffectName = new char[256];
        memcpy_s(EffectName, sizeof(char) * 256, m_CurrentName, sizeof(char) * 256);

        _tchar szFullPath[MAX_PATH] = TEXT("");
        MultiByteToWideChar(CP_ACP, 0, m_ImagesList[m_iImageNum], (_int)strlen(m_ImagesList[m_iImageNum]), szFullPath, MAX_PATH);

        shared_ptr<CEffectParticle> pParticle = CEffectParticle::Create(m_iInstanceNum, szFullPath, &InstanceDesc, EffectName);

        m_EffectNameList.push_back(EffectName);
        m_pEffectPreset->AddEffect(pParticle);

    }
    else if (1 == m_iEffectType) {


        CEffectTexture::TEXEFFECT_DESC textureDesc = {};

        textureDesc.vCenter = m_vCenter;
        textureDesc.vColor = m_vColor;
        textureDesc.vRotation = m_vRotation;
        textureDesc.vScale = m_vSize;

        char* EffectName = new char[256];
        memcpy_s(EffectName, sizeof(char) * 256, m_CurrentName, sizeof(char) * 256);

        _tchar szFullPath[MAX_PATH] = TEXT("");
        MultiByteToWideChar(CP_ACP, 0, m_ImagesList[m_iImageNum], (_int)strlen(m_ImagesList[m_iImageNum]), szFullPath, MAX_PATH);

        shared_ptr<CEffectTexture> pParticle = CEffectTexture::Create(szFullPath, &textureDesc, EffectName);

        m_EffectNameList.push_back(EffectName);
        m_pEffectPreset->AddEffect(pParticle);
    }
    else if (2 == m_iEffectType) {



    }

  

}

void CImguiMgr::CreatePreset()
{

    shared_ptr<CEffectPreset> pPreset = CEffectPreset::Clone(m_pEffectPreset);
    m_Presets.push_back(pPreset);

    char* PresetName = new char[256];
    memcpy_s(PresetName, sizeof(char) * 256, m_CurrentPresetName, sizeof(char) * 256);

    m_PresetNameList.push_back(PresetName);


}


void CImguiMgr::TransFloat4x4ToFloatArray(_float4x4 _baseMat ,  _float* _pOut)
{
    _float temp[16] = { _baseMat._11, _baseMat._12, _baseMat._13, _baseMat._14,
                        _baseMat._21, _baseMat._22, _baseMat._23, _baseMat._24,
                        _baseMat._31, _baseMat._32, _baseMat._33, _baseMat._34,
                        _baseMat._41, _baseMat._42, _baseMat._43, _baseMat._44 };
    
    memcpy(_pOut, temp, sizeof(temp));
}

