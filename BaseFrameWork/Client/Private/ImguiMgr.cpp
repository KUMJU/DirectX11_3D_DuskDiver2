#include "pch.h"
#include "ImguiMgr.h"
#include "GameInstance.h"
#include "NaviToolMgr.h"

#include "Terrain.h"
#include "Dummy.h"

#include "GameObject.h"
#include "MapLoader.h"

#include "Mesh.h"
#include "Model.h"

#include <fstream>

#include "Escalator.h"

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

    //리스트 세팅
    SetObjectList();

    CMapLoader::GetInstance()->SetLoadStateImgui();
    m_pMapTransform =  dynamic_pointer_cast<CTransform>(m_pMapMesh->GetComponent(TEXT("Com_Transform")));
    
    m_PickingModelList.push_back((m_pMapMesh));
    CNaviToolMgr::GetInstance()->SetTerrainTransform(m_pMapTransform);


    //shared_ptr<CEscalator> pInst1 = CEscalator::Create(0);

    //_vector v1 = { 1.f, 0.f, 0.f, 0.f };
    //_vector v2 = { 0.f, 1.f, 0.f, 0.f };
    //_vector v3 = { 0.f, 0.f, 1.f, 0.f };
    //_vector v4 = { 4.27f, -7.8f, -20.f, 1.f };


    //_matrix World1 = { v1 ,v2,v3,v4};
    //shared_ptr<CTransform> pTrans =  dynamic_pointer_cast<CTransform>(pInst1->GetComponent(TEXT("Com_Transform")));
    //pTrans->SetState(CTransform::STATE_POSITION, { 4.2f, -7.8f, -20.f, 1.f });
    //pTrans->SetScaling(1.5f, 1.8f, 1.8f);
    ////_float4x4 WorldFloat;
    ////XMStoreFloat4x4(&WorldFloat, World1);
    ////pTrans->SetWorldMatrix(WorldFloat);
    //CGameInstance::GetInstance()->AddObject(LEVEL_EDIT, TEXT("Layer_BackGround"), pInst1);

    //m_PickingModelList.push_back(pInst1);



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

    ObjectLoader();

    if(m_IsZmoUsing)
        ImgZmoUpdate();

    SelectObjectInfo();
    DrawNaviMesh();

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

