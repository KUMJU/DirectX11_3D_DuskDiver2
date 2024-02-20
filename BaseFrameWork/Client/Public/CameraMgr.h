#pragma once

#include "Client_Defines.h"
#include "EventCamera.h"


BEGIN(Engine)

class CGameObject;
class CCamera;
END


BEGIN(Client)


class CCameraMgr
{
	DECLARE_SINGLETON(CCameraMgr)

public:
	enum class ECAMERATYPE {
		THIRDPERSON,
		FREE,
		EVENT,
		ENUM_END
	};

private:
	CCameraMgr();
	~CCameraMgr() = default;

public:
	void SwitchingCamera(ECAMERATYPE _eType);
	void SetCamObject(ECAMERATYPE _eType, shared_ptr<CGameObject> _pInstance);

public:
	_float4 GetCamLook();
	_vector GetCamPos();


public:
	void FocusPlayer(_vector _vPos , _float _fHeight);

////////////////////////////이벤트 카메라////////////////////////////
public:
	void AddEventPreset(const wstring& _strName, vector<CEventCamera::EVENT_INFO> _info);
	void StartEvent(const wstring& _strName);

////////////////////////////프리 카메라////////////////////////////
public:
	void SetFreeCamPos(_vector _vPos, _vector _vLook);


private:
	ECAMERATYPE m_eCurrentCamType = ECAMERATYPE::THIRDPERSON;

private:
	shared_ptr<CCamera> m_pMainCam = nullptr;

	shared_ptr<class CThirdPersonCam> m_pDefualtCam = nullptr;
	shared_ptr<class CCameraFree> m_pFreeCam = nullptr;
	shared_ptr<class CEventCamera> m_pEventCam = nullptr;
	
};

END