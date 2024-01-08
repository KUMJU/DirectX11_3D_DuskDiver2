#pragma once

#include "Client_Defines.h"
#include "Json/json.h"

BEGIN(Engine)
class CGameObject;
END


BEGIN(Client)

class CMapLoader
{
	DECLARE_SINGLETON(CMapLoader)

private:
	CMapLoader();
	~CMapLoader() = default;


public:
	void LoadMapData(const char* _filePath, vector<char*>* _ObjectNameList = nullptr, list<shared_ptr<CGameObject>>* _ObjectList = nullptr);

private:
	void ClassifyObject(const wstring& _strKeyName, _float4x4* _fWorldMat, list<shared_ptr<CGameObject>>* _ObjectList = nullptr);

public:
	HRESULT JsonFileWriter(Json::Value _root, const char* _filePath);
	Json::Value JsonFileReader(const char* _filePath);

	void SetLoadStateImgui() { m_IsImguiLoad = true; }

private:
	_bool m_IsImguiLoad = false;

};

END