void CImguiMgr::ObjectLoader()
{
    ImGui::Begin("Map Object Creator");

    ImGui::SeparatorText("Object Spawner");

    //combobox
    ImGui::Combo("SelectLevel", &m_iCurrentLevelIdx, "Arcade Map\0Town\0Boss Map\0");

    ImGui::ListBox("Object List", &m_ObjListIdx, m_ObjectList.data(), (_int)m_ObjectList.size(), 10);

    if (ImGui::Button("Select Object Create")) {
       
        _tchar szFullPath[MAX_PATH] = TEXT("");
        MultiByteToWideChar(CP_ACP, 0, m_ObjectList[m_ObjListIdx] , (_int)strlen(m_ObjectList[m_ObjListIdx]), szFullPath, MAX_PATH);

        shared_ptr<CGameObject> pDummy = CDummy::Create(szFullPath);
        CGameInstance::GetInstance()->AddObject(LEVEL_EDIT, TEXT("Layer_BackGround"), pDummy);
        m_CurrentObjectList.push_back(m_ObjectList[m_ObjListIdx]);
        m_Objects.push_back(pDummy);
        m_PickingModelList.push_back(pDummy);
    }

    ImGui::Dummy(ImVec2(0.f, 6.f));

    //////////////////////////////////////////////////////////////////////////////////////
    ImGui::SeparatorText("Current Map");

    _int iSelectedNum = m_iCurrentObjIdx;

    ImGui::ListBox("Create Object", &m_iCurrentObjIdx, m_CurrentObjectList.data(), (_int)m_CurrentObjectList.size(), 10);

    //선택 번호가 바뀜
    if (iSelectedNum != m_iCurrentObjIdx) {
        auto iter2 = m_Objects.begin();
        for (_int i = 0; i < m_iCurrentObjIdx; ++i) {
            ++iter2;
        }

        m_pCurrentSelectObj = (*iter2);
        m_IsZmoUsing = true;

    }

    if (ImGui::Button("Delete")) {
       
        auto iter = m_CurrentObjectList.begin();
        auto iter2 = m_Objects.begin();

        for (_int i = 0; i < m_iCurrentObjIdx; ++i) {
            ++iter;
            ++iter2;
        }

        m_CurrentObjectList.erase(iter);
        (*iter2)->SetInActive();
        m_Objects.erase(iter2);

        m_iCurrentObjIdx = -1;
    } ImGui::SameLine();

    if (ImGui::Button("Clear All")) {

        auto iter2 = m_Objects.begin();


        for (auto iter = m_Objects.begin(); iter != m_Objects.end(); ++iter) {

            (*iter)->SetInActive();

        }
        

        m_CurrentObjectList.clear();
        m_Objects.clear();

        m_iCurrentObjIdx = -1;
    }

    //////////////////////////////////////////////////////////////////////////////////////

    ImGui::SeparatorText("Load & Save Data");
    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.5f, 1.f), "Enter the file name only");
    ImGui::InputText("File Name", m_SaveFileName, 256,ImGuiInputTextFlags_CharsNoBlank);

    if (ImGui::Button("Save")) {
        SaveData();
    } ImGui::SameLine();

    if (ImGui::Button("Load")) {
        string strBasePath = "../Bin/DataFiles/";
        string fileName = m_SaveFileName;
        string fullPathName = strBasePath + fileName + ".dat";

        CMapLoader::GetInstance()->LoadMapData(fullPathName.c_str(), &m_CurrentObjectList, &m_Objects);


        for (auto& iter : m_Objects) {

           m_PickingModelList.push_back(iter);

        }

    }

    if (m_Objects.empty())
        m_iCurrentObjIdx = -1;

    ImGui::End();
}

void CImguiMgr::SelectObjectInfo()
{
    ImGui::Begin("SelectObjectInfo");

    ImGui::SeparatorText("Object Info");

    static int e = 0;
    ImGui::RadioButton("Translate", &e, 0); ImGui::SameLine();
    ImGui::RadioButton("Rotate", &e, 1); ImGui::SameLine();
    ImGui::RadioButton("Scale", &e, 2);

    if (0 == e) {
        m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
    }
    else if (1 == e) {
        m_CurrentGizmoOperation = ImGuizmo::ROTATE;
    }
    else if (2 == e) {
        m_CurrentGizmoOperation = ImGuizmo::SCALE;
    }

    ImGui::Dummy(ImVec2(0.f, 6.f));

    ImGui::InputFloat3("T", m_vObjectTranslate);
    ImGui::InputFloat3("R", m_vObjectRotate);
    ImGui::InputFloat3("S", m_vObjectScale);

    ImGui::End();
}

void CImguiMgr::ImgZmoUpdate()
{
    if (-1 != m_iCurrentObjIdx || nullptr != m_pCurrentSelectObj) {
        ImGuizmo::BeginFrame();
        ImGuizmo::Enable(true);

        ImGuiIO& io = ImGui::GetIO();

        _float4x4 projMat = CGameInstance::GetInstance()->GetTransformFloat4x4(Engine::CPipeLine::D3DTS_PROJ);
        _float4x4 viewMat = CGameInstance::GetInstance()->GetTransformFloat4x4(Engine::CPipeLine::D3DTS_VIEW);

        _float* World;
        World = m_pCurrentSelectObj->GetWoldMatFloatArr();

        float Proj[16], View[16];
        TransFloat4x4ToFloatArray(viewMat, View);
        TransFloat4x4ToFloatArray(projMat, Proj);

        //SetRect < 꼭 넣어줘야함! 뭔가 그리기 전에 캔버스? 같은 역할을 하나봄
        ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
        
        //그리드 생성
        ImGuizmo::DrawGrid(View, Proj, identityMatrix, 100.f);
        
        //실질적인 기능을 해주는 function
        ImGuizmo::Manipulate(View, Proj, m_CurrentGizmoOperation, m_CurrentGizmoMode, World);

        ImGuizmo::DecomposeMatrixToComponents(World, m_vObjectTranslate, m_vObjectRotate, m_vObjectScale);
    }
}

