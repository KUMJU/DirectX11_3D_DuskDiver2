#pragma once

#include"Client_Defines.h"

BEGIN(Client)

class CEffectMgr
{
	DECLARE_SINGLETON(CEffectMgr)


public:
	CEffectMgr();
	virtual ~CEffectMgr() = default;

public:
	void LoadEffectPreset();
	void GetEffect();
	shared_ptr<class CEffectPreset> FindEffect(const wstring& _strKeyName);

	void SetHitMark(_vector _vPos);


private:
	void ReadData(const wstring&  _strFullPath, const wstring& _strKeyName);


private:
	const wstring& m_strBasePath = TEXT("../Bin/DataFiles/EffectData/");
	_bool IsFileOrDir(_wfinddata_t _fd);
	wstring EraseExt(const wstring& _strFileName);


private:

	map<wstring, shared_ptr<class CEffectPreset>> m_EffectPresets;
	vector<shared_ptr<class CEffectPreset>> m_HitMarks;

};

END