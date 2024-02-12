#include "pch.h"
#include "EffectMgr.h"

#include "Json/json.h"
#include "Json/json-forwards.h"

#include <io.h>
#include <fstream>


#include "EffectPreset.h"

#include "EffectParticle.h"
#include "EffectTexture.h"
#include "EffectMesh.h"

#include "VIBufferPoint.h"

#include "GameInstance.h"

IMPLEMENT_SINGLETON(CEffectMgr)

CEffectMgr::CEffectMgr()
{
}

void CEffectMgr::LoadEffectPreset()
{
	
    wstring strExt = TEXT("*.*");
    wstring strFullPath = m_strBasePath + strExt;

    _wfinddata_t fd;
    intptr_t lHandle;

    if (-1 == (lHandle = _wfindfirst(strFullPath.c_str(), &fd)))
        return;

    while (0 == _wfindnext(lHandle, &fd))
    {
        const wstring& strName = fd.name;

        if (IsFileOrDir(fd) && fd.size != 0 && fd.name[0] != '.') {
            wstring strName = fd.name;
            wstring strExt = EraseExt(strName);
            wstring strFilePath = m_strBasePath + strName;
            
            ReadData(strFilePath, strExt);
        }
    }

    _findclose(lHandle);

}


_bool CEffectMgr::IsFileOrDir(_wfinddata_t _fd)
{
    if (_fd.attrib & _A_SUBDIR)
        return false; //디렉토리
    else
        return true; //파일

}

wstring CEffectMgr::EraseExt(const wstring& _strFileName)
{
    size_t iCount = _strFileName.find_last_of(TEXT("."));
    return _strFileName.substr(0, iCount);
}

void CEffectMgr::GetEffect()
{
}

shared_ptr<CEffectPreset> CEffectMgr::FindEffect(const wstring& _strKeyName)
{
    auto iter = m_EffectPresets.find(_strKeyName);
    shared_ptr<CEffectPreset> pPreset = nullptr;
    pPreset = iter->second;

    //if (iter != m_EffectPresets.end()) {
    //    pPreset = iter->second;
    //}

    return pPreset;

}

void CEffectMgr::ReadData(const wstring& _strFullPath, const wstring& _strKeyName)
{
    ifstream in(_strFullPath);

    Json::Value root;

    in >> root;

    _int iElementNum = root["Elements"].size();

    _float fDuration = root["Preset Info"]["Duration"].asFloat();
    _bool bLoop = root["Preset Info"]["Loop"].asBool();
   

    shared_ptr<CEffectPreset> pPreset =  CEffectPreset::Create(fDuration, bLoop);
    pPreset->SetEnable(false);

    auto iter = root["Elements"].begin();

    for (size_t i = 0; i < iElementNum; ++i) {

        string keyName = iter.key().asString();

        Json::Value ElementKey = root["Elements"][keyName.c_str()];
        _int iEffectType = ElementKey["EffectType"].asInt();

        //Particle
        if (0 == iEffectType) {

            CVIBufferInstancing::INSTANCE_DESC desc = {};

            _uint iInstanceNum = ElementKey["InstanceNum"].asUInt();
            string strTextureKey = ElementKey["TextureKey"].asString();

            desc.isLoop = bLoop;
            desc.vCenter = { ElementKey["Center"]["x"].asFloat(),ElementKey["Center"]["y"].asFloat() ,ElementKey["Center"]["z"].asFloat()};
            desc.vColor = { ElementKey["Color"]["x"].asFloat(),ElementKey["Color"]["y"].asFloat() ,ElementKey["Color"]["z"].asFloat(), 1.f };
            desc.vDirectionMax = { ElementKey["DirectMax"]["x"].asFloat(),ElementKey["DirectMax"]["y"].asFloat() ,ElementKey["DirectMax"]["z"].asFloat() };
            desc.vDirectionMin = { ElementKey["DirectMin"]["x"].asFloat(),ElementKey["DirectMin"]["y"].asFloat() ,ElementKey["DirectMin"]["z"].asFloat() };
            desc.vLifeTime = { ElementKey["LifeTime"]["x"].asFloat(),ElementKey["LifeTime"]["y"].asFloat()};
            desc.vSize = { ElementKey["Size"]["x"].asFloat(),ElementKey["Size"]["y"].asFloat()};
            desc.vStartPointMax = { ElementKey["StartPointMax"]["x"].asFloat(),ElementKey["StartPointMax"]["y"].asFloat(),ElementKey["StartPointMax"]["z"].asFloat() };
            desc.vStartPointMin = { ElementKey["StartPointMin"]["x"].asFloat(),ElementKey["StartPointMin"]["y"].asFloat(),ElementKey["StartPointMin"]["z"].asFloat() };
            desc.vDuration.x = 5.f;
            desc.vDuration.y = 15.f;
            desc.vSpeed = { ElementKey["Speed"]["x"].asFloat(),ElementKey["Speed"]["y"].asFloat()};
            
            _tchar szFullPath[MAX_PATH] = TEXT("");
            MultiByteToWideChar(CP_ACP, 0, strTextureKey.c_str(), (_int)strlen(strTextureKey.c_str()), szFullPath, MAX_PATH);

            
            shared_ptr<CEffectParticle> pParticle =  CEffectParticle::Create(iInstanceNum, szFullPath, &desc, const_cast<char*>(keyName.c_str()));
            pPreset->AddEffect(pParticle);
            pParticle->SetEnable(false);
        }
        //Texture
        else if (1 == iEffectType) {

            CEffectTexture::TEXEFFECT_DESC desc = {};
            string strTextureKey = ElementKey["TextureKey"].asString();


            desc.vCenter= { ElementKey["Center"]["x"].asFloat(),ElementKey["Center"]["y"].asFloat() ,ElementKey["Center"]["z"].asFloat() };
            desc.vColor = { ElementKey["Color"]["x"].asFloat(),ElementKey["Color"]["y"].asFloat() ,ElementKey["Color"]["z"].asFloat(), 1.f };
            desc.vRotation = { ElementKey["Rotation"]["x"].asFloat(),ElementKey["Rotation"]["y"].asFloat() ,ElementKey["Rotation"]["z"].asFloat() };
            desc.vDuration.x = 5.f;
            desc.vDuration.y = 15.f;
            desc.vScale = { ElementKey["Scale"]["x"].asFloat(),ElementKey["Scale"]["y"].asFloat() };

            _tchar szFullPath[MAX_PATH] = TEXT("");
            MultiByteToWideChar(CP_ACP, 0, strTextureKey.c_str(), (_int)strlen(strTextureKey.c_str()), szFullPath, MAX_PATH);


            shared_ptr<CEffectTexture> pTexEffect = CEffectTexture::Create(szFullPath, &desc, const_cast<char*>(keyName.c_str()));
            pPreset->AddEffect(pTexEffect);
            pTexEffect->SetEnable(false);

        }
        //Mesh
        else if (2 == iEffectType) {

        }


        ++iter;

    }


    m_EffectPresets.emplace(_strKeyName, pPreset);
    in.close();

}
