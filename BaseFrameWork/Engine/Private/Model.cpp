#include "Model.h"
#include "ModelMaterial.h"
#include "Texture.h"
#include "Bone.h"
#include "Shader.h"
#include "Animation.h"

#include "Mesh.h"
#include "GameInstance.h"

#include <fstream>
#include <iostream>

CModel::CModel(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
    :CComponent(_pDevice, _pContext)
{
}

CModel::CModel(const CModel& _rhs)
    :CComponent(_rhs.m_pDevice, _rhs.m_pContext)
    , m_PivotMatrix(_rhs.m_PivotMatrix)
    , m_iNumMeshes(_rhs.m_iNumMeshes)
    ,m_Meshes(_rhs.m_Meshes)
    , m_iNumMaterials(_rhs.m_iNumMaterials)
    , m_Materials(_rhs.m_Materials)
    ,m_iNumAnimations(_rhs.m_iNumAnimations)
{
    for (auto& pBone : _rhs.m_Bones)
        m_Bones.push_back(pBone->Clone());

    for (auto& pAnim : _rhs.m_Animations)
        m_Animations.push_back(pAnim->Clone());
}

HRESULT CModel::Initialize(TYPE eModelType, const _char* pModelFilePath, const _tchar* _DatFilePath, _fmatrix PivotMatrix)
{
    m_eModelType = eModelType;
    const _tchar* szName = _DatFilePath;

    std::ifstream ifs;
    ifs.open(pModelFilePath, std::ios::binary);


     XMStoreFloat4x4(&m_PivotMatrix,XMMatrixIdentity());

    if (TYPE_ANIM == m_eModelType) {

        //본갯수
        size_t iBoneNumSize;
        ifs.read((char*)&iBoneNumSize, sizeof(size_t));

        //ReadyBone
        for (size_t i = 0; i < iBoneNumSize; ++i) {

            char* szName = new char[MAX_PATH];
            ifs.read(szName, MAX_PATH * sizeof(char));

            _int iParentBone;
            ifs.read((char*)&iParentBone, sizeof(_int));

            shared_ptr<CBone> pBone = CBone::Create(szName, iParentBone, ifs);

            m_Bones.push_back(pBone);
        }
    }

    if (FAILED(ReadyMeshes(ifs)))
        return E_FAIL;

    ifs.read((char*)&m_iNumMaterials, sizeof(size_t));

    if (FAILED(ReadyMaterials(ifs)))
        return E_FAIL;

    if (TYPE_ANIM == m_eModelType) {

        std::ifstream AnimIfs;
        AnimIfs.open(_DatFilePath, std::ios::binary);

        if (FAILED(ReadyAnimations(AnimIfs)))
            return E_FAIL;

        AnimIfs.close();
    }

    ifs.close();

    return S_OK;
}
  
HRESULT CModel::InitializeClone()
{

    return S_OK;
}

HRESULT CModel::Render(_uint _iMeshIndex)
{
   m_Meshes[_iMeshIndex]->BindBuffers();
   m_Meshes[_iMeshIndex]->Render();

    return S_OK;
}

void CModel::SetSpecificAnimSpeed(_uint _iAnimIdx, _float _fSpeed)
{
    if (m_Animations.size() < _iAnimIdx) {
        return;
    }

    m_Animations[_iAnimIdx]->SetAnimSpeed(_fSpeed);

}

_int CModel::GetBoneIndex(const _char* _pBoneName) const
{
    _int iBoneIndex = { -1 };

    auto iter = find_if(m_Bones.begin(), m_Bones.end(), [&](shared_ptr<CBone> pBone) {

        iBoneIndex++;
         
        if (!strcmp(_pBoneName, pBone->GetBoneName()))
            return true;
        return false;
    });


    if (iter == m_Bones.end())
        return -1;


    return iBoneIndex;
}


HRESULT CModel::BindMaterialShaderResource(shared_ptr<class CShader> _pShader, _uint _iMeshIndex, aiTextureType _eMaterialType, const _char* _pConstantName)
{
    _uint iMaterialIndex = m_Meshes[_iMeshIndex]->GetMaterialIndex();
    shared_ptr<CTexture> pTexture =  m_Materials[iMaterialIndex]->GetTextures()[_eMaterialType];
    return pTexture->BindShaderResource(_pShader, _pConstantName, 0);
}

HRESULT CModel::BindBoneMatrices(shared_ptr<class CShader> _pShader, const _char* _pConstName, _uint _iMeshIndex)
{
    _float4x4 BoneMatirces[MAX_BONE];

    m_Meshes[_iMeshIndex]->SetUpBoneMatrices(BoneMatirces, m_Bones);

    return _pShader->BindMatrices(_pConstName, BoneMatirces, MAX_BONE) ;
} 

_bool CModel::PlayAnimation(_float _fTimeDelta , _bool _isLoop, _float3* _vRootPos)
{
    _bool m_IsFinish = false;
    _bool IsSaveRootPos = true;

    //애니메이션 변경 이후 선형보간하는 시간
    if (m_IsLinearState) {

        m_fLinearTime += _fTimeDelta;
        if (m_fLinearTime >= m_fLinearTotalTime) {
            m_Animations[m_iCurrentAnimation]->ChangeAnimation(m_NextAnim, m_Bones, 1.f);

            m_IsLinearState = false;
            m_Animations[m_iCurrentAnimation]->AnimStateReset();
            m_CurrentAnim = m_NextAnim;
            m_iCurrentAnimation = m_iNextAnimation;

            m_NextAnim = nullptr;
            m_iNextAnimation = 0;
            m_fLinearTime = 0.f;

        }
        else {

            m_Animations[m_iCurrentAnimation]->ChangeAnimation(m_NextAnim, m_Bones, m_fLinearTime / m_fLinearTotalTime);
        }
    }
    else {
        m_IsFinish = m_Animations[m_iCurrentAnimation]->InvalidateTransformationMatrix(_fTimeDelta, m_Bones, _isLoop);
        IsSaveRootPos = false;
    }
    //애니메이션 변경 없이 한 애니메이션을 계속 돌리는 상황

    for (auto& pBone : m_Bones)
        pBone->InvalidateCombinedTransformationMatrix(m_Bones, _vRootPos, IsSaveRootPos);


    return m_IsFinish;
}

_bool CModel::ChangeAnimation(_uint _iAnimNum, _float _LinearTime)
{
    if (_iAnimNum == m_iCurrentAnimation || m_IsLinearState)
        return false;

    m_IsLinearState = true;
    m_NextAnim = m_Animations[_iAnimNum];
    m_iNextAnimation = _iAnimNum;

    m_fLinearTotalTime = _LinearTime;
  
    //m_Animations[m_iCurrentAnimation]->AnimStateReset();
    //m_iCurrentAnimation = _iAnimNum;

    //여기에 애니메이션 간의 러프 넣기 

    //(구) animation 의 currentkeyFrame + m_KeyFrames 벡터 주소
    // ->위의 정보값을 현 animationd 객체에게 넘긴 후 그 animation이 갖고 있는 채널들에게 싹 다 돌려서 계산한다? 
    //잠만! 걍 애니메이션 객체를 던지면 될듯?

    return true;
}

HRESULT CModel::ReadyMeshes(ifstream& _ifs)
{
    _ifs.read((char*)&m_iNumMeshes, sizeof(size_t));

    for (size_t i = 0; i < m_iNumMeshes; i++) {

        shared_ptr<CMesh> pMesh = CMesh::Create(m_eModelType, m_pDevice, m_pContext, _ifs, XMLoadFloat4x4(&m_PivotMatrix));

        if (!pMesh)
            return E_FAIL;

        m_Meshes.push_back(pMesh);
    }

    return S_OK;
}

HRESULT CModel::ReadyMaterials(ifstream& _ifs)
{
    _ulong dwByte = 0;

    for (size_t i = 0; i < m_iNumMaterials; i++) {

        map<_uint, shared_ptr<class CTexture>> Textures;
        
        size_t iTotTextureIdx = 0;
        _ifs.read((char*)&iTotTextureIdx, sizeof(size_t));

        for (size_t j = 0; j < iTotTextureIdx; j++) {

            char* szTexName = new char[MAX_PATH];
            _uint iTextureType;

            _ifs.read((char*)szTexName, sizeof(char) * MAX_PATH);
            _ifs.read((char*)&iTextureType, sizeof(_uint));

            _tchar szFullPath[MAX_PATH] = TEXT("");
            MultiByteToWideChar(CP_ACP, 0, szTexName, (_int)strlen(szTexName), szFullPath, MAX_PATH);

            Textures.emplace(iTextureType, CGameInstance::GetInstance()->GetTexture(szFullPath));
        }
        
        m_Materials.push_back(CMaterial::Create(Textures));
        
    }

    return S_OK;
}



HRESULT CModel::ReadyAnimations(ifstream& _ifs)
{
    _ifs.read((char*)&m_iNumAnimations, sizeof(size_t));

    shared_ptr<CModel> pModel = dynamic_pointer_cast<CModel>(shared_from_this());

    for (size_t i = 0; i < m_iNumAnimations; i++) {

        shared_ptr<CAnimation> pAnimation = CAnimation::Create(_ifs, pModel);

        if (!pAnimation)
            return E_FAIL;

        m_Animations.push_back(pAnimation);
    }

    return S_OK;
}

shared_ptr<CModel> CModel::Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, TYPE eModelType, const _char* pModelFilePath, const _tchar* _DatFilePath, _fmatrix PivotMatrix)
{
    shared_ptr<CModel> pInstance = make_shared<CModel>(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize(eModelType, pModelFilePath, _DatFilePath, PivotMatrix)))
        MSG_BOX("Failed to Create : CModel");

    return pInstance;
}

shared_ptr<CModel> CModel::Clone(shared_ptr<CModel> _rhs)
{   
    shared_ptr<CModel> pInstance = make_shared<CModel>(*(_rhs.get()));

    if (FAILED(pInstance->InitializeClone())) {
        MSG_BOX("Failed to Clone : CModel");
    }

    return pInstance;
}
