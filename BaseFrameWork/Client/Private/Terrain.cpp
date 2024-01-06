#include "pch.h"
#include "Terrain.h"
#include "GameInstance.h"

#include "Shader.h"
#include "VITerrain.h"
#include "Transform.h"
#include "Texture.h"

#include "Engine_Defines.h"

CTerrain::CTerrain()
	:CGameObject()
{
}

CTerrain::~CTerrain()
{
}

void CTerrain::PreInitialize()
{
}

HRESULT CTerrain::Initialize(CVITerrain::TerrainDesc _desc)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	if (FAILED(AddComponent(_desc)))
		return E_FAIL;

	m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxNorTex"));

	return S_OK;
}

HRESULT CTerrain::Initialize(const wstring& strHeightMapFilePath)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	if (FAILED(AddComponent(strHeightMapFilePath)))
		return E_FAIL;

	m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxNorTex"));

	return S_OK;
}

void CTerrain::PriorityTick(_float _fTimeDelta)
{
}

void CTerrain::Tick(_float _fTimeDelta)
{
}

void CTerrain::LateTick(_float _fTimeDelta)
{
	if (FAILED(CGameInstance::GetInstance()->AddRenderGroup(CRenderer::RENDER_NONBLEND, shared_from_this())))
		return;

}

HRESULT CTerrain::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pVITerrain->BindBuffers()))
		return E_FAIL;

	if (FAILED(m_pVITerrain->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::AddComponent(CVITerrain::TerrainDesc _desc)
{

	wrl::ComPtr<ID3D11Device> pDevice = CGameInstance::GetInstance()->GetDeviceInfo();
	wrl::ComPtr<ID3D11DeviceContext> pContext = CGameInstance::GetInstance()->GetDeviceContextInfo();

	m_pTextureCom.push_back(CGameInstance::GetInstance()->GetTexture(TEXT("Tile0")));
	m_Components.emplace(TEXT("Com_Texture"),m_pTextureCom[TYPE_DIFFUSE]);

	m_pTextureCom.push_back(CGameInstance::GetInstance()->GetTexture(TEXT("Mask")));
	m_Components.emplace(TEXT("Com_TextureMask"), m_pTextureCom[TYPE_MASK]);

	m_pTextureCom.push_back(CGameInstance::GetInstance()->GetTexture(TEXT("Brush")));
	m_Components.emplace(TEXT("Com_TextureBrush"), m_pTextureCom[TYPE_BRUSH]);

	m_pVITerrain = CVITerrain::Create(pDevice, pContext, _desc);
	m_Components.emplace(TEXT("Com_VITerrain"), m_pVITerrain);

	return S_OK;
}

HRESULT CTerrain::AddComponent(const wstring& strHeightMapFilePath)
{
	wrl::ComPtr<ID3D11Device> pDevice = CGameInstance::GetInstance()->GetDeviceInfo();
	wrl::ComPtr<ID3D11DeviceContext> pContext = CGameInstance::GetInstance()->GetDeviceContextInfo();

	m_pTextureCom.push_back(CGameInstance::GetInstance()->GetTexture(TEXT("Tile0")));
	m_Components.emplace(TEXT("Com_Texture"), m_pTextureCom[TYPE_DIFFUSE]);

	m_pTextureCom.push_back(CGameInstance::GetInstance()->GetTexture(TEXT("Mask")));
	m_Components.emplace(TEXT("Com_TextureMask"), m_pTextureCom[TYPE_MASK]);

	m_pTextureCom.push_back(CGameInstance::GetInstance()->GetTexture(TEXT("Brush")));
	m_Components.emplace(TEXT("Com_TextureBrush"), m_pTextureCom[TYPE_BRUSH]);

	m_pVITerrain = CVITerrain::Create(pDevice, pContext, strHeightMapFilePath);
	m_Components.emplace(TEXT("Com_VITerrain"), m_pVITerrain);

	return S_OK;
}

HRESULT CTerrain::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->BindShaderResource(m_pShader, "g_WorldMatrix")))
		return E_FAIL;

	_float4x4 ViewMat = CGameInstance::GetInstance()->GetTransformFloat4x4(CPipeLine::D3DTS_VIEW);

	if (FAILED(m_pShader->BindMatrix("g_ViewMatrix", &ViewMat)))
		return E_FAIL;

	_float4x4 ProjMat = CGameInstance::GetInstance()->GetTransformFloat4x4(CPipeLine::D3DTS_PROJ);

	if (FAILED(m_pShader->BindMatrix("g_ProjMatrix", &ProjMat)))
		return E_FAIL;

	if (FAILED(m_pTextureCom[TYPE_DIFFUSE]->BindShaderResources(m_pShader, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(m_pTextureCom[TYPE_MASK]->BindShaderResource(m_pShader, "g_MaskTexture", 0)))
		return E_FAIL;

	if (m_IsBrushVisible) {
		if (FAILED(m_pTextureCom[TYPE_BRUSH]->BindShaderResource(m_pShader, "g_BrushTexture", 0)))
			return E_FAIL;

		if (FAILED(m_pShader->BindRawValue("g_vBrushPos", &m_vBrushPos, sizeof(_float3))))
			return E_FAIL;
	}

	_float4 fCamPos = CGameInstance::GetInstance()->GetCamPosition();
	if (FAILED(m_pShader->BindRawValue("g_vCamPosition", &fCamPos, sizeof(_float4))))
		return E_FAIL;

	const LIGHT_DESC* pLightDesc = CGameInstance::GetInstance()->GetLightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;


	if (FAILED(m_pShader->BindRawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShader->BindRawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShader->BindRawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;

	_uint iPassIndex = { 0 };

	if (LIGHT_DESC::TYPE::TYPE_DIRECTIONAL == pLightDesc->eType) {

		if (FAILED(m_pShader->BindRawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
			return E_FAIL;

		iPassIndex = 1;
	}
	else if (LIGHT_DESC::TYPE::TYPE_POINT == pLightDesc->eType) {

		if (FAILED(m_pShader->BindRawValue("g_vLightPos", &pLightDesc->vPosition, sizeof(_float4))))
			return E_FAIL;

		if (FAILED(m_pShader->BindRawValue("g_fLightRange", &pLightDesc->fRange, sizeof(_float))))
			return E_FAIL;

		iPassIndex = 0;

	}

	if (FAILED(m_pShader->Begin(iPassIndex)))
		return E_FAIL;

	return S_OK;
}

shared_ptr<CTerrain> CTerrain::Create(CVITerrain::TerrainDesc _desc)
{
	shared_ptr<CTerrain> pInstance = make_shared<CTerrain>();

	if (FAILED(pInstance->Initialize(_desc)))
		MSG_BOX("Failed to Create : CTerrain");

	return pInstance;
};

shared_ptr<CTerrain> CTerrain::Create(const wstring& strHeightMapFilePath)
{
	shared_ptr<CTerrain> pInstance = make_shared<CTerrain>();

	if (FAILED(pInstance->Initialize(strHeightMapFilePath)))
		MSG_BOX("Failed to Create : CTerrain");

	return pInstance;
}