void CImguiMgr::DrawNaviMesh()
{
    ImGui::Begin("NaviMesh");


    if (!m_KeyDeb) {


        if (CGameInstance::GetInstance()->GetDIMouseState(MOUSEKEYSTATE::DIM_RB)) {

            POINT pt = GetMouse();

            if (m_pMapMesh) {
                m_KeyDeb = true;

                _int iIdxNum = -1;
                _float fNearest = 9999.f;
                _float fDistance = 0.f;
                m_vCurrentPicking = _float3();

                _int iPickingNum = -1;

                for (auto& iter : m_PickingModelList) {
                    ++iIdxNum;
                    shared_ptr<CModel> Models = dynamic_pointer_cast<CModel>(iter->GetComponent(TEXT("Com_Model")));

                    vector<shared_ptr<CMesh>> Meshes = Models->GetMeshes();
                    shared_ptr<CTransform> pTransform = dynamic_pointer_cast<CTransform>(iter->GetComponent(TEXT("Com_Transform")));


                    _float3 vPickingPos = CGameInstance::GetInstance()->MeshPicking(pt, Models, pTransform, fDistance);

                    if ((vPickingPos.x != 0 && vPickingPos.y != 0 && vPickingPos.z != 0)&&
                        fDistance < fNearest) {
                        fNearest = fDistance;
                        m_vCurrentPicking = vPickingPos;
                        iPickingNum = iIdxNum;
                    }

                }

                if ((m_vCurrentPicking.x != 0 && m_vCurrentPicking.y != 0 && m_vCurrentPicking.z != 0)) {
                    //각 오브젝트의 좌표계를 통맵좌표계로 치환
                    if (iPickingNum != 0) {
                        //m_vCurrentPicking
                        
                        
                        shared_ptr<CTransform> pTransform = dynamic_pointer_cast<CTransform>(m_PickingModelList[iPickingNum]->GetComponent(TEXT("Com_Transform")));
                        _matrix OriginWorld = pTransform->GetWorldMatrix();
                        _vector PickWorldPos = XMVector3TransformCoord(XMLoadFloat3(&m_vCurrentPicking), OriginWorld);
                        
                        _matrix MapInverseWorldMat = m_pMapTransform->GetWorldMatrixInverse();
                        PickWorldPos =  XMVector3TransformCoord(PickWorldPos, MapInverseWorldMat);
                        XMStoreFloat3(&m_vCurrentPicking, PickWorldPos);

                    }

                    CNaviToolMgr::GetInstance()->PickPoint(m_vCurrentPicking);
                }
            }

        }
    }


    ImGui::Text("X : %f", m_vCurrentPicking.x);
    ImGui::Text("Y : %f", m_vCurrentPicking.y);
    ImGui::Text("Z : %f", m_vCurrentPicking.z);


    if (ImGui::Button("Delete Last")) {

        CNaviToolMgr::GetInstance()->CancleLast();

    }


    if (ImGui::Button("Save Cells")) {

        CNaviToolMgr::GetInstance()->ParsingCell(TEXT("CellTest.dat"));

    }

    if (ImGui::Button("Load Cells")) {

        CNaviToolMgr::GetInstance()->LoadData(TEXT("CellTest.dat"));
    }


    ImGui::End();

}

