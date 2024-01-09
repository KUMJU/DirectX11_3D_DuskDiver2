#pragma once

#include "Client_Defines.h"

BEGIN(Engine)
class CGameInstance;
END


BEGIN(Client)

class CMainApp final
{
public:
	CMainApp();
	~CMainApp();

public:
	void PreInit();
	HRESULT Initialize();
	void Tick(_float _fTimeDelta);
	HRESULT Render();

private:
	HRESULT OpenLevel(LEVEL _eStartLevel);	 


private:
	_bool m_IsImguiOn = true;


public:
	void Free();
	HRESULT ReadyGara();
};

END