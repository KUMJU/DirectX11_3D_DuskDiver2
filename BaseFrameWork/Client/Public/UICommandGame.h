#pragma once
#include "UI.h"

BEGIN(Client)

class CUICommandGame : public CUI
{
public:
	CUICommandGame();
	virtual ~CUICommandGame() = default;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	//게이지바
	//키 입력 설명 




private:


public:
	static shared_ptr<CUICommandGame> Create();


};

END