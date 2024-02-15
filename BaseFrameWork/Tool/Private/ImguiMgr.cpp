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
#include "EffectMesh.h"

#include <fstream>

#include "Escalator.h"
#include "VIBufferInstancing.h"
#include "EffectPreset.h"

#include <io.h>


#include "Json/json.h"
#include "Json/json-forwards.h"

#include "EffectTestModel.h"
#include "Animation.h"

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

    m_pTestModel = CEffectTestModel::Create();
    CGameInstance::GetInstance()->AddObject(LEVEL_EDIT, TEXT("Layer_Player"), m_pTestModel);

    m_pPlrModel = m_pTestModel->GetModel();
    m_pAnimList = m_pPlrModel->GetAnimations();

    //메쉬& 텍스쳐 리스트 세팅
    SettingImageData();
    SettingMaskData();
    SettingNoiseData();
    SettingMeshData();


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

    TestModelTool();
    EffectSetting();
    EffectListView();

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
        m_pTestModel->ChangeAnimation(m_iAnimNum);


    }ImGui::SameLine();

    if (ImGui::Button("Edit")) {

        EditEffect();
    }


    ImGui::End();

}

void CImguiMgr::EffectListView()
{
    //이펙트 리스트박스
    ImGui::Begin("Effect ListView");
     
    ImGui::ListBox("Object List", &m_iEffectSelectIdx, m_EffectNameList.data(), (_int)m_EffectNameList.size(), 5);

    if (ImGui::Button("Delete")) {
       
        if (m_iEffectSelectIdx == -1 || m_EffectNameList.empty())
            return;

        m_pEffectPreset->DeleteEffect(m_iEffectSelectIdx);

        auto iter = m_EffectNameList.begin();

        for (size_t i = 0; i < m_iEffectSelectIdx; ++i) {
            ++iter;
        }

        m_EffectNameList.erase(iter);
        m_iEffectSelectIdx = 0;




    } ImGui::SameLine();

    if (ImGui::Button("Clear")) {
        
    }

    if (ImGui::Button("Preset Play")) {

        m_pEffectPreset->PlayEffect();   
        m_pTestModel->ChangeAnimation(m_iAnimNum);

    }

    if (ImGui::Button("Preset Stop")) {

        m_pEffectPreset->StopEffect();

    }

        
    ImGui::Text("Preset");
    ImGui::Separator();
    

    ImGui::InputText(" ", m_CurrentPresetName, sizeof(_char) * 256);
    ImGui::SameLine();

    if (ImGui::Button("Save Preset")) {
        CreatePreset();
    }

    if (ImGui::Button("Load Preset")) {
        LoadPreset();
    }

    //프리셋 Json 리스트 

    ImGui::ListBox("Preset List", &m_iPresetSelectIdx, m_PresetNameList.data(), (_int)m_PresetNameList.size(), 5);

    if (m_iPrevPresetIdx != m_iPresetSelectIdx) {
        m_iPrevPresetIdx = m_iPresetSelectIdx;

        if (m_iPresetSelectIdx != -1) {


            //vector<shared_ptr<CEffect>> pList = *(m_Presets[m_iPresetSelectIdx]->GetEffectList());
            //m_EffectNameList.clear();

            //for (auto& iter : pList) {

            //    m_EffectNameList.push_back(iter->GetEffectName());

            //}
        }
    }

    ImGui::End();

}

void CImguiMgr::TestModelTool()
{
    ImGui::Begin("Model Tool");

    ImGui::InputInt("Anim Num", &m_iAnimNum);

    if(ImGui::Button("Change Anim")) {
        m_pTestModel->ChangeAnimation(m_iAnimNum);
    }

    _int TrackPos = (_int)(m_pAnimList[m_iAnimNum]->GetCurrentTrackPosition());
    _int Duration = (_int)(m_pAnimList[m_iAnimNum]->GetAnimDuration());


    m_fTotalDuration.y = Duration;


    ImGui::SliderInt("Lines", &TrackPos, 0, Duration);

    ImGui::End();


}


