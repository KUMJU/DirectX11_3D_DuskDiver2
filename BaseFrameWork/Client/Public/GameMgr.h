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
		//직접 써보고 고쳐야할 수도 있음
		_pDevice = &m_pDevice;
		_pContext = &m_pContext;
	}

private:

	//플레이어 정보 등록
	EGameState m_eCurrentGameState = EGameState::PROCESSING;

	//Current Frame Timer : 타이머 이름 등록시켜주고 필요할 때 바꾸거나 해도 될듯 

private:
	wrl::ComPtr<ID3D11Device> m_pDevice;
	wrl::ComPtr<ID3D11DeviceContext> m_pContext;


};

