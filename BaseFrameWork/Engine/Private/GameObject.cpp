#include "GameObject.h"
#include "GameInstance.h"

CGameObject::CGameObject()
{
}

CGameObject::CGameObject(const CGameObject& _rhs)
{
}

HRESULT CGameObject::Initialize(CTransform::TRANSFORM_DESC* _pDesc)
{
	m_pTransformCom = CTransform::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo());

	if (!m_pTransformCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Initialize(_pDesc)))
		return E_FAIL;

	m_Components.emplace(TEXT("Com_Transform"), m_pTransformCom);                    

	return S_OK;
}

void CGameObject::PriorityTick(_float _fTimeDelta)
{


}

void CGameObject::Tick(_float _fTimeDelta)
{


}

void CGameObject::LateTick(_float _fTimeDelta)
{

}

HRESULT CGameObject::Render()
{


	return S_OK;
}

void CGameObject::Free()
{
}

HRESULT CGameObject::AddComponent(_uint _iLevelIndex, const wstring& _strProtoTag, const wstring& _strComTag, shared_ptr<CComponent>* _ppOut, shared_ptr<void> _pArg)
{
	return S_OK;
}

void CGameObject::SetPosition(_vector _vPos)
{
	m_pTransformCom->SetState(CTransform::STATE_POSITION, _vPos);

}

_matrix CGameObject::GetWorldMatrix()
{
	return m_pTransformCom->GetWorldMatrix(); 
}

_float* CGameObject::GetWoldMatFloatArr()
{
	return m_pTransformCom->GetWorldMatFloatArr();
}

void CGameObject::SetWorldMatrix(_float4x4 _worldMat)
{
	m_pTransformCom->SetWorldMatrix(_worldMat);
}

shared_ptr<CComponent> CGameObject::GetComponent(const wstring& _strComTag)
{
	auto iter = m_Components.find(_strComTag);

	if (m_Components.end() != iter) {
		return iter->second;
	}

	return nullptr;
}