void CImguiMgr::BasicEffectSetting()
{
 
    ImGui::Checkbox("Loop", &m_IsLoop);
    ImGui::InputFloat2("Total Duration", &m_fTotalDuration.x);

    ImGui::InputText("Effect Name", m_CurrentName, sizeof(char) * 256);

    //추후에 Noise 텍스쳐랑 Base Diffuse랑 구분해두기 
    if (ImGui::BeginCombo("Image List", m_ImagesList[m_iImageNum]))
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

    ImGui::ColorEdit4("Color", &m_vColor.x);

    ImGui::SeparatorText("Direction");

    ImGui::InputFloat3("Direction Min", &m_vDirMin.x);
    ImGui::InputFloat3("Direction Max", &m_vDirMax.x);

    ImGui::SeparatorText("Start Position");

    ImGui::InputFloat3("Start Position Min", &m_vStartPosMin.x);
    ImGui::InputFloat3("Start Position Max", &m_vStartPosMax.x);
    ImGui::InputFloat2("Duration", &m_vDuration.x);

    ImGui::SeparatorText("Glow");
    ImGui::Checkbox("Using Glow", &m_bGlow);
    ImGui::ColorEdit4("Glow Color", &m_vGlowColor.x);

    ImGui::SeparatorText("Alpha");
    //Alpha Lerp

}

void CImguiMgr::TextureEffectSetting()
{

    ImGui::InputFloat3("Center", &m_vCenter.x);
    ImGui::ColorEdit4("Color", &m_vColor.x);
    ImGui::InputFloat2("Duration", &m_vDuration.x);

    ImGui::SeparatorText("Scale Info");

    ImGui::InputFloat2("Start Scale", &m_vTexStartScale.x); 
    ImGui::InputFloat2("Middle Scale", &m_vTexMiddleScale.x);
    ImGui::InputFloat2("End Scale", &m_vTexEndScale.x); 
    ImGui::InputFloat("Change Time", &m_fScaleChangeTime);


    ImGui::SeparatorText("Spin Info");

    ImGui::InputFloat3("Rotation", &m_vRotation.x);

    ImGui::InputFloat("Turn Speed", &m_fTurnSpeed);
    ImGui::InputFloat3("Turn Axis", &m_vAxis.x);

}

void CImguiMgr::MeshEffectSetting()
{
    ImGui::InputFloat2("Life Time", &m_vDuration.x);

    if (ImGui::BeginCombo("Mesh List", m_MeshesList[m_iMeshNum]))
    {
        for (int n = 0; n < m_MeshesList.size(); n++)
        {
            const bool is_selected = (m_iMeshNum == n);
            if (ImGui::Selectable(m_MeshesList[n], is_selected))
                m_iMeshNum = n;

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }


    ImGui::InputFloat3("Start Scale", &m_vScale.x);
    ImGui::InputFloat3("Middle Scale", &m_vMiddleScale.x);
    ImGui::InputFloat3("End Scale", &m_vEndScale.x);
    ImGui::InputFloat("Change Time", &m_fScaleChangeTime);

    ImGui::InputFloat3("Center", &m_vCenter.x);

    ImGui::ColorEdit4("Color", &m_vColor.x);
    ImGui::ColorEdit4("Color Lerp", &m_vLerpColor.x);

    ImGui::SeparatorText("Mask Texture");

    ImGui::Checkbox("Using MaskTexture", &m_IsMaskTex);

    ImGui::Checkbox("Mask UV Loop", &m_bMaskUVLoop);

    if (ImGui::BeginCombo("Mask Texture List", m_MaskList[m_iMaskNum]))
    {
        for (int n = 0; n < m_MaskList.size(); n++)
        {
            const bool is_selected = (m_iMaskNum == n);
            if (ImGui::Selectable(m_MaskList[n], is_selected))
                m_iMaskNum = n;

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::InputFloat2("MaskTex Direction", &m_vMaskDirection.x);
    ImGui::InputFloat("Mask UV Speed", &m_fMaskSpeed);


    ImGui::SeparatorText("Noise Texture");

    ImGui::Checkbox("Using NoiseTexture", &m_IsNoiseTex);

    if (ImGui::BeginCombo("Noise Texture List", m_NoiseList[m_iNoiseNum]))
    {
        for (int n = 0; n < m_NoiseList.size(); n++)
        {
            const bool is_selected = (m_iNoiseNum == n);
            if (ImGui::Selectable(m_NoiseList[n], is_selected))
                m_iNoiseNum = n;

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }


    ImGui::InputFloat2("NoiseTex Direction", &m_vNoiseDirection.x);
    ImGui::InputFloat("Noise UV Speed", &m_fNoiseSpeed);

}


void CImguiMgr::SettingImageData()
{
    _wfinddata_t fd;
    intptr_t lHandle;
    const wstring& strFullPath = m_strBasePath + TEXT("Diffuse/*.*");

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

void CImguiMgr::SettingMaskData()
{
    _wfinddata_t fd;
    intptr_t lHandle;
    const wstring& strFullPath = m_strBasePath + TEXT("Mask/*.*");

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
            m_MaskList.push_back(const_cast<char*>(strMultiByte));

        }
    }

    _findclose(lHandle);
}

void CImguiMgr::SettingNoiseData()
{
    _wfinddata_t fd;
    intptr_t lHandle;
    const wstring& strFullPath = m_strBasePath + TEXT("Noise/*.*");

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
            m_NoiseList.push_back(const_cast<char*>(strMultiByte));

        }
    }

    _findclose(lHandle);

}


void CImguiMgr::SettingMeshData()
{
    _wfinddata_t fd;
    intptr_t lHandle;
    const wstring& BasePath = TEXT("../../Client/Bin/Resources/Base/Models/NonAnim/Effect/");
    const wstring& strFullPath = BasePath + TEXT("*.*");

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
            m_MeshesList.push_back(const_cast<char*>(strMultiByte));

        }
    }

    _findclose(lHandle);



}

