#include "Bone.h"

#include <iostream>
#include <fstream>

CBone::CBone()
{
}

HRESULT CBone::Initialize(char* _pName, _int _iParentBoneIndex, ifstream& _ifs)
{
    strcpy_s(m_szName, _pName);

	_float a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16 = 0.f;


	_ifs.read((char*)&a1, sizeof(_float));
	_ifs.read((char*)&a2, sizeof(_float));
	_ifs.read((char*)&a3, sizeof(_float));
	_ifs.read((char*)&a4, sizeof(_float));
	_ifs.read((char*)&a5, sizeof(_float));
	_ifs.read((char*)&a6, sizeof(_float));
	_ifs.read((char*)&a7, sizeof(_float));
	_ifs.read((char*)&a8, sizeof(_float));
	_ifs.read((char*)&a9, sizeof(_float));
	_ifs.read((char*)&a10, sizeof(_float));
	_ifs.read((char*)&a11, sizeof(_float));
	_ifs.read((char*)&a12, sizeof(_float));
	_ifs.read((char*)&a13, sizeof(_float));
	_ifs.read((char*)&a14, sizeof(_float));
	_ifs.read((char*)&a15, sizeof(_float));
	_ifs.read((char*)&a16, sizeof(_float));



	m_TransformationMatrix._11 = a1;
	m_TransformationMatrix._12 = a2;
	m_TransformationMatrix._13 = a3;
	m_TransformationMatrix._14 = a4;
	m_TransformationMatrix._21 = a5;
	m_TransformationMatrix._22 = a6;
	m_TransformationMatrix._23 = a7;
	m_TransformationMatrix._24 = a8;
	m_TransformationMatrix._31 = a9;
	m_TransformationMatrix._32 = a10;
	m_TransformationMatrix._33 = a11;
	m_TransformationMatrix._34 = a12;
	m_TransformationMatrix._41 = a13;
	m_TransformationMatrix._42 = a14;
	m_TransformationMatrix._43 = a15;
	m_TransformationMatrix._44 = a16;



   // _ifs.read((char*)&m_TransformationMatrix, sizeof(_float4x4));
   
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


shared_ptr<CBone> CBone::Create(char* _pName, _int _iParentBoneIndex, ifstream& _ifs)
{
    shared_ptr<CBone> pInstance = make_shared<CBone>();

    if (FAILED(pInstance->Initialize(_pName, _iParentBoneIndex, _ifs)))
        MSG_BOX("Failed to Create : CBone");

    return pInstance;
}
