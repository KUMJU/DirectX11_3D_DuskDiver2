#include "Channel.h"


CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(const aiNodeAnim* _pAIChannel, shared_ptr<Engine::CModel> _pModel)
{
	strcpy_s(m_szName, _pAIChannel->mNodeName.data);

	m_iBoneIndex = _pModel->GetBoneIndex(m_szName);

	m_iNumKeyFrames = max(_pAIChannel->mNumScalingKeys, _pAIChannel->mNumRotationKeys);
	m_iNumKeyFrames = max(m_iNumKeyFrames, _pAIChannel->mNumPositionKeys);

	_float3 vScale;
	_float4 vRotation;
	_float3 vPosition;


	for (size_t i = 0; i < m_iNumKeyFrames; i++) {

		KEYFRAME KeyFrame = {};

		//키는 중간에 빈공간이 없이 연속적으로 있음 + 어느 한 키는 전체 프레임 키만큼 존재하기에 이렇게 for문을 작성해도 괜찮다
		if (_pAIChannel->mNumScalingKeys > i) {

			memcpy(&vScale, &_pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
			KeyFrame.Time = _pAIChannel->mScalingKeys[i].mTime;
		}

		if (_pAIChannel->mNumRotationKeys > i) {
			//wxyz 순서로 들어있어서 직접 매칭해줘야함

			vRotation.x = _pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y = _pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z = _pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w = _pAIChannel->mRotationKeys[i].mValue.w;
			KeyFrame.Time = _pAIChannel->mScalingKeys[i].mTime;

		}

		if (_pAIChannel->mNumPositionKeys > i) {

			memcpy(&vPosition, &_pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
			KeyFrame.Time = _pAIChannel->mPositionKeys[i].mTime;
		}


		KeyFrame.vScale = vScale;
		KeyFrame.vRotation = vRotation;
		KeyFrame.vPosition = vPosition;

		m_KeyFrames.push_back(KeyFrame);

	}

	return S_OK;
}

void CChannel::InvalidateTransformationMatrix(_double _TrackPosition, _uint* _pCurrentKeyFrame, const vector<shared_ptr<Engine::CBone>>& Bones)
{

	if (0.0 == _TrackPosition)
		*_pCurrentKeyFrame = 0;


	KEYFRAME LastKeyFrame = m_KeyFrames.back();
	_matrix TransformationMatrix;

	_vector vScale, vRotation, vTranslation;

	//마지막 프레임보다 큰 경우
	if (_TrackPosition >= LastKeyFrame.Time) {
		
		vScale = XMLoadFloat3(&LastKeyFrame.vScale);
		vRotation =  XMLoadFloat4(&LastKeyFrame.vRotation);
		vTranslation = XMVectorSetW(XMLoadFloat3(&LastKeyFrame.vPosition),1.f);

	}
	else 
	{
		//다음 키 프레임으로 넘어감
		if (_TrackPosition >= m_KeyFrames[*_pCurrentKeyFrame + 1].Time)
			++*_pCurrentKeyFrame;

		//현재 프레임 진행 정도
		_double Ratio = (_TrackPosition - m_KeyFrames[*_pCurrentKeyFrame].Time) /
			(m_KeyFrames[*_pCurrentKeyFrame + 1].Time - m_KeyFrames[*_pCurrentKeyFrame].Time);

		vScale = XMVectorLerp(XMLoadFloat3(&m_KeyFrames[*_pCurrentKeyFrame].vScale), XMLoadFloat3(&m_KeyFrames[*_pCurrentKeyFrame + 1].vScale), (_float)Ratio);
		vTranslation = XMVectorSetW(XMVectorLerp(XMLoadFloat3(&m_KeyFrames[*_pCurrentKeyFrame].vPosition), XMLoadFloat3(&m_KeyFrames[*_pCurrentKeyFrame + 1].vPosition), (_float)Ratio), 1.f);
		vRotation = XMQuaternionSlerp(XMLoadFloat4(&m_KeyFrames[*_pCurrentKeyFrame].vRotation), XMLoadFloat4(&m_KeyFrames[*_pCurrentKeyFrame + 1].vRotation), (_float)Ratio);

	}

	TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);
	Bones[m_iBoneIndex]->SetTranslationMatrix(TransformationMatrix);

}

_matrix CChannel::LinearInterpolation(_uint _iCurFrame, KEYFRAME _NextKeyFrame, const vector<shared_ptr<CBone>>& _Bones , _float _fRatio)
{
	_vector vScale, vRotation, vTranslation;
	_matrix TransformationMatrix;

	vScale = XMVectorLerp(XMLoadFloat3(&m_KeyFrames[_iCurFrame].vScale), XMLoadFloat3(&_NextKeyFrame.vScale), 0.3f);
	vTranslation = XMVectorSetW(XMVectorLerp(XMLoadFloat3(&m_KeyFrames[_iCurFrame].vPosition), XMLoadFloat3(&_NextKeyFrame.vPosition), 0.3f), 1.f);
	vRotation = XMQuaternionSlerp(XMLoadFloat4(&m_KeyFrames[_iCurFrame].vRotation), XMLoadFloat4(&_NextKeyFrame.vRotation), 0.3f);

	TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);
	_Bones[m_iBoneIndex]->SetTranslationMatrix(TransformationMatrix);

	return TransformationMatrix;
}

shared_ptr<CChannel> CChannel::Create(const aiNodeAnim* _pAIChannel, shared_ptr<CModel> _pModel)
{
	shared_ptr<CChannel> pInstance = make_shared<CChannel>();

	if (FAILED(pInstance->Initialize(_pAIChannel, _pModel)))
		MSG_BOX("Failed to Create : CChannel");

	return pInstance;
}