void CImguiMgr::LoadPreset()
{
    Json::Value root;

    const wstring& strFullPath = m_strSavePath;
    const wstring& strExt = TEXT(".json");
    
    _tchar szFullPath[MAX_PATH] = TEXT("");
    MultiByteToWideChar(CP_ACP, 0, m_PresetNameList[m_iPresetSelectIdx], (_int)strlen(m_CurrentPresetName), szFullPath, MAX_PATH);

    const wstring& strCompletePath = strFullPath + szFullPath + strExt;
    ifstream in(strCompletePath);

    in >> root;

    _int iEffectNum = root.size();

    Json::Value PresetInfo = root["Preset Info"];

    Json::Value Element = root["Elements"];
    Json::ValueIterator iter = Element.begin();

    m_EffectNameList.clear();

    for (_int i = 0; i < iEffectNum; ++i) {

        string KeyName;
        KeyName = iter.key().asString();

        _int iEffectType = Element[KeyName]["EffectType"].asInt();
       
        Json::Value Elements = Element[KeyName];

        shared_ptr<CEffectMesh> pEffect;


        if(0 == iEffectType){

            //Particle
        }
        else if(1== iEffectType){

            //Texture
            CEffectTexture::TEXEFFECT_DESC textureDesc = {};

            textureDesc.vCenter = { Elements["Center"]["x"].asFloat(),
            Elements["Center"]["y"].asFloat(),
            Elements["Center"]["z"].asFloat() };

            textureDesc.vStartScale = { Elements["StartScale"]["x"].asFloat(),
           Elements["StartScale"]["y"].asFloat() };

            textureDesc.vMiddleScale = { Elements["MiddleScale"]["x"].asFloat(),
            Elements["MiddleScale"]["y"].asFloat()};

            textureDesc.vEndScale = { Elements["EndScale"]["x"].asFloat(),
            Elements["EndScale"]["y"].asFloat()};


            textureDesc.vRotation = { Elements["Rotation"]["x"].asFloat(),
                                      Elements["Rotation"]["y"].asFloat(),
                                      Elements["Rotation"]["z"].asFloat() };


            textureDesc.vColor = { Elements["Color"]["x"].asFloat(),
            Elements["Color"]["y"].asFloat(),
            Elements["Color"]["z"].asFloat(),
            Elements["Color"]["w"].asFloat() };



            textureDesc.vDuration = { Elements["Duration"]["x"].asFloat(),
                                      Elements["Duration"]["y"].asFloat() };

            textureDesc.fTurnSpeed = Elements["Speed"].asFloat();

            textureDesc.vTurnAxis = { Elements["Axis"]["x"].asFloat(),
                                      Elements["Axis"]["y"].asFloat(),
                                      Elements["Axis"]["z"].asFloat(),
                                      0.f };

            textureDesc.fScaleChangeTime = Elements["ScaleChageTime"].asFloat();


            _tchar szFullPath[MAX_PATH] = TEXT("");
            MultiByteToWideChar(CP_ACP, 0, Elements["TextureKey"].asString().c_str(), (_int)strlen(Elements["TextureKey"].asString().c_str()), szFullPath, MAX_PATH);

            shared_ptr<CEffectTexture> pEffect = CEffectTexture::Create(szFullPath, &textureDesc, const_cast<char*>(KeyName.c_str()));
            m_pEffectPreset->AddEffect(pEffect);


        }
        else if (2 == iEffectType) {
            //Mesh
            CEffectMesh::MESH_DESC desc = {};

            desc.vCenter = { Elements["Center"]["x"].asFloat(),
            Elements["Center"]["y"].asFloat(),
            Elements["Center"]["z"].asFloat() };

            desc.vStartScale = { Elements["StartScale"]["x"].asFloat(),
            Elements["StartScale"]["y"].asFloat(),
            Elements["StartScale"]["z"].asFloat() };

            desc.vMiddleScale = { Elements["MiddleScale"]["x"].asFloat(),
            Elements["MiddleScale"]["y"].asFloat(),
            Elements["MiddleScale"]["z"].asFloat() };

            desc.vEndScale = { Elements["EndScale"]["x"].asFloat(),
            Elements["EndScale"]["y"].asFloat(),
            Elements["EndScale"]["z"].asFloat() };

            desc.bUVLoop = Elements["Loop"]["UVLoop"].asBool();
            desc.bLoop = Elements["Loop"]["EffectLoop"].asBool();

            desc.vColor = { Elements["Color"]["x"].asFloat(),
            Elements["Color"]["y"].asFloat(),
            Elements["Color"]["z"].asFloat(),
            1.f};

            desc.vDuration = { Elements["Duration"]["x"].asFloat(),
            Elements["Duration"]["y"].asFloat() };

            desc.fScaleChangeTime = Elements["ChangeTime"]["ChangeTime"].asFloat();

            string strMaskPath = Elements["Mask"]["TexKey"].asString();
            string strNoisePath = Elements["Noise"]["TexKey"].asString();

            string strMeshName = Elements["MeshKey"].asString();

            _tchar szNoiseTexName[MAX_PATH] = TEXT("");
            MultiByteToWideChar(CP_ACP, 0, strNoisePath.c_str(), (_int)strlen(strNoisePath.c_str()), szNoiseTexName, MAX_PATH);

            _tchar szMeshName[MAX_PATH] = TEXT("");
            MultiByteToWideChar(CP_ACP, 0, strMeshName.c_str(), (_int)strlen(strMeshName.c_str()), szMeshName, MAX_PATH);

            desc.szMaskTexKey = new char[256];
            desc.szNoiseTexKey = new char[256];

            memcpy_s(desc.szMaskTexKey, sizeof(char) * 256, strMaskPath.c_str() , sizeof(char) * 256);
            memcpy_s(desc.szNoiseTexKey, sizeof(char) * 256, strNoisePath.c_str(), sizeof(char) * 256);

            desc.bMask = Elements["Mask"]["Using"].asBool();
            desc.bNoise = Elements["Noise"]["Using"].asBool();

            desc.vMaskDir = { Elements["Mask"]["x"].asFloat(), Elements["Mask"]["y"].asFloat() };
            desc.vNoiseDir = { Elements["Noise"]["x"].asFloat(), Elements["Noise"]["y"].asFloat() };
            desc.fMaskSpeed = Elements["Mask"]["Speed"].asFloat();
            desc.fNoiseSpeed = Elements["Noise"]["Speed"].asFloat();
            desc.vLerpColor = { Elements["LerpColor"]["x"].asFloat(),
            Elements["LerpColor"]["y"].asFloat(),
            Elements["LerpColor"]["z"].asFloat(),
            1.f };

            pEffect = CEffectMesh::Create(TEXT(""), szMeshName, &desc, const_cast<char*>(KeyName.c_str()));
            m_pEffectPreset->AddEffect(pEffect);
        }



        char* szName = new char[256];
        memcpy_s(szName, sizeof(char) * 256, KeyName.c_str(), sizeof(char) * 256);
        m_EffectNameList.push_back(szName);

        ++iter;
    }
    
    m_pEffectPreset->SetLoop(Element["Loop"].asBool());
    m_pEffectPreset->SetDuration(Element["Duration"].asFloat());


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

    m_pEffectPreset->SetLoop(m_IsLoop);
    m_pEffectPreset->SetDuration(m_fTotalDuration.y);

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
        InstanceDesc.bGlow = m_bGlow;
        InstanceDesc.vGlowColor = m_vGlowColor;

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
        textureDesc.vStartScale = m_vTexStartScale;
        textureDesc.vMiddleScale = m_vTexMiddleScale;
        textureDesc.vEndScale = m_vTexEndScale;
        textureDesc.vDuration = m_vDuration;

        textureDesc.fTurnSpeed = m_fTurnSpeed;
        textureDesc.vTurnAxis = m_vAxis;

        textureDesc.fScaleChangeTime = m_fScaleChangeTime;

        char* EffectName = new char[256];
        memcpy_s(EffectName, sizeof(char) * 256, m_CurrentName, sizeof(char) * 256);

        _tchar szFullPath[MAX_PATH] = TEXT("");
        MultiByteToWideChar(CP_ACP, 0, m_ImagesList[m_iImageNum], (_int)strlen(m_ImagesList[m_iImageNum]), szFullPath, MAX_PATH);

        shared_ptr<CEffectTexture> pParticle = CEffectTexture::Create(szFullPath, &textureDesc, EffectName);

        m_EffectNameList.push_back(EffectName);
        m_pEffectPreset->AddEffect(pParticle);
    }
    else if (2 == m_iEffectType) {

        CEffectMesh::MESH_DESC desc = {};

        desc.vCenter = m_vCenter;
        desc.vStartScale = m_vScale;
        desc.vMiddleScale = m_vMiddleScale;
        desc.vEndScale = m_vEndScale;

        desc.bUVLoop = m_bMaskUVLoop;
        desc.bLoop = m_IsLoop;

        desc.vColor = m_vColor;
        desc.vDuration = m_vDuration;

        desc.fScaleChangeTime = m_fScaleChangeTime;

        char* EffectName = new char[256];
        memcpy_s(EffectName, sizeof(char) * 256, m_CurrentName, sizeof(char) * 256);

        _tchar szFullPath[MAX_PATH] = TEXT("");
        MultiByteToWideChar(CP_ACP, 0, m_ImagesList[m_iImageNum], (_int)strlen(m_ImagesList[m_iImageNum]), szFullPath, MAX_PATH);

        _tchar szMeshName[MAX_PATH] = TEXT("");
        MultiByteToWideChar(CP_ACP, 0, m_MeshesList[m_iMeshNum], (_int)strlen(m_MeshesList[m_iMeshNum]), szMeshName, MAX_PATH);

        desc.szMaskTexKey = new char[256];
        desc.szNoiseTexKey = new char[256];

        memcpy_s(desc.szMaskTexKey, sizeof(char) * 256, m_MaskList[m_iMaskNum], sizeof(char) * 256);
        memcpy_s(desc.szNoiseTexKey, sizeof(char) * 256, m_NoiseList[m_iNoiseNum], sizeof(char) * 256);

        desc.bMask = m_IsMaskTex;
        desc.bNoise = m_IsNoiseTex;

        desc.vMaskDir = m_vMaskDirection;
        desc.vNoiseDir = m_vNoiseDirection;
        desc.fMaskSpeed = m_fMaskSpeed;
        desc.fNoiseSpeed = m_fNoiseSpeed;
        desc.vLerpColor = m_vLerpColor;
     

        shared_ptr<CEffectMesh> pParticle = CEffectMesh::Create(szFullPath, szMeshName, &desc, EffectName);


        m_EffectNameList.push_back(EffectName);
        m_pEffectPreset->AddEffect(pParticle);


    }

    m_iEffectSelectIdx = m_EffectNameList.size() - 1;
    m_pEffectPreset->PlayEffect();
  

}

