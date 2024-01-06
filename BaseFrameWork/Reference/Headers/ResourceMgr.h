#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CResourceMgr final
{
public:
	//Ư�� ���������� �ʿ��� resource�� ���� ����� ���� ���� bool ������ �� resource struct ���
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
	// ��Ƽ �ؽ��� ������ ��� �������� �� �� ����غ����ҵ�?
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
	//�������� ���丮���� �������ִ� �Լ�
	_bool IsFileOrDir(_wfinddata_t _fd);
	//Ȯ���� ����
	wstring EraseExt(const wstring& _strFileName);
	//Ư�� Ȯ���� ã��
	wstring FindExt(const wstring& _strFileName);

public:
	//Ư�� ������ ���� ��, �� ���������� ����ϴ� ���ҽ��� ���������� �Լ�
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