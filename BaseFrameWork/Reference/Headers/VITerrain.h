#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVITerrain final : public CVIBuffer
{
public:
	struct TerrainDesc {
		_uint iVerticesX;
		_uint iVerticesZ;
	};


public:
	CVITerrain(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	virtual ~CVITerrain();

public:
	//Terrain X,Z Num을 입력받아 생성
	HRESULT Initialize(TerrainDesc _desc);
	//Height Map 이미지를 로드해서 생성
	HRESULT Initialize(const wstring& strHeightMapFilePath);


public:
	TerrainDesc GetTerrainDesc() { return m_tTerrainDesc; }

private:
	_uint m_iNumVerticesX = { 0 };
	_uint m_iNumVerticesZ = { 0 };

	TerrainDesc m_tTerrainDesc = {};

public:
	static shared_ptr<CVITerrain> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, TerrainDesc _desc);
	static shared_ptr<CVITerrain> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, const wstring& strHeightMapFilePath);

};

END
