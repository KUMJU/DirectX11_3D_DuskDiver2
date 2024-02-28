#include "pch.h"
#include "MapLoader.h"
#include <fstream>
#include "GameInstance.h"

#include "Dummy.h"
#include "Navigation.h"

#include "Escalator.h"
#include "MonsterTower.h"
#include "Model.h"
#include "Coin.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "ImGuizmo.h"

IMPLEMENT_SINGLETON(CMapLoader)

CMapLoader::CMapLoader()
{
}


void CMapLoader::LoadMapData(const char* _filePath, vector<char*>* _ObjectNameList , list<shared_ptr<CGameObject>>* _ObjectList, vector<shared_ptr<CModel>>* _modelList)
{

  //  Json::Value root;
   // root = JsonFileReader(_filePath);
  //  _int iJsonSize = root.size();

	ifstream fp(_filePath, ios::binary);

	_int iIndex = 0;

	fp.read((char*)&iIndex, sizeof(_int));

	for (_int i = 0; i < iIndex; ++i) {

		_float4x4 CurrentMatrix;
		fp.read((char*)&CurrentMatrix, sizeof(_float) * 16);
		char* szKeyName = new char[MAX_PATH];

		fp.read((char*)szKeyName, sizeof(char) * MAX_PATH);

		string strModelName = szKeyName;

		wstring wstrModelName;
		wstrModelName.assign(strModelName.begin(), strModelName.end());
		
		if (m_IsImguiLoad) {
			_ObjectNameList->push_back(szKeyName);
		}

		ClassifyObject(wstrModelName, &CurrentMatrix, _ObjectList);
	}

	fp.close();
}

void CMapLoader::LoadCellData(const wstring& _strPath)
{

	shared_ptr<CNavigation> pNavigation = CNavigation::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), _strPath);
	pNavigation->Tick(XMLoadFloat4x4(&m_WorldMatrix));    
	m_pNavigationCom = pNavigation;


}

shared_ptr<CNavigation> CMapLoader::GetCurrentNavi(_uint _iStartIdx)
{
	return CNavigation::Clone(m_pNavigationCom, _iStartIdx);;
}

void CMapLoader::ClassifyObject(const wstring& _strKeyName, _float4x4* _fWorldMat, list<shared_ptr<CGameObject>>* _ObjectList, vector<shared_ptr<CModel>>*  _modelList)
{
	//if문으로 별도로 생성해야하는건 구분하기
	shared_ptr<CGameObject> pInstance;

	if (TEXT("TowerA") == _strKeyName) {

		pInstance = CMonsterTower::Create(m_iTowerIdx);
		CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Object"), pInstance);
		++m_iTowerIdx;

	}
	else if (TEXT("Escalator_Base") == _strKeyName) {

		pInstance = CEscalator::Create(0);
		CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Object"), pInstance);


	}else if(TEXT("GoldA") == _strKeyName){
		_fWorldMat;
		pInstance = CCoin::Create();
		CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Object"), pInstance);

	}else {

		pInstance = CDummy::Create(_strKeyName);
		CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Object"), pInstance);
	}

	pInstance->SetWorldMatrix(*_fWorldMat);


	//포지션 세팅 이후 빛 세팅 
	if (TEXT("TowerA") == _strKeyName) {
		dynamic_pointer_cast<CMonsterTower>(pInstance)->SetTowerLight();
	}


	if (m_IsImguiLoad) {
		_ObjectList->push_back(pInstance);
			
	}

}


HRESULT CMapLoader::JsonFileWriter(Json::Value _root, const char* _filePath)
{
    if (_root.empty() || "" == _filePath) {
        MSG_BOX("JSON root is Empty or Wrong PathName");
        return E_FAIL;
    }

    Json::StreamWriterBuilder builder;
    const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

    std::ofstream ofStream;
    ofStream.open(_filePath, ios_base::out);

    if (ofStream.is_open()) {
        writer->write(_root, &ofStream);
    }
    else {
        return E_FAIL;
    }

    ofStream.close();

	return S_OK;
}

Json::Value CMapLoader::JsonFileReader(const char* _filePath)
{
	Json::Value root;

	if ("" == _filePath)
		return root;

    ifstream fin(_filePath);

    fin >> root;

	return root;
}
