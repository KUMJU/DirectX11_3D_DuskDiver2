#include "SoundMgr.h"
#include <io.h>

CSoundMgr::CSoundMgr()
{
}

CSoundMgr::~CSoundMgr()
{
	Release();
}

void CSoundMgr::Initialize()
{
	FMOD_System_Create(&m_pSystem, FMOD_VERSION);
	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);

	LoadSoundFile();
}

void CSoundMgr::Release()
{
	for (auto& Mypair : m_mapSound)
	{
		delete[] Mypair.first;
		FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound.clear();
	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}

void CSoundMgr::PlaySound(const TCHAR* _pSoundKey, CHANNELID eID, _float _fVolume)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
		{
			return !lstrcmp(_pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE;
	if (FMOD_Channel_IsPlaying(m_pChannelArr[(unsigned long long)eID], &bPlay))
	{
		FMOD_System_PlaySound(m_pSystem, iter->second, NULL, FALSE, &m_pChannelArr[(unsigned long long)eID]);
		FMOD_Channel_SetVolume(m_pChannelArr[(unsigned long long)eID], _fVolume);

	}
	FMOD_System_Update(m_pSystem);

}

void CSoundMgr::PlayBGM(const TCHAR* _pSoundKey, _float _fVolume)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
		{
			return !lstrcmp(_pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(m_pSystem, iter->second, FALSE, NULL, &m_pChannelArr[(unsigned long long)CHANNELID::CH_BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[(unsigned long long)CHANNELID::CH_BGM], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pChannelArr[(unsigned long long)CHANNELID::CH_BGM], _fVolume);
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::StopSound(CHANNELID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[(unsigned long long)eID]);

}

void CSoundMgr::StopAll()
{
	for (int i = 0; i < (unsigned long long)CHANNELID::CH_END; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSoundMgr::LoadSoundFile()
{
	_finddata_t fd;

	intptr_t handle = _findfirst("../../Client/Bin/Resources/Audio/*.wav", &fd);

	if (handle == -1)
		return;

	int iResult = 0;

	char szCurPath[128] = "../../Client/Bin/Resources/Audio/";
	char szFullPath[128] = "";

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);
		strcat_s(szFullPath, fd.name);
		FMOD_SOUND* pSound = nullptr;

		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_DEFAULT, 0, &pSound);

		if (eRes == FMOD_OK)
		{
			int iLength = strlen(fd.name) + 1;

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);
			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

			m_mapSound.emplace(pSoundKey, pSound);

		}

		iResult = _findnext(handle, &fd);
	}

	FMOD_System_Update(m_pSystem);
	_findclose(handle);
}

shared_ptr<CSoundMgr> CSoundMgr::Create()
{
	shared_ptr<CSoundMgr> pInstance = make_shared<CSoundMgr>();
	pInstance->Initialize();

	return pInstance;
}
