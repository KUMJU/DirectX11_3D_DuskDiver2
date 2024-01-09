#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class CSoundMgr
{
public:
	enum class CHANNELID {
		BGM, PLAYER, ENUM_END
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
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[(unsigned long long)CHANNELID::ENUM_END];
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem;

};

END