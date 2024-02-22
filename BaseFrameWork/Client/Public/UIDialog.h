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
	void StartDialog(const wstring& _strDialogKey);
	void ChangeNextData();

private:
	HRESULT LoadDescriptionData();

private:
	const wstring& m_strBasePath = TEXT("../Bin/DataFiles/DialogData/");
	
	void ReadData(const wstring& _strFullPath, const wstring& _strKeyName);
	void SettingDialogInfo();
	void ResetDialogInfo();

	_bool IsFileOrDir(_wfinddata_t _fd);
	wstring EraseExt(const wstring& _strFileName);

private:
	shared_ptr<class CTexture> m_pPortraitTexCom[3];
	shared_ptr<CTransform> m_pPortraitTransCom = nullptr;

	_float m_fAccTime = 0.f;
	 
	_int m_iCurrentDialogIdx = 0;
	_int m_iCurrentDialogLastIdx = 0;

	_int m_iCurrentPortaitIdx = 0;

private:
	map<wstring, vector<DialogInfo>> m_Scripts;
	vector<DialogInfo>* m_CurrentDialog;
public:
	static shared_ptr<CUIDialog> Create();


};

END