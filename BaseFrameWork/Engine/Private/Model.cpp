#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "Bone.h"

#include "GameInstance.h"

CModel::CModel(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
    :CComponent(_pDevice, _pContext)
{
}

CModel::CModel(const CModel& _rhs)
    :CComponent(_rhs.m_pDevice, _rhs.m_pContext)
    , m_PivotMatrix(_rhs.m_PivotMatrix)
    , m_iNumMeshes(_rhs.m_iNumMeshes)
    , m_Meshes(_rhs.m_Meshes)
    , m_iNumMaterials(_rhs.m_iNumMaterials)
    , m_Materials(_rhs.m_Materials)
    , m_pAIScene(_rhs.m_pAIScene)
{
}

HRESULT CModel::Initialize(TYPE eModelType, const _char* pModelFilePath, _fmatrix PivotMatrix)
{
    XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);

    _uint iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
    m_eModelType = eModelType;

    if (TYPE_NONANIM == eModelType)
        iFlag |= aiProcess_PreTransformVertices;

    m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);

    if (!m_pAIScene)
        return E_FAIL;

    if (FAILED(ReadyMeshes()))
        return E_FAIL;

    if (FAILED(ReadyMaterials(pModelFilePath)))
        return E_FAIL;

    return S_OK;
}

HRESULT CModel::InitializeClone()
{
    if (FAILED(ReadyBones(m_pAIScene->mRootNode, -1)))
        return E_FAIL;

    return S_OK;
}

HRESULT CModel::Render(_uint _iMeshIndex)
{

    m_Meshes[_iMeshIndex]->BindBuffers();
    m_Meshes[_iMeshIndex]->Render();

    return S_OK;
}

HRESULT CModel::BindMaterialShaderResource(shared_ptr<class CShader> _pShader, _uint _iMeshIndex, aiTextureType _eMaterialType, const _char* _pConstantName)
{
    _uint iMaterialIndex = m_Meshes[_iMeshIndex]->GetMaterialIndex();
    shared_ptr<CTexture> pTexture =  m_Materials[iMaterialIndex]->GetTextures()[_eMaterialType];
    return pTexture->BindShaderResource(_pShader, _pConstantName, 0);
}

HRESULT CModel::ReadyMeshes()
{
    m_iNumMeshes = m_pAIScene->mNumMeshes;

    for (size_t i = 0; i < m_iNumMeshes; i++) {

        shared_ptr<CMesh> pMesh = CMesh::Create(m_eModelType, m_pDevice, m_pContext, m_pAIScene->mMeshes[i], XMLoadFloat4x4(&m_PivotMatrix));

        if (!pMesh)
            return E_FAIL;

        m_Meshes.push_back(pMesh);
    }

    return S_OK;
}

HRESULT CModel::ReadyMaterials(const _char* _pModelFilePath)
{
    _char szDrive[MAX_PATH] = "";
    _char szDirectory[MAX_PATH] = "";

    _splitpath_s(_pModelFilePath, szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);

    m_iNumMaterials = m_pAIScene->mNumMaterials;

    for (size_t i = 0; i < m_iNumMaterials; i++) {

        map<aiTextureType, shared_ptr<class CTexture>> Textures;

        aiMaterial* pAIMateral = m_pAIScene->mMaterials[i];

        for (size_t j = 0; j < AI_TEXTURE_TYPE_MAX; j++) {

            aiString strTextureFilePath;

            if (FAILED(pAIMateral->GetTexture(aiTextureType(j), 0, &strTextureFilePath)))
                continue;

            _char szFileName[MAX_PATH] = "";
            _char szExt[MAX_PATH] = "";

            _splitpath_s(strTextureFilePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

            _char szTmp[MAX_PATH] = "";
            strcpy_s(szTmp, szDrive);
            strcat_s(szTmp, szDirectory);
            strcat_s(szTmp, szFileName);
            strcat_s(szTmp, szExt);

            _tchar szFullPath[MAX_PATH] = TEXT("");
            MultiByteToWideChar(CP_ACP, 0, szFileName, (_int)strlen(szTmp), szFullPath, MAX_PATH);

            Textures.emplace(aiTextureType(j), CGameInstance::GetInstance()->GetTexture(szFullPath));
        }
        
        m_Materials.push_back(CMaterial::Create(Textures));
        
    }

    return S_OK;
}

HRESULT CModel::ReadyBones(aiNode* _pNode, _int _iParentBoneIndex)
{
    shared_ptr<CBone> pBone= CBone::Create(_pNode, _iParentBoneIndex++);
    
    if (!pBone)
        return E_FAIL;

    m_Bones.push_back(pBone);

    //재귀함수로 가장 최하단 노드까지 순회한다
    for (size_t i = 0; i < _pNode->mNumChildren; i++) {
        ReadyBones(_pNode->mChildren[i], _iParentBoneIndex);
    }

    return S_OK;
}

shared_ptr<CModel> CModel::Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, TYPE eModelType, const _char* pModelFilePath, _fmatrix PivotMatrix)
{
    shared_ptr<CModel> pInstance = make_shared<CModel>(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize(eModelType, pModelFilePath, PivotMatrix)))
        MSG_BOX("Failed to Create : CModel");

    return pInstance;
}

shared_ptr<CModel> CModel::Clone(shared_ptr<CModel> _rhs)
{   
    shared_ptr<CModel> pInstance = _rhs;

    if (FAILED(pInstance->InitializeClone())) {
        MSG_BOX("Failed to Clone : CModel");
    }

    return pInstance;
}
