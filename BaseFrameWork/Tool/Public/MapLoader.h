#pragma once

#include "Tool_Defines.h"
#include "Json/json.h"

BEGIN(Engine)
class CGameObject;
class CNavigation;
class CModel;
END


BEGIN(Tool)

class CMapLoader
{
	DECLARE_SINGLETON(CMapLoader)

private:
	CMapLoader();
	~CMapLoader() = default;


public:
	void LoadMapData(const char* _filePath, vector<char*>* _ObjectNameList = nullptr, list<shared_ptr<CGameObject>>* _ObjectList = nullptr, vector<shared_ptr<CModel>>* _modelList = nullptr );
	void LoadCellData(const wstring& _strPath);
	shared_ptr<CNavigation> GetCurrentNavi(_uint _iStartIdx);

private:
	void ClassifyObject(const wstring& _strKeyName, _float4x4* _fWorldMat, list<shared_ptr<CGameObject>>* _ObjectList = nullptr, vector<shared_ptr<CModel>>*_modelList = nullptr);

public:
	HRESULT JsonFileWriter(Json::Value _root, const char* _filePath);
	Json::Value JsonFileReader(const char* _filePath);

	void SetLoadStateImgui() { m_IsImguiLoad = true; }
	void SetWorldMatrix(_float4x4 _MapMat) {
		m_WorldMatrix = _MapMat;
	}
	
private:
	_bool m_IsImguiLoad = false;
	_uint m_iTowerIdx = 0;
	_float4x4 m_WorldMatrix;

	shared_ptr<CNavigation> m_pNavigationCom = nullptr;

};

END