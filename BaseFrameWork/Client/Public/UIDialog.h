#pragma once

#include "UI.h"

BEGIN(Client)

class CUIDialog : public CUI
{
public:
	struct DialogInfo {
		string strPortraitKey;
		wstring strDialog;
	};


public:
	CUIDialog();
	virtual ~CUIDialog() = default;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public:

private:
	HRESULT LoadDescriptionData();
	

private:
	const wstring& m_strBasePath = TEXT("../Bin/DataFiles/DialogData/");
	
	void ReadData(const wstring& _strFullPath, const wstring& _strKeyName);
	
	_bool IsFileOrDir(_wfinddata_t _fd);
	wstring EraseExt(const wstring& _strFileName);

private:
	shared_ptr<class CTexture> m_pPortraitTexCom = nullptr;
	shared_ptr<CTransform> m_pPortraitTransCom = nullptr;


public:
	static shared_ptr<CUIDialog> Create();


};

END