#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class CSoundMgr
{
public:
	enum class CHANNELID {
		CH_BGM, CH_PLR_VO, CH_PLR_FX, CH_MON, CH_EFFECT, CH_END
	};

public:
	CSoundMgr();
	~CSoundMgr();


public:
	void Initialize();
	void Release();

public:
	void PlaySound(const TCHAR* _pSoundKey, CHANNELID eID, _float _fVolume);
	void PlayBGM(const TCHAR* _pSoundKey, _float _fVolume);
	void StopSound(CHANNELID eID);
	void StopAll();

private:
	void LoadSoundFile();

private:

	map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[(unsigned long long)CHANNELID::CH_END];
	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem;


public:

	static shared_ptr<CSoundMgr> Create();

};

END