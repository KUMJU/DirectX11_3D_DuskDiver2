#include "Mesh.h"
#include "Bone.h"

CMesh::CMesh(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
	:CVIBuffer(_pDevice, _pContext)
{
}

HRESULT CMesh::Initialize(CModel::TYPE _eType, const aiMesh* pAIMesh, shared_ptr<CModel> _pModel, _fmatrix PivotMatrix)
{
	strcpy_s(m_szName, pAIMesh->mName.data);

	m_iMaterialIndex = pAIMesh->mMaterialIndex;

	m_iNumVertexBuffers = 1;
	m_iNumVertices = pAIMesh->mNumVertices;

	m_iIndexStride = 4;
	m_eIndexFormat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	//mNumFaces : ���� ���� 
	m_iNumIndices = pAIMesh->mNumFaces * 3;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	/////////////Vertex Buffer///////////////
	HRESULT hr = _eType == CModel::TYPE_NONANIM ? ReadyVertexBufferNonAnim(pAIMesh, PivotMatrix) : ReadyVertexBufferAnim(pAIMesh, _pModel);

	if (FAILED(hr))
		return E_FAIL;

	/////////////Index Buffer///////////////
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	_uint* pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);

	_uint iNumIndices = { 0 };

	for (size_t i = 0; i < pAIMesh->mNumFaces; i++) {

		pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[0];
		pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[1];
		pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[2];

	}

	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::CreateBuffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
	return S_OK;
}

void CMesh::SetUpBoneMatrices(_float4x4* _pBoneMatirces, vector<shared_ptr<class CBone>>& _Bones)
{
	_float4x4 CombinedMat;

	for (size_t i = 0; i < m_iNumBones; i++) {
		CombinedMat = _Bones[m_BoneIndices[i]]->GetCombiendTransformationMatrix();
		XMStoreFloat4x4(&_pBoneMatirces[i], XMLoadFloat4x4(&m_OffsetMatrices[i])* XMLoadFloat4x4(&CombinedMat));
	}
} 

HRESULT CMesh::ReadyVertexBufferNonAnim(const aiMesh* _pAIMesh, _fmatrix PivotMatrix)
{
	m_iVertexStride = sizeof(VTXMESH);

	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	VTXMESH* pVertices = new VTXMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMESH) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; i++) {

		memcpy(&pVertices[i].vPosition, &_pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &_pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexcoord, &_pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &_pAIMesh->mTangents[i], sizeof(_float3));

	}

	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::CreateBuffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);


	return S_OK;
}

HRESULT CMesh::ReadyVertexBufferAnim(const aiMesh* _pAIMesh, shared_ptr<CModel> _pModel)
{
	m_iVertexStride = sizeof(VTXANIMMESH);

	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	VTXANIMMESH* pVertices = new VTXANIMMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMESH) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; i++) {

		memcpy(&pVertices[i].vPosition, &_pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &_pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexcoord, &_pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &_pAIMesh->mTangents[i], sizeof(_float3));

	}

	//�޽��� �������� ���¿� ������ �ִ� ������ ������

	m_iNumBones = _pAIMesh->mNumBones;


	for (size_t i = 0; i < m_iNumBones; i++) {

		aiBone* pAIBone = _pAIMesh->mBones[i];
		_float4x4 OffsetMatrix = {};

		memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));

		m_OffsetMatrices.push_back(OffsetMatrix);
		m_BoneIndices.push_back(_pModel->GetBoneIndex(pAIBone->mName.data));

		//�ִ� 4������ ������ ���� �� ���� 
		//Weight�� 0�� �� �� ���� ������ weights�� 0�϶� ���� ä������ ������ �� �� �ִ�
		//�ѹ��� ä���ְ� �����������ϱ� ������ else if������ ������ش�
		for (size_t j = 0; j < pAIBone->mNumWeights; j++) {
			if (0.f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.x) {
				
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.x = (_uint)i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.x = pAIBone->mWeights[j].mWeight;

			}else if (0.f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.y) {

				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.y = (_uint)i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.y = pAIBone->mWeights[j].mWeight;

			}else if (0.f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.z) {

				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.z = (_uint)i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.z = pAIBone->mWeights[j].mWeight;

			}else if (0.f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.w) {

				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.w = (_uint)i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.w = pAIBone->mWeights[j].mWeight;

			}
		}
	}

	//���� ���� sword, shadow ���� �޽���
	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;
		m_BoneIndices.push_back(_pModel->GetBoneIndex(m_szName));

		_float4x4 OffsetMatrix;
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());

		m_OffsetMatrices.push_back(OffsetMatrix);
	}

	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::CreateBuffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

shared_ptr<CMesh> CMesh::Create(CModel::TYPE _eType, wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, const aiMesh* pAIMesh, shared_ptr<CModel> _pModel, _fmatrix PivotMatrix)
{
	shared_ptr<CMesh> pInstance = make_shared<CMesh>(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_eType, pAIMesh, _pModel, PivotMatrix)))
		MSG_BOX("Failed to Create : CMesh");

	return pInstance;
}
