#include "ObjectManager.h"
#include "GameObject.h"
#include "Layers.h"

CObjectManager::CObjectManager()
{
}

CObjectManager::~CObjectManager()
{
	Free();
}

HRESULT CObjectManager::Initialize(_uint _iNumLevels)
{
	m_pLayers = new LAYERS[_iNumLevels];
	m_iNumLevels = _iNumLevels;

	return S_OK;
}

void CObjectManager::PriorityTick(_float fTimeDelta)
{


	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->PriorityTick(fTimeDelta);
	}
}

void CObjectManager::Tick(_float fTimeDelta)
{


	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Tick(fTimeDelta);
	}
}

void CObjectManager::LateTick(_float fTimeDelta)
{

	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->LateTick(fTimeDelta);
	}
}

HRESULT CObjectManager::AddObject(_uint iLevelIndex, const wstring& strLayerTag, shared_ptr<CGameObject> _pGameObject)
{
	if (!_pGameObject)
		return E_FAIL;

	shared_ptr<CLayers> pLayer = FindLayer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CLayers::Create();

		if (!pLayer)
			return E_FAIL;

		pLayer->AddGameObject(_pGameObject);
		m_pLayers[iLevelIndex].emplace(strLayerTag, pLayer);

	}
	else {
		pLayer->AddGameObject(_pGameObject);
	}


	return S_OK;
}



shared_ptr<class CLayers> CObjectManager::FindLayer(_uint _iLevelIndex, const wstring& _strLayerTag)
{
	if (_iLevelIndex >= m_iNumLevels || m_pLayers[_iLevelIndex].empty())
		return nullptr;

	auto iter = m_pLayers[_iLevelIndex].find(_strLayerTag);

	if (m_pLayers[_iLevelIndex].end() == iter)
		return nullptr;

	return iter->second;
}

shared_ptr<CObjectManager> CObjectManager::Create(_uint iNumLevels)
{
	shared_ptr<CObjectManager> pInstance = make_shared<CObjectManager>();

	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX("Failed to Created : CObject_Manager");
	}

	return pInstance;
}

void CObjectManager::Clear(_uint _iLevelNum)
{
	m_pLayers[_iLevelNum].clear();
}

void CObjectManager::Free()
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		m_pLayers[i].clear();
	}
}
