#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CResourceMgr final
{
public:
	//특정 레벨에서만 필요한 resource는 쓰고 지우기 위해 별도 bool 변수를 둔 resource struct 사용
	template<typename T>
	struct ResourceDesc {
		shared_ptr<T> pRes;
		_bool IsBaseRes;
	};

public:
	CResourceMgr(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	~CResourceMgr() = default;

public:

	void LoadLevelResource(_uint _iLevelIndex);

	shared_ptr<class CShader> GetShader(const wstring& _strShaderKey);
	shared_ptr<class CTexture> GetTexture(const wstring& _strTextureKey);
	shared_ptr<class CModel> GetModel(const wstring& _strModelKey);
		
private:
	void BaseResourceLoad();
	void LogoResourceLoad();
	void ArcadeMapResourceLoad();
	void TownMapResourceLoad();

/*Shader*/
public:
	HRESULT LoadShader();
	void ReadShaderFile(const wstring& _strBaseFilepath , const wstring& _strExt);

/*Texture*/
public:
	// 멀티 텍스쳐 때문에 어떻게 읽을건지 좀 더 고민해봐야할듯?
	HRESULT LoadTexture(const wstring& _strBaseFilepath);
	void ReadTextureFile(const wstring& _strBaseFilepath, const wstring& _strExt);
 
/*Mesh*/
public:
	HRESULT LoadMesh(const wstring& _strBaseFilepath);
	void ReadAnimMeshFile(const wstring& _strBaseFilepath, const wstring& _strExt);
	void ReadNonAnimMeshFile(const wstring& _strBaseFilepath, const wstring& _strExt);


/*Material*/
public:

private:
	//파일인지 디렉토리인지 구분해주는 함수
	_bool IsFileOrDir(_wfinddata_t _fd);
	//확장자 제거
	wstring EraseExt(const wstring& _strFileName);
	//특정 확장자 찾기
	wstring FindExt(const wstring& _strFileName);

public:
	//특정 레벨이 끝난 후, 그 레벨에서만 사용하는 리소스를 지워버리는 함수
	void DeleteResource();

public:
	map <wstring, ResourceDesc<class CModel>>* GetModels() { return &m_Models; }

private:
	map <wstring, ResourceDesc<class CTexture>> m_Textures;
	map <wstring, ResourceDesc<class CMesh>> m_Meshes;
	map <wstring, ResourceDesc<class CModel>> m_Models;
	map<wstring, shared_ptr<class CShader>> m_Shaders;

private:

	_bool m_IsBaseRes = true;

	wrl::ComPtr<ID3D11Device> m_pDevice = nullptr;
	wrl::ComPtr<ID3D11DeviceContext> m_pContext = nullptr;

public:
	static shared_ptr<CResourceMgr> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);


};

END