#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "VITerrain.h"

BEGIN(Engine)
class CShader;
class CTexture;
END

BEGIN(Client)

class CTerrain final : public CGameObject
{
public:
	enum TEXTURETYPE {TYPE_DIFFUSE, TYPE_MASK, TYPE_BRUSH, TYPE_END};

public:
	CTerrain();
	virtual ~CTerrain();

public:
	virtual void PreInitialize() override;

	virtual HRESULT Initialize(CVITerrain::TerrainDesc _desc);
	virtual HRESULT Initialize(const wstring& strHeightMapFilePath);

	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void SetTerrainBrushPos(_float3 _vPos) { m_vBrushPos = _vPos; }
	void SetBrushVisible(_bool _bVisible) { m_IsBrushVisible = _bVisible; }

private:
	shared_ptr<CShader> m_pShader = nullptr;
	vector<shared_ptr<CTexture>> m_pTextureCom;
	shared_ptr<CVITerrain> m_pVITerrain = nullptr;

	_float3 m_vBrushPos = _float3(0.f, 0.f, 0.f);
	_bool m_IsBrushVisible = false;

private:
	HRESULT AddComponent(CVITerrain::TerrainDesc _desc);
	HRESULT AddComponent(const wstring& strHeightMapFilePath);

	HRESULT Bind_ShaderResources();
	
public:
	static shared_ptr<CTerrain> Create(CVITerrain::TerrainDesc _desc);
	static shared_ptr<CTerrain> Create(const wstring& strHeightMapFilePath);

};

END