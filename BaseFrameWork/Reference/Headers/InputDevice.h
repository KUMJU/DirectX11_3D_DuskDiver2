#ifndef InputDev_h__
#define InputDev_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class CInputDevice 
{
public:
	CInputDevice(void);
	virtual ~CInputDevice(void) = default;
	
public:
	_byte	Get_DIKeyState(_ubyte byKeyID)			{ return m_byKeyState[byKeyID]; }
	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse) 	{ 	return m_tMouseState.rgbButtons[eMouse]; 	}

	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)	
	{	
		return *(((_long*)&m_tMouseState) + eMouseState);	
	}
	
public:
	HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);
	void	Tick(void);

private:

	wrl::ComPtr<IDirectInput8> m_pInputSDK = nullptr;

private:

	wrl::ComPtr<IDirectInputDevice8> m_pKeyBoard = nullptr;
	wrl::ComPtr<IDirectInputDevice8> m_pMouse = nullptr;

private:
	_byte					m_byKeyState[256];		// 키보드에 있는 모든 키값을 저장하기 위한 변수
	DIMOUSESTATE			m_tMouseState;	

public:
	static shared_ptr<CInputDevice> Create(HINSTANCE hInst, HWND hWnd);

};
END
#endif // InputDev_h__
