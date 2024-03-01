#include "pch.h"
#include "SpecialBossPattern.h"

#include "GameInstance.h"

#include "FinalBoss.h"
#include "UI_SequenceTex.h"
#include "UIBackGround.h"

#include "GameMgr.h"
#include "Player.h"

#include "CameraMgr.h"
#include "UIMgr.h"

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
	TagInfo.fY = g_iWinSizeY * 0.5f + 200.f;

	CUI_SequenceTex::SequenceTexInfo TexInfo = {};
	TexInfo.bLoop = true;
	TexInfo.fScrollTime = 0.f;
	TexInfo.iCol = 16;
	TexInfo.iRow = 1;

	m_pGaugeUI = CUI_SequenceTex::Create(&TagInfo, TEXT("fx_boss_battle01"), 2, &TexInfo);
	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_UI"), m_pGaugeUI)))
		return E_FAIL;

	TagInfo.fSizeX = 80.f;
	TagInfo.fSizeY = 60.f;
	TagInfo.fX = g_iWinSizeX * 0.5f;
	TagInfo.fY = g_iWinSizeY * 0.5f + 240.f;

	m_pKeySpaceBarUI = CUIBackGround::Create(TagInfo, TEXT("btn_space"), 2);
	m_pKeySpaceBarUI->SetEnable(false);

	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_UI"), m_pKeySpaceBarUI)))
		return E_FAIL;

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

	if (m_bStartDone) {
		m_fAccTime += _fTimeDelta;

		if (m_fAccTime >= 0.1f && !m_bSetPosEvent) {

			dynamic_pointer_cast<CTransform>(m_pPlayer->GetComponent(TEXT("Com_Transform")))->SetState(CTransform::STATE_POSITION, { 0.f, 40.f, -410.f, 1.f });
			dynamic_pointer_cast<CTransform>(m_pPlayer->GetComponent(TEXT("Com_Transform")))->LookAtForLandObject({ 0.f, 42.5f, -425.f , 1.f });
			
			m_bSetPosEvent = true;
		}

		if (m_fAccTime >= 3.2f && !m_bCamFocusDone) {
			m_pPlayer->SetGaugingEffect(0);

			m_pGaugeUI->SetEnable(true);
			m_pKeySpaceBarUI->SetEnable(true);

			CCameraMgr::GetInstance()->SwitchingCamera(CCameraMgr::ECAMERATYPE::THIRDPERSON);
			CCameraMgr::GetInstance()->FocusingPlr({ 0.f, 42.5f, -415.f, 1.f });
			m_bCamFocusDone = true;
		}

	}


	if (m_bStartLastAtk) {

		if (m_fAccTime > 5.f && !m_bPlayerEndVoice) {
			CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_PLR_VO);
			CGameInstance::GetInstance()->PlayAudio(TEXT("Hero01_ba_65.wav"), CSoundMgr::CHANNELID::CH_PLR_VO, 1.3f);
			m_bPlayerEndVoice = true;
		}

		if (m_fAccTime > 5.9f && !m_bTimerSetting) {

			CGameInstance::GetInstance()->SetTimerOffset(TEXT("Timer_60"), 0.3f);
			CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_MONHIT);
			CGameInstance::GetInstance()->PlayAudio(TEXT("se_slowmotion_finish.wav"), CSoundMgr::CHANNELID::CH_MONHIT, 1.3f);
			m_bTimerSetting = true;
		}


	}

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
	/*Player*/
	m_pPlayer->SetOnFinalEvent(true);	
	CCameraMgr::GetInstance()->SetFreeCamPos({ 0.f, 43.f, -407.f, 1.f }, { 0.f, 42.5f, -425.f , 1.f });

	CUIMgr::GetInstance()->StartDialog(TEXT("LastAttack"));

	m_bStartDone = true;


}

void CSpecialBossPattern::StartEvent()
{
}

void CSpecialBossPattern::KeyInput()
{
	if (m_bKeyEventComplete)
		return;

	if (CGameInstance::GetInstance()->Key_Down(VK_SPACE)) {

		++m_iPressNum;

		if (3 == m_iPressNum) {

			m_iPressNum = 0;
			++m_iSuccessNum;
			m_pGaugeUI->SlideNextTexIdx();

			if (5 == m_iSuccessNum) {
				m_pPlayer->SetGaugingEffect(1);
				CCameraMgr::GetInstance()->SetFovLerp(40.f);
				CCameraMgr::GetInstance()->SetShakingMode(0.4f, 3.f, false);

			}

			if (10 == m_iSuccessNum) {
				m_pPlayer->SetGaugingEffect(2);
				CCameraMgr::GetInstance()->SetFovLerp(25.f);
				CCameraMgr::GetInstance()->SetShakingMode(1.f, 3.f, false);
			}
		}

		if (15 == m_iSuccessNum) {
			m_pKeySpaceBarUI->SetEnable(false);
			m_pGaugeUI->SetEnable(false);
			m_bKeyEventComplete = true;
			SuccessPattern();
		}

	}


}

void CSpecialBossPattern::SuccessPattern()
{
	//CCameraMgr::GetInstance()->SwitchDefaultCamMode(CThirdPersonCam::ECAM_DEFAULT);
	m_pPlayer->StartLastAttack();
	CCameraMgr::GetInstance()->StartPlrCamEvent(TEXT("FinalAttack"));

	m_fAccTime = 0.f;
	m_bStartLastAtk = true;
	//m_pFinalBoss.lock()->EndSpecialPattern();

}

shared_ptr<CSpecialBossPattern> CSpecialBossPattern::Create(shared_ptr<class CFinalBoss> _pBoss)
{
	shared_ptr<CSpecialBossPattern> pInstance = make_shared<CSpecialBossPattern>();

	if (FAILED(pInstance->Initialize(_pBoss)))
		MSG_BOX("Failed to Create : CFinalBoss");

	return pInstance;
}

