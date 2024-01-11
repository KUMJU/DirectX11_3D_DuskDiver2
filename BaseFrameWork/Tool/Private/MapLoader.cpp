#include "pch.h"
#include "MapLoader.h"
#include <fstream>
#include "GameInstance.h"

#include "Dummy.h"

IMPLEMENT_SINGLETON(CMapLoader)

CMapLoader::CMapLoader()
{
}


void CMapLoader::LoadMapData(const char* _filePath, vector<char*>* _ObjectNameList , list<shared_ptr<CGameObject>>* _ObjectList)
{

    Json::Value root;
    root = JsonFileReader(_filePath);
    _int iJsonSize = root.size();

	for (auto iter = root.begin(); iter != root.end(); ++iter) {
		string KeyName = iter.key().asString();

		_float4x4 CurrentMatrix;

		Json::Value vId = root[KeyName];
		Json::Value WorldMatrix = vId["WorldMatrix"];

		CurrentMatrix._11 = WorldMatrix["v0"].asFloat();
		CurrentMatrix._12 = WorldMatrix["v1"].asFloat();
		CurrentMatrix._13 = WorldMatrix["v2"].asFloat();
		CurrentMatrix._14 = WorldMatrix["v3"].asFloat();
		CurrentMatrix._21 = WorldMatrix["v4"].asFloat();
		CurrentMatrix._22 = WorldMatrix["v5"].asFloat();
		CurrentMatrix._23 = WorldMatrix["v6"].asFloat();
		CurrentMatrix._24 = WorldMatrix["v7"].asFloat();
		CurrentMatrix._31 = WorldMatrix["v8"].asFloat();
		CurrentMatrix._32 = WorldMatrix["v9"].asFloat();
		CurrentMatrix._33 = WorldMatrix["v10"].asFloat();
		CurrentMatrix._34 = WorldMatrix["v11"].asFloat();
		CurrentMatrix._41 = WorldMatrix["v12"].asFloat();
		CurrentMatrix._42 = WorldMatrix["v13"].asFloat();
		CurrentMatrix._43 = WorldMatrix["v14"].asFloat();
		CurrentMatrix._44 = WorldMatrix["v15"].asFloat();

		wstring strModelName;
		strModelName.assign(KeyName.begin(), KeyName.end());
		
		if (m_IsImguiLoad) {
			char* ddd = (char*)(KeyName.c_str());
			_int a = 4;

			char* szKeyName = new char[KeyName.size()+1];
			memcpy(szKeyName, KeyName.c_str(), KeyName.size() + 1);
			_ObjectNameList->push_back(szKeyName);
		}

		ClassifyObject(strModelName, &CurrentMatrix, _ObjectList);
	}

}

void CMapLoader::ClassifyObject(const wstring& _strKeyName, _float4x4* _fWorldMat, list<shared_ptr<CGameObject>>* _ObjectList)
{
	//if문으로 별도로 생성해야하는건 구분하기
	shared_ptr<CGameObject> pInstance;

	if (TEXT("MonsterTower") == _strKeyName) {

	}
	else {

		pInstance = CDummy::Create(_strKeyName);
		CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Object"), pInstance);
	}
	pInstance->SetWorldMatrix(*_fWorldMat);

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
