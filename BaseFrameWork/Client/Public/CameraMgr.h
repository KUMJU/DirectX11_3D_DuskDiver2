#pragma once

#include "Client_Defines.h"
#include "EventCamera.h"
#include "ThirdPersonCam.h"


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
	void SwitchDefaultCamMode(CThirdPersonCam::ECAMSTATE _eCamState);


public:
	void SetBattleZoom(_float _fAccTime, _float _fDstFov);

public:
	void SetShakingMode(_float _fShakingPow, _float _fShakingTime, _bool m_bXDir);
	void SetFov(_float _fFov);
	void SetLerpMoving(_vector _vPos, _float _fAccTime);
	void SetLerpMovingBack( _float _fAccTime);

public:
	void FocusPlayer(_vector _vPos , _float _fHeight);
	void StartPlrCamEvent(const wstring& _strEventKey);
	void FocusingPlr(_vector vCamPos);
	void SetFovLerp(_float _fDstFovy);

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