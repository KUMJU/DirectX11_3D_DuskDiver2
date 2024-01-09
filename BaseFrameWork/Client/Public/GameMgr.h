#pragma once

#include "Client_Defines.h"
#include "Player.h"

class CGameMgr
{
	DECLARE_SINGLETON(CGameMgr)

private:
	CGameMgr();
	~CGameMgr()= default;

public:
	enum class EGameState { PROCESSING, PAUSE , SLOWMOTION };

public:

	void SwitchingGameState(EGameState _eState) { m_eCurrentGameState = _eState; };


public:
	shared_ptr<CPlayer> GetPlayer() { return m_pPlayer; }
	void SetPlayer(shared_ptr<CPlayer> _pPlr) { m_pPlayer = _pPlr; }

private:

	//�÷��̾� ���� ���
	EGameState m_eCurrentGameState = EGameState::PROCESSING;

	shared_ptr<CPlayer> m_pPlayer = nullptr;

	//Current Frame Timer : Ÿ�̸� �̸� ��Ͻ����ְ� �ʿ��� �� �ٲٰų� �ص� �ɵ� 

private:
	wrl::ComPtr<ID3D11Device> m_pDevice;
	wrl::ComPtr<ID3D11DeviceContext> m_pContext;


};

