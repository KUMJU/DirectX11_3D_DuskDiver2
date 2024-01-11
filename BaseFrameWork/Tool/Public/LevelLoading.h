#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevelLoading : public CLevel
{
public:
	CLevelLoading();
	virtual ~CLevelLoading();

public:

	virtual void PreInit() override;
	virtual HRESULT Initialize(LEVEL _eNextLevel);
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick() override;

	virtual HRESULT Render() override;

public:

	HRESULT ReadyLayerBackGround(const wstring& _strLayerTag);
	HRESULT ReadyLayerUI();

	void OpenLevel();

private:
	class CLoader* m_pLoader = { nullptr };
	LEVEL m_eNextLevel = { LEVEL_END };

public:
	static shared_ptr<CLevelLoading> Create( LEVEL _eNextLevel);
	void Free();

};

END