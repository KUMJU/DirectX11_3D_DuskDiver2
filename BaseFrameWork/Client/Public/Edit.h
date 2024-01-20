#pragma once

/* ImGui툴 출력용 level*/

#include "Level.h"
#include "Client_Defines.h"

BEGIN(Client)

class CEdit final : public CLevel
{
public:
	CEdit();
	virtual ~CEdit() = default;

public:
	virtual void PreInit();
	virtual HRESULT Initialize();
	virtual void Tick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT ReadyLayerBackGround(const wstring& _strLayerTag);
	HRESULT ReadyLayerCamera(const wstring& _strLayerTag);
	HRESULT ReadyLayerPlayer(const wstring& _strLayerTag);
	HRESULT ReadyLight();

public:

	static shared_ptr<CEdit> Create();


};

END