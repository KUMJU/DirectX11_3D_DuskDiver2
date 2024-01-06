#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CLevel
{
public:
	CLevel();
	virtual ~CLevel();

public:
	virtual void PreInit(); 
	virtual HRESULT Initialize();
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick();
	virtual HRESULT Render();

protected:

public:
	void Free();

};

END
