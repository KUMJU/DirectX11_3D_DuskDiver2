#include "pch.h"
#include "Edit.h"
#include "ImguiMgr.h"
#include "CameraFree.h"
#include "Dummy.h"

#include "GameInstance.h"

CEdit::CEdit()
{
}

void CEdit::PreInit()
{
}

HRESULT CEdit::Initialize()
{
	if (FAILED(ReadyLight()))
		return E_FAIL;

	if (FAILED(ReadyLayerCamera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(CImguiMgr::GetInstance()->Initialize()))
		return E_FAIL;

	return S_OK;
}

void CEdit::Tick(_float _fTimeDelta)
{
}

HRESULT CEdit::Render()
{
	return S_OK;
}

HRESULT CEdit::ReadyLayerCamera(const wstring& _strLayerTag)
{
	CCameraFree::CAMERAFREE_DESC pCamFreeDesc = {};
	pCamFreeDesc.fMouseSensor = 0.05f;

	CCameraFree::CAMERA_DESC pCamDesc = {};
	pCamDesc.vEye = _float4(0.0f, 30.f, -25.f, 1.f);
	pCamDesc.vAt = _float4(0.0f, 0.0f, 0.f, 1.f);
	pCamDesc.fFovy = XMConvertToRadians(60.0f);
	pCamDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	pCamDesc.fNear = 0.2f;
	pCamDesc.fFar = 1000.f;

	CTransform::TRANSFORM_DESC pTransDesc = {};
	pTransDesc.fSpeedPerSet = 20.f;
	pTransDesc.fRotationPerSet = XMConvertToRadians(90.0f);

	if (FAILED(CGameInstance::GetInstance()->AddObject( LEVEL_EDIT, _strLayerTag, CCameraFree::Create(true, &pCamFreeDesc, &pCamDesc, &pTransDesc))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEdit::ReadyLayerPlayer(const wstring& _strLayerTag)
{
	return S_OK;
}

HRESULT CEdit::ReadyLight()
{
	LIGHT_DESC LightDesc = {};

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(CGameInstance::GetInstance()->AddLight(LightDesc)))
		return E_FAIL;

	return S_OK;
}


shared_ptr<CEdit> CEdit::Create()
{
	shared_ptr<CEdit> pInstance = make_shared<CEdit>();

	if (FAILED(pInstance->Initialize()))
		MSG_BOX("Failed to Create : CEdit Level");

	return pInstance;
}
