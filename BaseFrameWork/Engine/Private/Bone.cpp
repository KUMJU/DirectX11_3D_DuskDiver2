#include "Bone.h"

CBone::CBone()
{
}

HRESULT CBone::Initialize(const aiNode* _pAIBone, _int _iParentBoneIndex)
{
    strcpy_s(m_szName, _pAIBone->mName.data);

    //AIscene에서 바로 읽어오면 col-major Matrix로 읽어지기 때문에 전치해주는 과정이 필요하다
    memcpy(&m_TransformationMatrix, &_pAIBone->mTransformation, sizeof(_float4x4));
    XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));

    //초기에는 값을 세팅할 수 없기에 identity를 넣어둔다
    XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

    m_iParentBoneIndex = _iParentBoneIndex;

    return S_OK;
}

void CBone::InvalidateCombinedTransformationMatrix(const vector<shared_ptr<CBone>>& _Bones)
{
    if (-1 == m_iParentBoneIndex)
        m_CombinedTransformationMatrix = m_TransformationMatrix;
    else
        XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&_Bones[m_iParentBoneIndex]->m_CombinedTransformationMatrix));

}

shared_ptr<CBone> CBone::Create(const aiNode* _pAIBone, _int _iParentBoneIndex)
{
    shared_ptr<CBone> pInstance = make_shared<CBone>();

    if (FAILED(pInstance->Initialize(_pAIBone, _iParentBoneIndex)))
        MSG_BOX("Failed to Create : CBone");

    return pInstance;
}
