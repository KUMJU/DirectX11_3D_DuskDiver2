#include "Layers.h"

#include "GameObject.h"

CLayers::CLayers()
{
}

CLayers::~CLayers()
{
}

HRESULT CLayers::Initialize()
{
	return S_OK;
}

HRESULT CLayers::AddGameObject(shared_ptr<class CGameObject> _pGameObject)
{
	if (nullptr == _pGameObject)
		return E_FAIL;

	m_GameObjects.push_back(_pGameObject);

	return S_OK;
}

void CLayers::PriorityTick(_float _fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects) {

		if (nullptr != pGameObject) {
			pGameObject->PriorityTick(_fTimeDelta);
		}
	}
}

void CLayers::Tick(_float _fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects) {

		if (nullptr != pGameObject) {
			pGameObject->Tick(_fTimeDelta);
		}
	}

}

void CLayers::LateTick(_float _fTimeDelta)
{

	for (auto iter = m_GameObjects.begin(); iter != m_GameObjects.end(); ) {

		//Dead Check
		if (!((*iter)->IsActive())) {
			iter->reset();
			iter = m_GameObjects.erase(iter);
			continue;
		}

		(*iter)->LateTick(_fTimeDelta);
		iter++;
	}

}

shared_ptr<CLayers> CLayers::Create()
{
	shared_ptr<CLayers> pInstance = make_shared<CLayers>();

	if (FAILED(pInstance->Initialize())) {
		MSG_BOX("Failed to Create : CLayer");
	}

	return pInstance;
}
