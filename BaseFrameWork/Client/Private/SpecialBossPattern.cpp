#include "pch.h"
#include "SpecialBossPattern.h"

#include "GameInstance.h"

#include "FinalBoss.h"
#include "UI_SequenceTex.h"

#include "GameMgr.h"
#include "Player.h"

CSpecialBossPattern::CSpecialBossPattern()
{
}

HRESULT CSpecialBossPattern::Initialize(shared_ptr<class CFinalBoss> _pBoss)
{

	m_pFinalBoss = _pBoss;
	m_IsEnabled = false;

	//UI
	CUI::tagUIInfo TagInfo = {};
	TagInfo.fSizeX = 700.f;
	TagInfo.fSizeY = 90.f;
	TagInfo.fX = g_iWinSizeX * 0.5f;
	TagInfo.fY = g_iWinSizeY * 0.5f;

	CUI_SequenceTex::SequenceTexInfo TexInfo = {};
	TexInfo.bLoop = true;
	TexInfo.fScrollTime = 0.f;
	TexInfo.iCol = 16;
	TexInfo.iRow = 1;

	m_pGaugeUI = CUI_SequenceTex::Create(&TagInfo, TEXT("fx_boss_battle01"), 2, &TexInfo);
	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_UI"), m_pGaugeUI)))
		return E_FAIL;

	//Player 

	m_pPlayer = CGameMgr::GetInstance()->GetPlayer();


	return S_OK;
}

void CSpecialBossPattern::PriorityTick(_float _fTimeDelta)
{
	if (!m_IsEnabled)
		return;


}

void CSpecialBossPattern::Tick(_float _fTimeDelta)
{
	if (!m_IsEnabled)
		return;

	KeyInput();
}

void CSpecialBossPattern::LateTick(_float _fTimeDelta)
{
	if (!m_IsEnabled)
		return;

}

HRESULT CSpecialBossPattern::Render()
{
	return S_OK;
}

void CSpecialBossPattern::PatternStart()
{
	m_IsEnabled = true;
	m_pGaugeUI->SetEnable(true);
	m_pPlayer->SetOnFinalEvent(true);

}

void CSpecialBossPattern::StartEvent()
{
}

void CSpecialBossPattern::KeyInput()
{
	if (CGameInstance::GetInstance()->Key_Down(VK_SPACE)) {

		++m_iPressNum;

		if (3 == m_iPressNum) {
			m_iPressNum = 0;
			++m_iSuccessNum;
			m_pGaugeUI->SlideNextTexIdx();
		}

		if (15 == m_iSuccessNum) {
			SuccessPattern();
		}

	}


}

void CSpecialBossPattern::SuccessPattern()
{

	m_pFinalBoss.lock()->EndSpecialPattern();

}

shared_ptr<CSpecialBossPattern> CSpecialBossPattern::Create(shared_ptr<class CFinalBoss> _pBoss)
{
	shared_ptr<CSpecialBossPattern> pInstance = make_shared<CSpecialBossPattern>();

	if (FAILED(pInstance->Initialize(_pBoss)))
		MSG_BOX("Failed to Create : CFinalBoss");

	return pInstance;
}

