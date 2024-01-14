#include "pch.h"
#include "LevelLoading.h"

#include "UIBackGround.h"

#include "GameInstance.h"
#include "Loader.h"
#include "Logo.h"
#include "ArcadeMap.h"
#include "Edit.h"

CLevelLoading::CLevelLoading()
	:CLevel() 
{
}

CLevelLoading::~CLevelLoading()
{
	Free();
}

void CLevelLoading::PreInit()
{
}

HRESULT CLevelLoading::Initialize(LEVEL _eNextLevel)
{
	m_eNextLevel = _eNextLevel;

	if (FAILED(ReadyLayerBackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	/* 다음레벨에 대한 자원을 준비한다. */
	m_pLoader = CLoader::Create(_eNextLevel);
	if (nullptr == m_pLoader)
		return E_FAIL;

	return S_OK;
}

void CLevelLoading::Tick(_float _fTimeDelta)
{
	if (m_pLoader->isFinished()) {

		if (LEVEL_LOGO == m_eNextLevel) {
			OpenLevel();
		}
		else {
			//이걸 loader안으로 옮겨야할듯
			if (GetKeyState(VK_RETURN) & 0x8000)
			{
				OpenLevel();
			}
		}
	}


}

void CLevelLoading::LateTick()
{
}

HRESULT CLevelLoading::Render()
{
	if (nullptr == m_pLoader)
		return E_FAIL;

	m_pLoader->Output_LoadingText();

	return S_OK;
}

HRESULT CLevelLoading::ReadyLayerBackGround(const wstring& _strLayerTag)
{
	CUIBackGround::tagUIInfo UITagInfo;
	UITagInfo.fSizeX = g_iWinSizeX;
	UITagInfo.fSizeY = g_iWinSizeY;
	UITagInfo.fX = g_iWinSizeX * 0.5f;
	UITagInfo.fY = g_iWinSizeY * 0.5f;

	switch (m_eNextLevel)
	{
	case Client::LEVEL_LOGO:
		if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_LOADING, _strLayerTag, CUIBackGround::Create(UITagInfo, TEXT("title_BackGround"),0))))
			return E_FAIL;
		break;

	case Client::LEVEL_ARCADE:
		if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_LOADING, _strLayerTag, CUIBackGround::Create(UITagInfo, TEXT("Loading_E01"),0))))
			return E_FAIL;
		break;

	case Client::LEVEL_EDIT:
		if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_LOADING, _strLayerTag, CUIBackGround::Create(UITagInfo, TEXT("Loading_E02"),0))))
			return E_FAIL;
		break;

	case Client::LEVEL_END:
		break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CLevelLoading::ReadyLayerUI()
{
	return S_OK;
}

void CLevelLoading::OpenLevel()
{
	shared_ptr<CLevel> pLevel = nullptr;

	switch (m_eNextLevel)
	{
	case Client::LEVEL_LOGO:
		pLevel = CLogo::Create();
		break;
	case Client::LEVEL_ARCADE:
		pLevel = CArcadeMap::Create();
		break;
	case Client::LEVEL_EDIT:
		pLevel = CEdit::Create();
		break;
	case Client::LEVEL_END:
		break;
	default:
		break;
	}

	if (nullptr == pLevel)
		return;

	if (FAILED(CGameInstance::GetInstance()->OpenLevel(m_eNextLevel, pLevel)))
		return;

}

shared_ptr<CLevelLoading> CLevelLoading::Create(LEVEL _eNextLevel)
{
	shared_ptr<CLevelLoading> pInstance = make_shared<CLevelLoading>();

	if (FAILED(pInstance->Initialize(_eNextLevel))) {
		MSG_BOX("Faile Create : Clevel Loading");
	}

	return pInstance;
}

void CLevelLoading::Free()
{	delete m_pLoader;
}