void CImguiMgr::SaveData()
{
    _int iSize = m_Objects.size();

    auto iter = m_CurrentObjectList.begin();
    auto iter2 = m_Objects.begin();

    Json::Value root;

    string strBasePath = "../Bin/DataFiles/";
    string fileName = m_SaveFileName;
    string fullPathName = strBasePath + fileName + ".dat";

    ofstream fp(fullPathName , ios::binary);

    fp.write((char*)&iSize, sizeof(_int));

    //World Matrix 통째 저장 버전 
    for (_int i = 0; i < iSize; ++i) {

        Json::Value WorldMatrix;
        Json::Value ValueData;

        float* WorldObj = (*iter2)->GetWoldMatFloatArr();
        
        fp.write((char*)WorldObj, sizeof(_float) * 16);
        
        /*WorldMatrix["v0"] = to_string(WorldObj[0]);
        WorldMatrix["v1"] = to_string(WorldObj[1]);
        WorldMatrix["v2"] = to_string(WorldObj[2]);
        WorldMatrix["v3"] = to_string(WorldObj[3]);
        WorldMatrix["v4"] = to_string(WorldObj[4]);
        WorldMatrix["v5"] = to_string(WorldObj[5]);
        WorldMatrix["v6"] =  to_string(WorldObj[6]);
        WorldMatrix["v7"] =  to_string(WorldObj[7]);
        WorldMatrix["v8"] =  to_string(WorldObj[8]);
        WorldMatrix["v9"] = to_string(WorldObj[9]);
        WorldMatrix["v10"] = to_string(WorldObj[10]);
        WorldMatrix["v11"] = to_string(WorldObj[11]);
        WorldMatrix["v12"] = to_string(WorldObj[12]);
        WorldMatrix["v13"] = to_string(WorldObj[13]);
        WorldMatrix["v14"] = to_string(WorldObj[14]);
        WorldMatrix["v15"] = to_string(WorldObj[15]);

        Json::Value IDKey;*/

        fp.write((char*)*iter, sizeof(char) * MAX_PATH);


        //IDKey["WorldMatrix"] = WorldMatrix;
        //IDKey["ModelName"] = *iter;
        //root[i] = IDKey;

        ++iter;
        ++iter2;
    }


    //SRT별도 저장 버전
   /* for (_int i = 0; i < iSize; ++i) {

        Json::Value Scale;
        Json::Value Rotate;
        Json::Value Trans;

        Json::Value ValueData;

        float Sc[3], Rt[3], Tr[3];

        float* WorldObj = (*iter2)->GetWoldMatFloatArr();
        ImGuizmo::DecomposeMatrixToComponents(WorldObj, Tr, Rt, Sc);

        Scale["x"] = Sc[0];
        Scale["y"] = Sc[1];
        Scale["z"] = Sc[2];

        Rotate["x"] = Rt[0];
        Rotate["y"] = Rt[1];
        Rotate["z"] = Rt[2];

        Trans["x"] = Tr[0];
        Trans["y"] = Tr[1];
        Trans["z"] = Tr[2];

        Json::Value IDKey;
        IDKey["Scale"] = Scale;
        IDKey["Rotate"] = Rotate;
        IDKey["Trans"] = Trans;

        root[(*iter)] = IDKey;

        ++iter;
        ++iter2;
    }*/

    fp.close();
  //  CMapLoader::GetInstance()->JsonFileWriter(root, fullPathName.c_str());
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

POINT CImguiMgr::GetMouse()
{
    POINT pt;
    ::GetCursorPos(&pt);
    ::ScreenToClient(g_hWnd, &pt);

    return pt;

}

void CImguiMgr::TransFloat4x4ToFloatArray(_float4x4 _baseMat ,  _float* _pOut)
{
    _float temp[16] = { _baseMat._11, _baseMat._12, _baseMat._13, _baseMat._14,
                        _baseMat._21, _baseMat._22, _baseMat._23, _baseMat._24,
                        _baseMat._31, _baseMat._32, _baseMat._33, _baseMat._34,
                        _baseMat._41, _baseMat._42, _baseMat._43, _baseMat._44 };
    
    memcpy(_pOut, temp, sizeof(temp));
}