void CImguiMgr::CreatePreset()
{
    const wstring& strFullPath = m_strSavePath;
    const wstring& strExt = TEXT(".json");

    _tchar szFullPath[MAX_PATH] = TEXT("");
    MultiByteToWideChar(CP_ACP, 0, m_CurrentPresetName, (_int)strlen(m_CurrentPresetName), szFullPath, MAX_PATH);

    const wstring& strSavePath = strFullPath + szFullPath + strExt;
    const wstring& strName = szFullPath + strExt;

    ofstream out(strSavePath);

    Json::Value Root;    

    m_pEffectPreset->ParsingEffect(Root);

    Json::StreamWriterBuilder builder;
    const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    writer->write(Root, &out);

    char* strMultiByte = new char[256];
    WideCharToMultiByte(CP_ACP, 0, strName.c_str(), -1, strMultiByte, 256, NULL, NULL);
    m_PresetNameList.push_back(const_cast<char*>(strMultiByte));


    m_pEffectPreset->DeleteAll();
    m_EffectNameList.clear();
    m_iEffectSelectIdx = 0;

}

void CImguiMgr::EditEffect()
{
    if (0 == m_iEffectType) {


    }
    else if (1 == m_iEffectType) {

        CEffectTexture::TEXEFFECT_DESC textureDesc = {};

        textureDesc.vCenter = m_vCenter;
        textureDesc.vColor = m_vColor;
        textureDesc.vRotation = m_vRotation;
        textureDesc.vStartScale = m_vTexStartScale;
        textureDesc.vMiddleScale = m_vTexMiddleScale;
        textureDesc.vEndScale = m_vTexEndScale;
        textureDesc.vDuration = m_vDuration;

        textureDesc.fTurnSpeed = m_fTurnSpeed;
        textureDesc.vTurnAxis = m_vAxis;

        textureDesc.fScaleChangeTime = m_fScaleChangeTime;

        char* EffectName = new char[256];
        memcpy_s(EffectName, sizeof(char) * 256, m_CurrentName, sizeof(char) * 256);

        _tchar szFullPath[MAX_PATH] = TEXT("");
        MultiByteToWideChar(CP_ACP, 0, m_ImagesList[m_iImageNum], (_int)strlen(m_ImagesList[m_iImageNum]), szFullPath, MAX_PATH);

        dynamic_pointer_cast<CEffectTexture>(m_pEffectPreset->GetEffect(m_iEffectSelectIdx))->EditDesc(szFullPath, textureDesc);


    }
    else if (2 == m_iEffectType) {

        CEffectMesh::MESH_DESC desc = {};

        desc.vCenter = m_vCenter;
        desc.vStartScale = m_vScale;
        desc.vMiddleScale = m_vMiddleScale;
        desc.vEndScale = m_vEndScale;

        desc.bUVLoop = m_bMaskUVLoop;
        desc.bLoop = m_IsLoop;

        desc.vColor = m_vColor;
        desc.vDuration = m_vDuration;

        desc.fScaleChangeTime = m_fScaleChangeTime;

        char* EffectName = new char[256];
        memcpy_s(EffectName, sizeof(char) * 256, m_CurrentName, sizeof(char) * 256);

        _tchar szFullPath[MAX_PATH] = TEXT("");
        MultiByteToWideChar(CP_ACP, 0, m_ImagesList[m_iImageNum], (_int)strlen(m_ImagesList[m_iImageNum]), szFullPath, MAX_PATH);

        _tchar szMeshName[MAX_PATH] = TEXT("");
        MultiByteToWideChar(CP_ACP, 0, m_MeshesList[m_iMeshNum], (_int)strlen(m_MeshesList[m_iMeshNum]), szMeshName, MAX_PATH);

        desc.szMaskTexKey = new char[256];
        desc.szNoiseTexKey = new char[256];

        memcpy_s(desc.szMaskTexKey, sizeof(char) * 256, m_MaskList[m_iMaskNum], sizeof(char) * 256);
        memcpy_s(desc.szNoiseTexKey, sizeof(char) * 256, m_NoiseList[m_iNoiseNum], sizeof(char) * 256);

        desc.bMask = m_IsMaskTex;
        desc.bNoise = m_IsNoiseTex;

        desc.vMaskDir = m_vMaskDirection;
        desc.vNoiseDir = m_vNoiseDirection;
        desc.fMaskSpeed = m_fMaskSpeed;
        desc.fNoiseSpeed = m_fNoiseSpeed;
        desc.vLerpColor = m_vLerpColor;

        dynamic_pointer_cast<CEffectMesh>(m_pEffectPreset->GetEffect(m_iEffectSelectIdx))->EditEffect(szFullPath, szMeshName, &desc);


    }



}


void CImguiMgr::TransFloat4x4ToFloatArray(_float4x4 _baseMat ,  _float* _pOut)
{
    _float temp[16] = { _baseMat._11, _baseMat._12, _baseMat._13, _baseMat._14,
                        _baseMat._21, _baseMat._22, _baseMat._23, _baseMat._24,
                        _baseMat._31, _baseMat._32, _baseMat._33, _baseMat._34,
                        _baseMat._41, _baseMat._42, _baseMat._43, _baseMat._44 };
    
    memcpy(_pOut, temp, sizeof(temp));
}

