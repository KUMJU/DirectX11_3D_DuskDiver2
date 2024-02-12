#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Engine)
class CGameObject;

END


BEGIN(Client)

class CArcadeMap : public CLevel
{
public:
	CArcadeMap();
	virtual ~CArcadeMap();

public:
	virtual void PreInit();
	virtual HRESULT Initialize();
	virtual void Tick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT ReadyLayerBackGround(const wstring& _strLayerTag);
	HRESULT ReadyLayerCamera(const wstring& _strLayerTag);
	HRESULT ReadyLayerMap(const wstring& _strLayerTag);

	HRESULT ReadyLayerPlayer(const wstring& _strLayerTag);
	HRESULT ReadyLayerMonster(const wstring& _strLayerTag);

	HRESULT ReadyLayerEvent(const wstring& _strLayerTag);

	HRESULT ReadyLandObjects();

	HRESULT ReadyLayerUI(const wstring& _strLayerTag);
	HRESULT ReadyLight();

private:
	void KeyInput();

	shared_ptr<CGameObject> pMonster1;
	shared_ptr<CGameObject> pMonster2;
	shared_ptr<CGameObject> pMonster3;
	shared_ptr<CGameObject> pMonster4;

private:
	_bool m_bKeyDeb = false;
	_float m_fTime = 0.f;

public:
	static shared_ptr<CArcadeMap> Create();
};

END