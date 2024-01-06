#include "Camera.h"
#include "GameInstance.h"

CCamera::CCamera()
    :CGameObject()
{
}

HRESULT CCamera::Initialize(CAMERA_DESC* _pCamDes, CTransform::TRANSFORM_DESC* _TransDesc)
{

    m_fFovy = _pCamDes->fFovy;
    m_fNear = _pCamDes->fNear;
    m_fFar = _pCamDes->fFar;
    m_fAspect = _pCamDes->fAspect;

    if (FAILED(__super::Initialize(_TransDesc)))
        return E_FAIL;


    m_pTransformCom->SetState(CTransform::STATE_POSITION, XMLoadFloat4(&_pCamDes->vEye));
    m_pTransformCom->LookAt(XMLoadFloat4(&_pCamDes->vAt));

    return S_OK;
}

void CCamera::PriorityTick(_float fTimeDelta)
{
}

void CCamera::Tick(_float fTimeDelta)
{
}

void CCamera::LateTick(_float fTimeDelta)
{
}

HRESULT CCamera::Render()
{
    return S_OK;
}

HRESULT CCamera::SetUpTransformMatices()
{
    CGameInstance::GetInstance()->SetTransform(CPipeLine::D3DTS_VIEW, m_pTransformCom->GetWorldMatrixInverse());
    CGameInstance::GetInstance()->SetTransform(CPipeLine::D3DTS_PROJ, XMMatrixPerspectiveFovLH(m_fFovy,m_fAspect, m_fNear,m_fFar));

    return S_OK;
}
