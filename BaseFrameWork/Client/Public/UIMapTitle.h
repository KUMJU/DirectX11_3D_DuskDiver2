#pragma once

#include "UI.h"


BEGIN(Client)

class CUIMapTitle : public CUI
{
public:
	CUIMapTitle();
	virtual ~CUIMapTitle() = default;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void StartQuest(const wstring& _strQuestInfo);

private:
	_float m_fAccTime = 0.f;

private:
	shared_ptr<class CUI_SequenceTex> m_pEffectTex = nullptr;
	wstring m_strQuiestInfo = TEXT("");

public:
	static shared_ptr<CUIMapTitle> Create();

};

END