#include "InputDevice.h"

Engine::CInputDevice::CInputDevice(void)
{

}

_bool CInputDevice::Key_Down(_ubyte eKeyID)
{
	if (m_preKeyState[eKeyID] != m_byKeyState[eKeyID])
	{
		if (m_byKeyState[eKeyID] & 0x80)
			return true;
	}
	return false;
}

_bool CInputDevice::Key_Up(_ubyte eKeyID)
{
	if (m_preKeyState[eKeyID] != m_byKeyState[eKeyID])
	{
		if (m_preKeyState[eKeyID] & 0x80)
			return true;
	}
	return false;
}

_bool CInputDevice::Key_Pressing(_ubyte eKeyID)
{
	if (m_preKeyState[eKeyID] == m_byKeyState[eKeyID])
	{
		if (m_preKeyState[eKeyID] & 0x80)
			return true;
	}
	return false;
}

HRESULT Engine::CInputDevice::Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{

	// DInput 컴객체를 생성하는 함수
	if (FAILED(DirectInput8Create(hInst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pInputSDK,
		nullptr)))
		return E_FAIL;

	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pKeyBoard->Acquire();

	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	m_pMouse->SetDataFormat(&c_dfDIMouse);
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pMouse->Acquire();


	return S_OK;
}

void Engine::CInputDevice::Tick(void)
{
	memcpy(&m_preKeyState, &m_byKeyState, sizeof(_ubyte) * MAX_BONE);

	m_pKeyBoard->GetDeviceState(MAX_BONE, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);
}

shared_ptr<CInputDevice> CInputDevice::Create(HINSTANCE hInst, HWND hWnd)
{
	shared_ptr<CInputDevice>	pInstance = make_shared<CInputDevice>();

	if (FAILED(pInstance->Ready_InputDev(hInst, hWnd)))
		MSG_BOX("Failed to Created : CInput_Device");

	return pInstance;
}

