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

#include <fstream>

#include "Escalator.h"
#include "VIBufferInstancing.h"
#include "EffectPreset.h"

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

    //리스트 세팅
    //SetObjectList();

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
    ImGui::Begin("Effect ListView");

    ImGui::ListBox("Object List", &m_iEffectSelectIdx, m_EffectNameList.data(), (_int)m_EffectNameList.size(), 10);

    if (ImGui::Button("Delete")) {

    }




    ImGui::End();

}

void CImguiMgr::BasicEffectSetting()
{
 
    ImGui::Checkbox("Loop", &m_IsLoop);
    ImGui::InputFloat2("Total Duration", &m_fTotalDuration.x);

    ImGui::InputText("Effect Name", m_CurrentName, sizeof(char) * 256);

    //BaseTexture DropDown

    ImGui::Separator();


}

void CImguiMgr::ParticleEffectSetting()
{
    ImGui::Text("This is Paritcle Effect Setting");

    ImGui::InputInt("Instance Number", &m_iInstanceNum);

    ImGui::InputFloat3("Pivot", &m_vPivot.x);
    ImGui::InputFloat3("Center", &m_vCenter.x);
    ImGui::InputFloat3("Range", &m_vRange.x); // x: Start y:End
    ImGui::InputFloat2("Size", &m_vSize.x); // x : MinSpeed y: MaxSpeed
    ImGui::InputFloat2("Speed", &m_vSpeed.x);
    ImGui::InputFloat4("Color", &m_vColor.x);
    ImGui::InputFloat3("Direction", &m_fDir.x);
    ImGui::Checkbox("Start CenterPoint", &m_bStartOnePoint);

}

void CImguiMgr::TextureEffectSetting()
{
    ImGui::Text("This is Texture Effect Setting");

}

void CImguiMgr::MeshEffectSetting()
{
    ImGui::Text("This is Mesh Effect Setting");

}

void CImguiMgr::CreateEffect()
{
    /*Test*/

        //CVIBufferPoint::INSTANCE_DESC			InstanceDesc{};
    //InstanceDesc.vPivot = _float3(0.f, 9.f, 0.f);
    //InstanceDesc.vCenter = _float3(0.0f, 50.f, 0.f);
    //InstanceDesc.vRange = _float3(300.0f, 5.0f, 300.0f);
    //InstanceDesc.vSize = _float2(0.2f, 0.7f);
    //InstanceDesc.vSpeed = _float2(1.f, 3.f);
    //InstanceDesc.vLifeTime = _float2(20.f, 50.f);
    //InstanceDesc.isLoop = true;
    //InstanceDesc.vColor = _float4(1.f, 1.f, 1.f, 1.f);

    
    CVIBufferInstancing::INSTANCE_DESC InstanceDesc = {};
 
    InstanceDesc.vPivot = m_vPivot;
    InstanceDesc.vCenter = m_vCenter;
    InstanceDesc.vRange = m_vRange;
    InstanceDesc.vSize = m_vSize;
    InstanceDesc.vSpeed = m_vSpeed;
    InstanceDesc.vLifeTime = m_fTotalDuration;
    InstanceDesc.isLoop = m_IsLoop;
    InstanceDesc.vColor = m_vColor;
    InstanceDesc.fDirection = m_fDir;
    InstanceDesc.bStartSamePoint = m_bStartOnePoint;

    shared_ptr<CEffectParticle> pParticle = CEffectParticle::Create(m_iInstanceNum,TEXT("Snow"), &InstanceDesc);
   // CGameInstance::GetInstance()->AddObject(LEVEL_EDIT, TEXT("Layer_Effect"), pParticle);

    char* EffectName = new char[256];
    memcpy_s(EffectName, sizeof(char) * 256, m_CurrentName, sizeof(char) * 256);

    m_EffectNameList.push_back(EffectName);
    m_pEffectPreset->AddEffect(pParticle);

}


void CImguiMgr::TransFloat4x4ToFloatArray(_float4x4 _baseMat ,  _float* _pOut)
{
    _float temp[16] = { _baseMat._11, _baseMat._12, _baseMat._13, _baseMat._14,
                        _baseMat._21, _baseMat._22, _baseMat._23, _baseMat._24,
                        _baseMat._31, _baseMat._32, _baseMat._33, _baseMat._34,
                        _baseMat._41, _baseMat._42, _baseMat._43, _baseMat._44 };
    
    memcpy(_pOut, temp, sizeof(temp));
}

