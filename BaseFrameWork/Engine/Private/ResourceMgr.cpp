#include "ResourceMgr.h"

#include <io.h>
#include "Shader.h"
#include "Texture.h"
#include "Model.h"


CResourceMgr::CResourceMgr(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
    :m_pDevice(_pDevice) , m_pContext(_pContext)
{
}

void CResourceMgr::BaseResourceLoad()
{
    m_iCurrentLevel = 0;
    const wstring& strBasePath = TEXT("../../Client/Bin/Resources/Base/");

    LoadTexture(strBasePath);
    LoadMesh(strBasePath);
}

void CResourceMgr::LogoResourceLoad()
{
    m_iCurrentLevel = 2;
    const wstring& strBasePath = TEXT("../../Client/Bin/Resources/Logo/");

    LoadShader();
    LoadTexture(strBasePath);
    LoadMesh(strBasePath);
 
}

void CResourceMgr::ArcadeMapResourceLoad()
{
    const wstring& strBasePath = TEXT("../../Client/Bin/Resources/ArcadeMap/");

    LoadTexture(strBasePath);
    LoadMesh(strBasePath);
 
}

void CResourceMgr::TownMapResourceLoad()
{

}

void CResourceMgr::LoadLevelResource(_uint _iLevelIndex)
{
    m_iCurrentLevel = _iLevelIndex;

    switch (_iLevelIndex)
    {
    case 0:
        BaseResourceLoad();
        break;
    case 1:
        break;
    case 2:
        LogoResourceLoad();
        break;
    case 3:
        ArcadeMapResourceLoad();
        break;
    default:
        break;
    }

}

shared_ptr<CShader> CResourceMgr::GetShader(const wstring& _strShaderKey)
{
    auto iter = m_Shaders.find(_strShaderKey);
   
    if (m_Shaders.end() != iter)
        return iter->second;

    return nullptr;
}

shared_ptr<class CTexture> CResourceMgr::GetTexture(const wstring& _strTextureKey)
{
    auto iter = m_Textures.find(_strTextureKey);

    if (m_Textures.end() != iter)
        return iter->second.pRes;

    return nullptr;
}

shared_ptr<class CModel> CResourceMgr::GetModel(const wstring& _strModelKey)
{
    auto iter = m_Models.find(_strModelKey);

    if (iter != m_Models.end()) {

       if (CModel::TYPE::TYPE_ANIM == (*iter).second.pRes->GetModelType()) {
            return CModel::Clone(((*iter).second.pRes));
       }

        return (*iter).second.pRes;
    }

    return nullptr;
}

shared_ptr<class CComponent> CResourceMgr::GetBuffer(const wstring& _strModelKey)
{
    auto iter = m_Buffers.find(_strModelKey);

    if (m_Buffers.end() != iter) {
        return (*iter).second;
    }

    return nullptr;
}

void CResourceMgr::AddBuffer(const wstring& _strKeyName, shared_ptr<class CComponent> _pBuf)
{
    m_Buffers.emplace(_strKeyName, _pBuf);
}

HRESULT CResourceMgr::LoadShader()
{
    const wstring& strBasePath = TEXT("../../Client/Bin/ShaderFiles/");
    const wstring& strFlag = TEXT("*.*");
   
    ReadShaderFile(strBasePath, strFlag);

    return S_OK;
}

void CResourceMgr::ReadShaderFile(const wstring& _strBaseFilepath, const wstring& _strExt)
{
    _wfinddata_t fd;

     intptr_t lHandle;

     const wstring& strFullPath = _strBaseFilepath + _strExt;

    //파일을 더 이상 찾지 못했을 경우
    if (-1 == (lHandle = _wfindfirst(strFullPath.c_str(), &fd)))
        return;

    while (0 == _wfindnext(lHandle, &fd))
    {   
        const wstring& strPath = _strBaseFilepath + fd.name;

        if (!IsFileOrDir(fd) && fd.name[0] != '.') {
            ReadShaderFile(strPath, _strExt);
        }
        else if (IsFileOrDir(fd) && fd.size != 0 && fd.name[0] != '.') {
            shared_ptr<CShader> pShader = nullptr;
            wstring strName = fd.name;
            //file name Tag 식별 후 각자 맞는 elements를 채워줌
            if (wstring::npos != strName.find(TEXT("Pos")))
            {
                pShader = CShader::Create(m_pDevice, m_pContext, strPath, VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
            }
            else if (wstring::npos != strName.find(TEXT("Nor"))) {
                pShader =  CShader::Create(m_pDevice, m_pContext, strPath, VTXNORTEX::Elements, VTXNORTEX::iNumElements);
            }
            else if (wstring::npos != strName.find(TEXT("Mesh"))) {
                pShader = CShader::Create(m_pDevice, m_pContext, strPath, VTXMESH::Elements, VTXMESH::iNumElements);
            }
            else if (wstring::npos != strName.find(TEXT("Anim"))) {
                pShader = CShader::Create(m_pDevice, m_pContext, strPath, VTXANIMMESH::Elements, VTXANIMMESH::iNumElements);
            }

            if (pShader) {
                m_Shaders.emplace(EraseExt(strName), pShader);
            }
        }
    }
    _findclose(lHandle);
} 

HRESULT CResourceMgr::LoadTexture(const wstring& _strBaseFilepath)
{
    const wstring& strFlag = TEXT("*.*");

    ReadTextureFile(_strBaseFilepath, strFlag);
    

    return S_OK;
}

void CResourceMgr::ReadTextureFile(const wstring& _strBaseFilepath, const wstring& _strExt)
{
    _wfinddata_t fd;
    intptr_t lHandle;
    const wstring& strFullPath = _strBaseFilepath + _strExt;

    int iFileCount = 0;

    if (-1 == (lHandle = _wfindfirst(strFullPath.c_str(), &fd)))
        return;

    while (0 == _wfindnext(lHandle, &fd))
    {
        const wstring& strPath = _strBaseFilepath + fd.name;
        const wstring& strName = fd.name;

        if (!IsFileOrDir(fd) && fd.name[0] != '.') {
            const wstring& strNewPath = strPath + TEXT("/");
            ReadTextureFile(strNewPath, _strExt);
        }
        else if (IsFileOrDir(fd) && fd.size != 0 && fd.name[0] != '.') {
            wstring strName = fd.name;
            wstring strExt = FindExt(strName);
            if (TEXT(".png") == FindExt(strName) ||  TEXT(".dds") == FindExt(strName) || TEXT(".bmp") == FindExt(strName)) {
                ResourceDesc<CTexture> tmp = { CTexture::Create(m_pDevice, m_pContext, strPath, 1) , m_iCurrentLevel };
                m_Textures.emplace(EraseExt(strName), tmp);
            }
        }
    }

    _findclose(lHandle);
}

HRESULT CResourceMgr::LoadMesh(const wstring& _strBaseFilepath)
{
    const wstring& strExt = TEXT("*.*");
    const wstring& strNonAnimPath = _strBaseFilepath + TEXT("Models/NonAnim/");
    const wstring& strAnimPath = _strBaseFilepath + TEXT("Models/Anim/");

    ReadAnimMeshFile(strAnimPath, strExt);
    ReadNonAnimMeshFile(strNonAnimPath, strExt);

    return S_OK;
}

void CResourceMgr::ReadAnimMeshFile(const wstring& _strBaseFilepath, const wstring& _strExt)
{
    _wfinddata_t fd;
    intptr_t lHandle;
    const wstring& strFullPath = _strBaseFilepath + _strExt;

    int iFileCount = 0;

    if (-1 == (lHandle = _wfindfirst(strFullPath.c_str(), &fd)))
        return;

    while (0 == _wfindnext(lHandle, &fd))
    {
        const wstring& strPath = _strBaseFilepath + fd.name;
        const wstring& strName = fd.name;

        if (!IsFileOrDir(fd) && fd.name[0] != '.') {
            const wstring& strNewPath = strPath + TEXT("/");
            ReadAnimMeshFile(strNewPath, _strExt);
        }
        else if (IsFileOrDir(fd) && fd.size != 0 && fd.name[0] != '.') {
     
            char strMultiByte[MAX_PATH] = { 0 };
            WideCharToMultiByte(CP_ACP, 0, strPath.c_str(), lstrlenW(strPath.c_str()), strMultiByte, MAX_PATH, NULL, NULL);

            if (TEXT(".dat") == FindExt(strName)) {

                wstring AnimFilePath =  TEXT("../../Client/Bin/DataFiles/AnimData/Anim_");
                AnimFilePath = AnimFilePath + strName;

                ResourceDesc<CModel> tmp = { CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM , strMultiByte ,AnimFilePath.c_str()) ,m_iCurrentLevel };
                m_Models.emplace(EraseExt(strName), tmp);
            }
        }
    }

    _findclose(lHandle);

}

void CResourceMgr::ReadNonAnimMeshFile(const wstring& _strBaseFilepath, const wstring& _strExt)
{
    _wfinddata_t fd;
    intptr_t lHandle;
    const wstring& strFullPath = _strBaseFilepath + _strExt;

    int iFileCount = 0;

    if (-1 == (lHandle = _wfindfirst(strFullPath.c_str(), &fd)))
        return;

    while (0 == _wfindnext(lHandle, &fd))
    {
        const wstring& strPath = _strBaseFilepath + fd.name;
        const wstring& strName = fd.name;

        if (!IsFileOrDir(fd) && fd.name[0] != '.') {
            const wstring& strNewPath = strPath + TEXT("/");
            ReadNonAnimMeshFile(strNewPath, _strExt);
        }
        else if (IsFileOrDir(fd) && fd.size != 0 && fd.name[0] != '.') {

            char strMultiByte[MAX_PATH] = { 0 };
            WideCharToMultiByte(CP_ACP, 0, strPath.c_str(),lstrlenW(strPath.c_str()), strMultiByte, MAX_PATH , NULL, NULL);
           
            wstring AnimFilePath = TEXT("");

            if (TEXT(".dat") == FindExt(strName)) {
                ResourceDesc<CModel> tmp = { CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM , strMultiByte, AnimFilePath.c_str(), XMMatrixIdentity()) , m_iCurrentLevel};
                m_Models.emplace(EraseExt(strName), tmp);
            }
        }
    }

    _findclose(lHandle);

}

