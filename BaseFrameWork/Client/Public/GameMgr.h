#pragma once

#include "Client_Defines.h"

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
	void SetDeviceInfo(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext) 
	{
		m_pDevice = _pDevice;
		m_pContext = _pContext;
	}

	void GetDeviceInfo(wrl::ComPtr<ID3D11Device>* _pDevice, wrl::ComPtr<ID3D11DeviceContext>* _pContext) {
		//���� �Ẹ�� ���ľ��� ���� ����
		_pDevice = &m_pDevice;
		_pContext = &m_pContext;
	}

private:

	//�÷��̾� ���� ���
	EGameState m_eCurrentGameState = EGameState::PROCESSING;

	//Current Frame Timer : Ÿ�̸� �̸� ��Ͻ����ְ� �ʿ��� �� �ٲٰų� �ص� �ɵ� 

private:
	wrl::ComPtr<ID3D11Device> m_pDevice;
	wrl::ComPtr<ID3D11DeviceContext> m_pContext;


};

