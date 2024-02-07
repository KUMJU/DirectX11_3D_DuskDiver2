#pragma once

#include "UI.h"

BEGIN(Tool)

class CUIQuest : public CUI
{
public:
	CUIQuest();
	virtual ~CUIQuest() = default ;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void SetQuestDesc(const wstring& _strQuest) { strCurrentQuest = _strQuest; }

private:
	wstring strCurrentQuest = TEXT("");

public:
	static shared_ptr<CUIQuest> Create();

};

END