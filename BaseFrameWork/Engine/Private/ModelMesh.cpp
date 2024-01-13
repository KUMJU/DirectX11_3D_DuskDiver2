#include "ModelMesh.h"
#include "Bone.h"

CModelMesh::CModelMesh(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
	:CVIBuffer(_pDevice, _pContext)
{
}

CModelMesh::~CModelMesh()
{

}

HRESULT CModelMesh::Initialize(CModel::TYPE _eType, HANDLE _handle, _fmatrix PivotMatrix)
{

	_ulong dwByte = 0;

	ReadFile(_handle, m_szName, sizeof(char) * MAX_PATH, &dwByte, nullptr);
	ReadFile(_handle, &m_iMaterialIndex, sizeof(_uint), &dwByte, nullptr);
	ReadFile(_handle, &m_iNumVertices, sizeof(_uint), &dwByte, nullptr);
	ReadFile(_handle, &m_iNumIndices, sizeof(_uint), &dwByte, nullptr);

	m_iNumVertexBuffers = 1;

	m_iIndexStride = 4;
	m_eIndexFormat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	//mNumFaces : 면의 갯수 
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	/////////////Vertex Buffer///////////////
	HRESULT hr = _eType == CModel::TYPE_NONANIM ? ReadyVertexBufferNonAnim(_handle, PivotMatrix) : ReadyVertexBufferAnim(_handle);

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

	_uint iFaceNum;
	ReadFile(_handle, &iFaceNum, sizeof(_uint), &dwByte, nullptr);

	_uint iIndex1, iIndex2, iIndex3;


	ReadFile(_handle, &iIndex1, sizeof(_uint), &dwByte, nullptr);
	ReadFile(_handle, &iIndex2, sizeof(_uint), &dwByte, nullptr);
	ReadFile(_handle, &iIndex3, sizeof(_uint), &dwByte, nullptr);

	for (size_t i = 0; i < iFaceNum; i++) {

		pIndices[iNumIndices++] = iIndex1;
		pIndices[iNumIndices++] = iIndex2;
		pIndices[iNumIndices++] = iIndex3;

	}

	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::CreateBuffer(&m_pIB)))
		return E_FAIL;

	delete[] pIndices;

	//Safe_Delete_Array(pIndices);
	return S_OK;
}

void CModelMesh::SetUpBoneMatrices(_float4x4* _pBoneMatirces, vector<shared_ptr<class CBone>>& _Bones)
{
	_float4x4 CombinedMat;

	for (size_t i = 0; i < m_iNumBones; i++) {
		CombinedMat = _Bones[m_BoneIndices[i]]->GetCombiendTransformationMatrix();
		XMStoreFloat4x4(&_pBoneMatirces[i], XMLoadFloat4x4(&m_OffsetMatrices[i]) * XMLoadFloat4x4(&CombinedMat));
	}
}

HRESULT CModelMesh::ReadyVertexBufferNonAnim(HANDLE _handle, _fmatrix PivotMatrix)
{
	_ulong dwByte = 0;
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

		ReadFile(_handle, &pVertices[i].vPosition, sizeof(_float3), &dwByte, nullptr);
		ReadFile(_handle, &pVertices[i].vNormal, sizeof(_float3), &dwByte, nullptr);
		ReadFile(_handle, &pVertices[i].vTexcoord, sizeof(_float2), &dwByte, nullptr);
		ReadFile(_handle, &pVertices[i].vTangent, sizeof(_float3), &dwByte, nullptr);
	}

	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::CreateBuffer(&m_pVB)))
		return E_FAIL;

	delete[] pVertices;


	return S_OK;
}

HRESULT CModelMesh::ReadyVertexBufferAnim(HANDLE _handle)
{
	_ulong dwByte = 0;

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

		ReadFile(_handle, &pVertices[i].vPosition, sizeof(_float3), &dwByte, nullptr);
		ReadFile(_handle, &pVertices[i].vNormal, sizeof(_float3), &dwByte, nullptr);
		ReadFile(_handle, &pVertices[i].vTexcoord, sizeof(_float2), &dwByte, nullptr);
		ReadFile(_handle, &pVertices[i].vTangent, sizeof(_float3), &dwByte, nullptr);

	}

	//메시의 정점들의 상태에 영향을 주는 뼈들을 가져옴

	ReadFile(_handle, &m_iNumBones, sizeof(_uint), &dwByte, nullptr);



	for (size_t i = 0; i < m_iNumBones; i++) {

		_float4x4 OffsetMatrix = {};
		ReadFile(_handle, &OffsetMatrix, sizeof(_float4x4), &dwByte, nullptr);
		m_OffsetMatrices.push_back(OffsetMatrix);

		_int iIdxNum;

		ReadFile(_handle, &iIdxNum, sizeof(_int), &dwByte, nullptr);

		m_BoneIndices.push_back(iIdxNum);

	}

	for (size_t i = 0; i < m_iNumVertices; i++) {

		ReadFile(_handle, &pVertices[i], sizeof(VTXANIMMESH), &dwByte, nullptr);

	}

	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::CreateBuffer(&m_pVB)))
		return E_FAIL;

	//Safe_Delete_Array(pVertices);

	delete[] pVertices;

	return S_OK;
}

shared_ptr<CModelMesh> CModelMesh::Create(CModel::TYPE _eType, wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, HANDLE _handle, _fmatrix PivotMatrix)
{
	shared_ptr<CModelMesh> pInstance = make_shared<CModelMesh>(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_eType, _handle, PivotMatrix)))
		MSG_BOX("Failed to Create : CMesh");

	return pInstance;
}
