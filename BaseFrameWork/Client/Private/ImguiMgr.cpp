#include "pch.h"
#include "ImguiMgr.h"
#include "GameInstance.h"

#include "Terrain.h"
#include "Dummy.h"

IMPLEMENT_SINGLETON(CImguiMgr)

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

    SetObjectList();

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
    StartFrame();

    TerrainEditor();
    PickingNavi();

    if (CGameInstance::GetInstance()->GetDIMouseState(MOUSEKEYSTATE::DIM_RB)) {
        GetMouse();
    }

    ObjectLoader();
    ImgZmoTest();

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

void CImguiMgr::TerrainEditor()
{
    ImGuiStyle& imguiStyle = ImGui::GetStyle();


    ImGui::Begin("TerrainEditior");
    ImGui::Text("Terrain Size");


    //Terrain 텍스쳐 지정
   


    //Terrain 넓이, 높이 지정

    static int i1 = 0;
    static int i2 = 0;

    ImGui::DragInt("Size X", &i1, 4, 0, INT_MAX);
    ImGui::DragInt("Size Z", &i1, 4, 0, INT_MAX);

    //칸 띄우기
    ImGui::Dummy(ImVec2(0.f, 10.f / 2.f));

    //TextureList

    if (ImGui::Button("Create")) {

        if (0 == i1)
            return;

        CVITerrain::TerrainDesc desc = {};
        desc.iVerticesX = i1;
        desc.iVerticesZ = i1;

        //현재 있는 terrain 삭제
        if (nullptr != m_pCurrentTerrain) {
            m_pCurrentTerrain->SetInActive();
            m_pCurrentTerrain = nullptr;
        }

        m_pCurrentTerrain = CTerrain::Create(desc);
        CGameInstance::GetInstance()->AddObject(LEVEL_EDIT, TEXT("Layer_BackGround"), m_pCurrentTerrain);

    }

    ImGui::SameLine(60);

    if (ImGui::Button("Delete")) {

        if (nullptr == m_pCurrentTerrain) {
            return;
        }
        else {
            m_pCurrentTerrain->SetInActive();
            m_pCurrentTerrain = nullptr;
        }
    }


    ImGui::End();

}

void CImguiMgr::PickingNavi()
{
    ImGuiStyle& imguiStyle = ImGui::GetStyle();


    ImGui::Begin("Picking Navigation");
    ImGui::Text("1. Picking Mode");

    static int e = 0;
    ImGui::RadioButton("None", &e, 0); ImGui::SameLine();
    ImGui::RadioButton("Terrain", &e, 1); ImGui::SameLine();
    ImGui::RadioButton("Object", &e, 2);

    if (0 == e) {
        m_eMouseState = EMouseState::MS_DEFAULT;
    }
    else if (1 == e) {
        m_eMouseState = EMouseState::MS_TERRAIN;
    }
    else if (2 == e) {
        m_eMouseState = EMouseState::MS_OBJECT;
    }

    ImGui::Dummy(ImVec2(0.f, 6.f));
    ImGui::Text("2. Current Picking Position");

    ImGui::Text("X : %.1f , Y : %.1f, Z: %.1f", m_PickingPos.x, m_PickingPos.y, m_PickingPos.z);
    
    //현재 생성한 터레인이 존재할 때 
    if (m_pCurrentTerrain) {
        if (0 != m_PickingPos.x && 0 != m_PickingPos.y && 0 != m_PickingPos.z) {
            m_pCurrentTerrain->SetBrushVisible(true);
            m_pCurrentTerrain->SetTerrainBrushPos(_float3(m_PickingPos.x, m_PickingPos.y, m_PickingPos.z));
        }
        else {
            m_pCurrentTerrain->SetBrushVisible(false);
        }
    }

    //오브젝트 생성
    ImGui::Dummy(ImVec2(0.f, 6.f));

    if (ImGui::Button("Dummy Object Create")) {
        shared_ptr<CDummy> pDummy = CDummy::Create(TEXT("AssetsA"));
        CGameInstance::GetInstance()->AddObject(LEVEL_EDIT, TEXT("Layer_BackGround"), pDummy);
        _float4 fPos = _float4(m_PickingPos.x, m_PickingPos.y, m_PickingPos.z, 1.f);
        pDummy->SetPosition(XMLoadFloat4(&fPos));
    }


    ImGui::End();

}

void CImguiMgr::ObjectLoader()
{
    ImGui::Begin("Map Object Creator");

    static int item_current = 1;
    ImGui::ListBox("listbox", &item_current, m_ObjectList.data(), m_ObjectList.size(), 10);

    if (ImGui::Button("Select Object Create")) {
       
        _tchar szFullPath[MAX_PATH] = TEXT("");
        MultiByteToWideChar(CP_ACP, 0, m_ObjectList[item_current] , (_int)strlen(m_ObjectList[item_current]), szFullPath, MAX_PATH);

        shared_ptr<CDummy> pDummy = CDummy::Create(szFullPath);
        CGameInstance::GetInstance()->AddObject(LEVEL_EDIT, TEXT("Layer_BackGround"), pDummy);
  /*      _float4 fPos = _float4(m_PickingPos.x, m_PickingPos.y, m_PickingPos.z, 1.f);
        pDummy->SetPosition(XMLoadFloat4(&fPos));*/
    }

    ImGui::End();
}

void CImguiMgr::ImgZmoTest()
{
    ImGuizmo::BeginFrame();


}

void CImguiMgr::SetObjectList()
{

    map <wstring, CResourceMgr::ResourceDesc<class CModel>>* ModelList = CGameInstance::GetInstance()->GetModels();

    for (auto& iter : *ModelList) {

        _int iSize = lstrlenW(iter.first.c_str());

        char* strMultiByte = new char[iSize+1];
        WideCharToMultiByte(CP_ACP, 0, iter.first.c_str(),-1, strMultiByte, iSize + 1, NULL, NULL);
        m_ObjectList.push_back(const_cast<char*>(strMultiByte));
    }
}

void CImguiMgr::GetMouse()
{
    POINT pt;
    ::GetCursorPos(&pt);
    ::ScreenToClient(g_hWnd, &pt);


    //TerrainPickingMode
    if (EMouseState::MS_TERRAIN == m_eMouseState) {

        if(nullptr != m_pCurrentTerrain){


            shared_ptr<CVITerrain> pTerrain = dynamic_pointer_cast<CVITerrain>(m_pCurrentTerrain->GetComponent(TEXT("Com_VITerrain")));
            shared_ptr<CTransform> pTransform = dynamic_pointer_cast<CTransform>(m_pCurrentTerrain->GetComponent(TEXT("Com_Transform")));

            if (nullptr != pTerrain && nullptr != pTransform) {
                _float4 Pos =  CGameInstance::GetInstance()->TerrainPicking(pt, pTerrain, pTransform);
                m_PickingPos = _float3(Pos.x, Pos.y, Pos.z);
            }
        }
    }


}

