#include "Bone.h"

CBone::CBone()
{
}

HRESULT CBone::Initialize(char* _pName, _int _iParentBoneIndex, HANDLE _handle)
{
    _ulong dwByte = 0; 
    strcpy_s(m_szName, _pName);

    //AIscene에서 바로 읽어오면 col-major Matrix로 읽어지기 때문에 전치해주는 과정이 필요하다
   // memcpy(&m_TransformationMatrix, &_pAIBone->mTransformation, sizeof(_float4x4));

    ReadFile(_handle, &m_TransformationMatrix, sizeof(_float4x4), &dwByte, nullptr);

   /* if(-1 != m_iParentBoneIndex)
        XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));
    else
        XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));*/

    //초기에는 값을 세팅할 수 없기에 identity를 넣어둔다
    XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

    m_iParentBoneIndex = _iParentBoneIndex;

    return S_OK;
}

void CBone::InvalidateCombinedTransformationMatrix(const vector<shared_ptr<CBone>>& _Bones, _float3* _vRootPos, _bool _IsLinearState)
{
    if (-1 == m_iParentBoneIndex)
        m_CombinedTransformationMatrix = m_TransformationMatrix;
    else
        XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&_Bones[m_iParentBoneIndex]->m_CombinedTransformationMatrix));

    /*애니메이션 움직임 조절 */
    if (!strcmp(m_szName, "root"))
    {  
        if (!_IsLinearState) {
            _vRootPos->x = m_CombinedTransformationMatrix._41;
            _vRootPos->y = m_CombinedTransformationMatrix._42;
            _vRootPos->z = m_CombinedTransformationMatrix._43;
        }

        m_CombinedTransformationMatrix._41 = 0.f;
        m_CombinedTransformationMatrix._42 = 0.f;
        m_CombinedTransformationMatrix._43 = 0.f;
    }

}

void CBone::ParsingBone(HANDLE _handle)
{
    _ulong dwByte = 0;

    
    size_t NameSize = sizeof(m_szName);

    //다음에 읽을 이름 Char 배열의 크기
    WriteFile(_handle, &NameSize, sizeof(size_t), &dwByte, nullptr);
    WriteFile(_handle, m_szName, sizeof(m_szName), &dwByte, nullptr);

    WriteFile(_handle, &m_iParentBoneIndex, sizeof(_int), &dwByte, nullptr);

    for (_int i = 0; i < 16; ++i) {

        _int iRow = i / 4;
        _int iCol = i % 4;

        _float fValue = m_TransformationMatrix.m[iRow][iCol];
        WriteFile(_handle, &fValue, sizeof(_float), &dwByte, nullptr);
    }

}


shared_ptr<CBone> CBone::Create(char* _pName, _int _iParentBoneIndex, HANDLE _handle)
{
    shared_ptr<CBone> pInstance = make_shared<CBone>();

    if (FAILED(pInstance->Initialize(_pName, _iParentBoneIndex,_handle)))
        MSG_BOX("Failed to Create : CBone");

    return pInstance;
}
