#include "Transform.h"
#include "Shader.h"
#include "Navigation.h"

CTransform::CTransform(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
    :CComponent(_pDevice, _pContext)
{
}

CTransform::~CTransform()
{
}

void CTransform::SetScaling(_float _fX, _float _fY, _float _fZ)
{
    SetState(STATE_RIGHT, XMVector3Normalize(GetState(STATE_RIGHT)) * _fX);
    SetState(STATE_UP, XMVector3Normalize(GetState(STATE_UP)) * _fY);
    SetState(STATE_LOOK, XMVector3Normalize(GetState(STATE_LOOK)) * _fZ);
}

void CTransform::CheckingMove(_fvector _vPosition, shared_ptr<CNavigation> _pNavigation, _bool& _IsJump)
{
    _float fHeight = 0.f;

    _vector vNewPos = _vPosition;

    if (nullptr == _pNavigation) {
        SetState(STATE_POSITION, vNewPos);


    }
    else if (true == _pNavigation->IsMove(vNewPos, fHeight)) {

        if (_IsJump) {
            if (fHeight >= vNewPos.m128_f32[1]) {
                vNewPos.m128_f32[1] = fHeight + m_fYOffset;
                _IsJump = false;
            }

            SetState(STATE_POSITION, vNewPos);

        }
        else {
            vNewPos.m128_f32[1] = fHeight+ m_fYOffset;
            SetState(STATE_POSITION, vNewPos);
        }
    }


}

void CTransform::GoStraight(_float _fTimeDelta, shared_ptr<CNavigation> _pNavigation, _bool& _bJump)
{
    _vector vPosition = GetState(STATE_POSITION);
    _vector vLook = GetState(STATE_LOOK);

    vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * _fTimeDelta;
    _float fHeight = 0.f;

    CheckingMove(vPosition, _pNavigation, _bJump);


}

void CTransform::GoBackward(_float _fTimeDelta)
{
    _vector vPosition = GetState(STATE_POSITION);
    _vector vLook = GetState(STATE_LOOK);

    vPosition -= XMVector3Normalize(vLook) * m_fSpeedPerSec * _fTimeDelta;

    SetState(STATE_POSITION, vPosition);

}

void CTransform::GoLeft(_float _fTimeDelta)
{
    _vector vPosition = GetState(STATE_POSITION);
    _vector vRightOrigin = GetState(STATE_RIGHT);

    _vector vLook = vRightOrigin * -1.f;
    _vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
    _vector vUp = XMVector3Cross(vLook, vRight);

    vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * _fTimeDelta;

    SetState(STATE_POSITION, vPosition);

}

void CTransform::GoRight(_float _fTimeDelta, shared_ptr<CNavigation> _pNavigation, _bool _bJump)
{
    _vector vPosition = GetState(STATE_POSITION);

    _vector vRightOrigin = GetState(STATE_RIGHT);

    _vector vLook = vRightOrigin;
    _vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
    _vector vUp = XMVector3Cross(vLook, vRight);

    vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * _fTimeDelta;

    CheckingMove(vPosition, _pNavigation, _bJump);

}

void CTransform::Turn(_fvector _vAxis, _float _fTimeDelta)
{
    _matrix RotationMatrix = XMMatrixRotationAxis(_vAxis, m_fRotationPerSet * _fTimeDelta);

    for (size_t i = STATE_RIGHT; i < STATE_POSITION; i++) {

        _vector vStateDir = GetState(STATE(i));
        SetState(STATE(i), XMVector4Transform(vStateDir, RotationMatrix));
    }
}

void CTransform::TurnPlayer(_vector _vAxis, _float _fAngle, _float _fTimeDelta)
{
    _matrix RotationMatrix = XMMatrixRotationAxis(_vAxis, _fAngle * _fTimeDelta);

    for (size_t i = STATE_RIGHT; i < STATE_POSITION; i++) {

        _vector vStateDir = GetState(STATE(i));
        SetState(STATE(i), XMVector4Transform(vStateDir, RotationMatrix));
    }
}

void CTransform::Rotation(_fvector _vAxis, _float _fRadian)
{

    _matrix RotationMatrix = XMMatrixRotationAxis(_vAxis, _fRadian);
    _float3 vScaled = GetScaled();

    for (size_t i = STATE_RIGHT; i < STATE_POSITION; i++) {

        _float4 vTemp = _float4(0.f, 0.f, 0.f, 0.f);
        
        *((_float*)&vTemp + i) = 1.f * *((_float*)&vScaled + i);

        _vector vStateDir = XMLoadFloat4(&vTemp);
        SetState(STATE(i), XMVector4Transform(vStateDir, RotationMatrix));
    }
}

void CTransform::MoveTo(_fvector _vPoint, _float _fLimit, _float _fTimeDelta)
{
    _vector vPosition = GetState(STATE_POSITION);
    _vector vDir = _vPoint - vPosition;

    _float fDistance = XMVector3Length(vDir).m128_f32[0];

    if (fDistance > _fLimit)
        vPosition += XMVector3Normalize(vDir) * m_fSpeedPerSec * _fTimeDelta;
}

void CTransform::RotaitionRollYawPitch(_float _fRadianX, _float _fRadianY, _float _fRadianZ)
{
    _matrix RotationMatrix =  XMMatrixRotationRollPitchYaw(_fRadianX, _fRadianY, _fRadianZ);
    _float3 vScaled = GetScaled();

    for (size_t i = STATE_RIGHT; i < STATE_POSITION; i++) {

        _float4 vTemp = _float4(0.f, 0.f, 0.f, 0.f);

        *((_float*)&vTemp + i) = 1.f * *((_float*)&vScaled + i);

        _vector vStateDir = XMLoadFloat4(&vTemp);
        SetState(STATE(i), XMVector4Transform(vStateDir, RotationMatrix));
    }

}

void CTransform::LookAt(_fvector _vPoint)
{
    _float3 vScaled = GetScaled();

    _vector vPosition = GetState(CTransform::STATE_POSITION);

    _vector vLook = _vPoint - vPosition;
    _vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f),vLook);
    _vector vUp = XMVector3Cross(vLook, vRight);

    SetState(STATE_RIGHT, XMVector3Normalize(vRight) * vScaled.x);
    SetState(STATE_UP, XMVector3Normalize(vUp) * vScaled.y);
    SetState(STATE_LOOK, XMVector3Normalize(vLook) * vScaled.z);

}