void CResourceMgr::LoaderForTool()
{

    LoadMesh(TEXT("../../Client/Bin/Resources/Base/"));
    LoadMesh(TEXT("../../Client/Bin/Resources/ArcadeMap/"));

}

_bool CResourceMgr::IsFileOrDir(_wfinddata_t _fd)
{
    if (_fd.attrib & _A_SUBDIR)
        return false; //디렉토리
    else
        return true; //파일

}

wstring CResourceMgr::EraseExt(const wstring& _strFileName)
{
    size_t iCount = _strFileName.find_last_of(TEXT("."));
    return _strFileName.substr(0, iCount);
}

wstring CResourceMgr::FindExt(const wstring& _strFileName)
{
    size_t iCount = _strFileName.find_last_of(TEXT("."));
    return _strFileName.substr(iCount);
}

void CResourceMgr::DeleteResource()
{
    //특정 레벨에서만 사용하는 텍스쳐 제거
    for (auto iter = m_Textures.begin(); iter != m_Textures.end(); ) {

        if (0 != (*iter).second.iLevel  && m_iCurrentLevel != (*iter).second.iLevel) {
            iter = m_Textures.erase(iter);
        }
        else {
            ++iter;
        }
    }


    for (auto iter = m_Models.begin(); iter != m_Models.end(); ) {

        if (0 != (*iter).second.iLevel && m_iCurrentLevel != (*iter).second.iLevel) {
            iter = m_Models.erase(iter);
        }
        else {
            ++iter;
        }
    }
}

shared_ptr<CResourceMgr> CResourceMgr::Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
{
    return make_shared<CResourceMgr>(_pDevice, _pContext);
}
