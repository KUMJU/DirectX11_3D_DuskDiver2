#include "Mesh.h"

CMesh::CMesh(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
	:CVIBuffer(_pDevice, _pContext)
{
}

HRESULT CMesh::Initialize(CModel::TYPE _eType, const aiMesh* pAIMesh, _fmatrix PivotMatrix)
{
	m_iMaterialIndex = pAIMesh->mMaterialIndex;

	m_iNumVertexBuffers = 1;
	m_iVertexStride = sizeof(VTXMESH);
	m_iNumVertices = pAIMesh->mNumVertices;

	m_iIndexStride = 4;
	m_eIndexFormat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	//mNumFaces : 면의 갯수 
	m_iNumIndices = pAIMesh->mNumFaces * 3;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	/////////////Vertex Buffer///////////////
	HRESULT hr = _eType == CModel::TYPE_NONANIM ? ReadyVertexBufferNonAnim(pAIMesh, PivotMatrix) : ReadyVertexBufferAnim(pAIMesh);

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

HRESULT CMesh::ReadyVertexBufferAnim(const aiMesh* _pAIMesh)
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

	//메시의 정점들의 상태에 영향을 주는 뼈들을 가져옴

	m_iNumBones = _pAIMesh->mNumBones;


	for (size_t i = 0; i < m_iNumBones; i++) {

		aiBone* pAIBone = _pAIMesh->mBones[i];

		//최대 4개까지 영향을 받을 수 있음 
		//Weight는 0이 될 수 없기 때문에 weights가 0일때 값이 채워지지 않음을 알 수 있다
		//한번만 채워주고 빠져나가야하기 때문에 else if문으로 만들어준다
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

	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::CreateBuffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

shared_ptr<CMesh> CMesh::Create(CModel::TYPE _eType, wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, const aiMesh* pAIMesh, _fmatrix PivotMatrix)
{
	shared_ptr<CMesh> pInstance = make_shared<CMesh>(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_eType, pAIMesh, PivotMatrix)))
		MSG_BOX("Failed to Create : CMesh");

	return pInstance;
}
