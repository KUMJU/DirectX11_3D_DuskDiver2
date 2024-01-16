#pragma once

#include "Client_Defines.h"

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

private:
	ECAMERATYPE m_eCurrentCamType = ECAMERATYPE::THIRDPERSON;

private:
	shared_ptr<CCamera> m_pMainCam = nullptr;

	shared_ptr<class CThirdPersonCam> m_pDefualtCam = nullptr;
	shared_ptr<class CCameraFree> m_pFreeCam = nullptr;


};

END