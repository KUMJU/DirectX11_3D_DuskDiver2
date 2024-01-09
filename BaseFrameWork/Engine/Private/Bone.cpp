#include "Bone.h"

CBone::CBone()
{
}

HRESULT CBone::Initialize(const aiNode* _pAIBone, _int _iParentBoneIndex)
{
    strcpy_s(m_szName, _pAIBone->mName.data);

    //AIscene���� �ٷ� �о���� col-major Matrix�� �о����� ������ ��ġ���ִ� ������ �ʿ��ϴ�
    memcpy(&m_TransformationMatrix, &_pAIBone->mTransformation, sizeof(_float4x4));
    XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));

    //�ʱ⿡�� ���� ������ �� ���⿡ identity�� �־�д�
    XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

    m_iParentBoneIndex = _iParentBoneIndex;

    return S_OK;
}

shared_ptr<CBone> CBone::Create(const aiNode* _pAIBone, _int _iParentBoneIndex)
{
    shared_ptr<CBone> pInstance = make_shared<CBone>();

    if (FAILED(pInstance->Initialize(_pAIBone, _iParentBoneIndex)))
        MSG_BOX("Failed to Create : CBone");

    return pInstance;
}
