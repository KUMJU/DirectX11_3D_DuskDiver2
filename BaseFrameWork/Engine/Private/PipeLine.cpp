#include "PipeLine.h"

CPipeLine::CPipeLine()
{
}

void CPipeLine::Tick()
{

	for (size_t i = 0; i < D3DTS_END; i++)
	{
		XMStoreFloat4x4(&m_TransformMatrix_Inverse[i], XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_TransformMatrix[i])));
	}

	memcpy(&m_vCamPosition, &m_TransformMatrix_Inverse[D3DTS_VIEW].m[3][0], sizeof(_float4));
	memcpy(&m_vCamLook, &m_TransformMatrix_Inverse[D3DTS_VIEW].m[2][0], sizeof(_float4));

}

shared_ptr<CPipeLine> CPipeLine::Create()
{
	return make_shared<CPipeLine>();
}