void CTransform::LookAtForLandObject(_fvector _vPoint)
{
    _float3 vScaled = GetScaled();

    _vector vPosition = GetState(CTransform::STATE_POSITION);

    _vector	vLook = _vPoint - vPosition;
    _vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
    vLook = XMVector3Cross(vRight, XMVectorSet(0.f, 1.f, 0.f, 0.f));

    SetState(STATE_RIGHT, XMVector3Normalize(vRight) * vScaled.x);
    SetState(STATE_LOOK, XMVector3Normalize(vLook) * vScaled.z);

}


HRESULT CTransform::Initialize(TRANSFORM_DESC* _pDesc)
{
    XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

    if (!_pDesc)
        return S_OK;

    m_fSpeedPerSec = _pDesc->fSpeedPerSet;
    m_fRotationPerSet = _pDesc->fRotationPerSet;

    return S_OK;
}

HRESULT CTransform::BindShaderResource(shared_ptr<class CShader> _pShader, const _char* _pContantName)
{
    return _pShader->BindMatrix(_pContantName, &m_WorldMatrix);
}

shared_ptr<CTransform> CTransform::Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, TRANSFORM_DESC* _pDesc)
{
    shared_ptr<CTransform> pInstance = make_shared<CTransform>(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize(_pDesc)))
        MSG_BOX("Failed to Create : CTransform");

    return pInstance;
}

