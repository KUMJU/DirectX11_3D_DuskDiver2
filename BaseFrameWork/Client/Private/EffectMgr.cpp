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


    return pPreset;

}

shared_ptr<class CEffectPreset> CEffectMgr::FindAndCloneEffect(const wstring& _strKeyName)
{
    auto iter = m_EffectPresets.find(_strKeyName);
    shared_ptr<CEffectPreset> pOriginPreset = nullptr;
    shared_ptr<CEffectPreset> pClonePreset = nullptr;

    if (m_EffectPresets.end() != iter) {
        pOriginPreset = iter->second;
        pClonePreset = CEffectPreset::Clone(pOriginPreset);
        pClonePreset->SetEnable(true);
        CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Effect"), pClonePreset);
    }

    return pClonePreset;
}

void CEffectMgr::SetHitMark(_vector _vPos)
{
    for (auto& pHitMark : m_HitMarks) {

        if (!pHitMark->IsEnabled()) {
            pHitMark->SetEffectPosition(XMVectorSetW(_vPos, 1.f));
            pHitMark->PlayEffect();
            break;
        }
    }

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
            desc.vDuration = { ElementKey["Duration"]["x"].asFloat(),ElementKey["Duration"]["y"].asFloat()};
            desc.vSpeed = { ElementKey["Speed"]["x"].asFloat(),ElementKey["Speed"]["y"].asFloat()};
            
            _tchar szFullPath[MAX_PATH] = TEXT("");
            MultiByteToWideChar(CP_ACP, 0, strTextureKey.c_str(), (_int)strlen(strTextureKey.c_str()), szFullPath, MAX_PATH);

            
            shared_ptr<CEffectParticle> pParticle =  CEffectParticle::Create(iInstanceNum, szFullPath, &desc, const_cast<char*>(keyName.c_str()), bLoop);
            pPreset->AddEffect(pParticle);
            pParticle->SetEnable(false);
        }
        //Texture
        else if (1 == iEffectType) {

            CEffectTexture::TEXEFFECT_DESC desc = {};
            string strTextureKey = ElementKey["TextureKey"].asString();

            desc.vCenter = { ElementKey["Center"]["x"].asFloat(),
            ElementKey["Center"]["y"].asFloat(),
            ElementKey["Center"]["z"].asFloat() };

            desc.vStartScale = { ElementKey["StartScale"]["x"].asFloat(),
            ElementKey["StartScale"]["y"].asFloat() };

            desc.vMiddleScale = { ElementKey["MiddleScale"]["x"].asFloat(),
            ElementKey["MiddleScale"]["y"].asFloat() };

            desc.vEndScale = { ElementKey["EndScale"]["x"].asFloat(),
            ElementKey["EndScale"]["y"].asFloat() };


            desc.vRotation = { ElementKey["Rotation"]["x"].asFloat(),
                                      ElementKey["Rotation"]["y"].asFloat(),
                                      ElementKey["Rotation"]["z"].asFloat() };


            desc.vColor = { ElementKey["Color"]["x"].asFloat(),
            ElementKey["Color"]["y"].asFloat(),
            ElementKey["Color"]["z"].asFloat(),
            ElementKey["Color"]["w"].asFloat() };



            desc.vDuration = { ElementKey["Duration"]["x"].asFloat(),
                                      ElementKey["Duration"]["y"].asFloat() };

            desc.fTurnSpeed = ElementKey["Speed"].asFloat();

            desc.vTurnAxis = { ElementKey["Axis"]["x"].asFloat(),
                                      ElementKey["Axis"]["y"].asFloat(),
                                      ElementKey["Axis"]["z"].asFloat(),
                                      0.f };

            desc.fScaleChangeTime = ElementKey["ScaleChageTime"].asFloat();

     
            _tchar szFullPath[MAX_PATH] = TEXT("");
            MultiByteToWideChar(CP_ACP, 0, strTextureKey.c_str(), (_int)strlen(strTextureKey.c_str()), szFullPath, MAX_PATH);

            shared_ptr<CEffectTexture> pTexEffect = CEffectTexture::Create(szFullPath, &desc, const_cast<char*>(keyName.c_str()));
            pPreset->AddEffect(pTexEffect);
            pTexEffect->SetEnable(false);

        }
        //Mesh
        else if (2 == iEffectType) {

            CEffectMesh::MESH_DESC desc = {};
            desc.vCenter = { ElementKey["Center"]["x"].asFloat(),
            ElementKey["Center"]["y"].asFloat(),
            ElementKey["Center"]["z"].asFloat() };

            desc.vStartScale = { ElementKey["StartScale"]["x"].asFloat(),
            ElementKey["StartScale"]["y"].asFloat(),
            ElementKey["StartScale"]["z"].asFloat() };

            desc.vMiddleScale = { ElementKey["MiddleScale"]["x"].asFloat(),
            ElementKey["MiddleScale"]["y"].asFloat(),
            ElementKey["MiddleScale"]["z"].asFloat() };

            desc.vEndScale = { ElementKey["EndScale"]["x"].asFloat(),
            ElementKey["EndScale"]["y"].asFloat(),
            ElementKey["EndScale"]["z"].asFloat() };

            desc.bUVLoop = ElementKey["Loop"]["UVLoop"].asBool();
            desc.bLoop = ElementKey["Loop"]["EffectLoop"].asBool();

            desc.vColor = { ElementKey["Color"]["x"].asFloat(),
            ElementKey["Color"]["y"].asFloat(),
            ElementKey["Color"]["z"].asFloat(),
            1.f };

            desc.vDuration = { ElementKey["Duration"]["x"].asFloat(),
            ElementKey["Duration"]["y"].asFloat() };

            desc.vRotation = { ElementKey["Rotation"]["x"].asFloat(),
            ElementKey["Rotation"]["y"].asFloat(),
            ElementKey["Rotation"]["z"].asFloat()};

            desc.fScaleChangeTime = ElementKey["ChangeTime"].asFloat();

            string strMaskPath = ElementKey["Mask"]["TexKey"].asString();
            string strNoisePath = ElementKey["Noise"]["TexKey"].asString();

            string strMeshName = ElementKey["MeshKey"].asString();

            _tchar szMeshName[MAX_PATH] = TEXT("");
            MultiByteToWideChar(CP_ACP, 0, strMeshName.c_str(), (_int)strlen(strMeshName.c_str()), szMeshName, MAX_PATH);

            desc.szMaskTexKey = new char[256];
            desc.szNoiseTexKey = new char[256];

            memcpy_s(desc.szMaskTexKey, sizeof(char) * 256, strMaskPath.c_str(), sizeof(char) * 256);
            memcpy_s(desc.szNoiseTexKey, sizeof(char) * 256, strNoisePath.c_str(), sizeof(char) * 256);

            desc.bMask = ElementKey["Mask"]["Using"].asBool();
            desc.bNoise = ElementKey["Noise"]["Using"].asBool();

            desc.vMaskDir = { ElementKey["Mask"]["x"].asFloat(), ElementKey["Mask"]["y"].asFloat() };
            desc.vNoiseDir = { ElementKey["Noise"]["x"].asFloat(), ElementKey["Noise"]["y"].asFloat() };
            desc.fMaskSpeed = ElementKey["Mask"]["Speed"].asFloat();
            desc.fNoiseSpeed = ElementKey["Noise"]["Speed"].asFloat();
            desc.vLerpColor = { ElementKey["LerpColor"]["x"].asFloat(),
            ElementKey["LerpColor"]["y"].asFloat(),
            ElementKey["LerpColor"]["z"].asFloat(),
            1.f };

            desc.bDistortion = false;
            desc.fDistortionScale = 1.f;

            /*Distortion*/

            if (keyName == "OutSideBall") {

                desc.bDistortion = true;
                desc.fDistortionScale = 2.4f;

            }

            if(keyName ==  "BlastWind1") {

                desc.bDistortion = true;
                desc.fDistortionScale = 5.f;
            }


            if (keyName == "BlastWind2") {

                desc.bDistortion = true;
                desc.fDistortionScale = 5.f;
            }

            if (keyName == "ScreamWind") {

                desc.bDistortion = true;
                desc.fDistortionScale = 1.1f;
            }


            if (keyName == "BackWind") {

                desc.bDistortion = true;
                desc.fDistortionScale = 2.f;
            }


            if (keyName == "Tornado") {

                desc.bDistortion = true;
                desc.fDistortionScale = 3.f;
            }

            if (keyName == "TransBallWind") {

                desc.bDistortion = true;
                desc.fDistortionScale = 1.5f;
            }

            if (keyName == "DistortionMesh") {

                desc.bDistortion = true;
                desc.fDistortionScale = 1.f;
                desc.bDistortionMesh = true;
            }


            shared_ptr<CEffectMesh> pMeshEffect = CEffectMesh::Create(szMeshName, &desc, const_cast<char*>(keyName.c_str()), bLoop);
            pPreset->AddEffect(pMeshEffect);
            pMeshEffect->SetEnable(false);
        }


        ++iter;

    }

    //따로 분류하는 이펙트(모두 공용으로 사용할 수 있는 이펙트)
    if (TEXT("HitMark") == _strKeyName) {

        for (size_t iNum = 0; iNum < 20; ++iNum) {
            shared_ptr<CEffectPreset> pEffectPresetClone = CEffectPreset::Clone(pPreset);
            m_HitMarks.push_back(pEffectPresetClone);
            pEffectPresetClone->SetEnable(false);
            pEffectPresetClone->SetBillboard(true);
            CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Effect"), pEffectPresetClone);
        }    
    }
   // else {
        m_EffectPresets.emplace(_strKeyName, pPreset);
   // }

    in.close();

}
