#include "Channel.h"

#include <iostream>
#include <fstream>

CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(ifstream& _ifs, shared_ptr<Engine::CModel> _pModel)
{
	_ifs.read(m_szName, sizeof(char) * MAX_PATH);
	m_iBoneIndex = _pModel->GetBoneIndex(m_szName);

	_ifs.read((char*)&m_iNumKeyFrames, sizeof(_uint));


	for (_uint i = 0; i < m_iNumKeyFrames; i++) {

		KEYFRAME KeyFrame = {};
	
		//if (iScaleNum > i) {
		//	_ifs.read((char*)&KeyFrame.vScale.x, sizeof(_float));
		//	_ifs.read((char*)&KeyFrame.vScale.y, sizeof(_float));
		//	_ifs.read((char*)&KeyFrame.vScale.z, sizeof(_float));
		//}

		//if (iRotationNum > i) {
		//	_ifs.read((char*)&KeyFrame.vRotation.x, sizeof(_float));
		//	_ifs.read((char*)&KeyFrame.vRotation.y, sizeof(_float));
		//	_ifs.read((char*)&KeyFrame.vRotation.z, sizeof(_float));
		//	_ifs.read((char*)&KeyFrame.vRotation.w, sizeof(_float));

		//}

		//if (iPositionNum > i) {
		//	_ifs.read((char*)&KeyFrame.vPosition.x, sizeof(_float));
		//	_ifs.read((char*)&KeyFrame.vPosition.y, sizeof(_float));
		//	_ifs.read((char*)&KeyFrame.vPosition.z, sizeof(_float));

		//}

		//_ifs.read((char*)&KeyFrame.Time, sizeof(_double));

		//_int a = 5;
		////_ifs.read((char*)&KeyFrame.vPosition.x, sizeof(_float));
		////_ifs.read((char*)&KeyFrame.vPosition.y, sizeof(_float));
		////_ifs.read((char*)&KeyFrame.vPosition.z, sizeof(_float));

		////_ifs.read((char*)&KeyFrame.vRotation.x, sizeof(_float));
		////_ifs.read((char*)&KeyFrame.vRotation.y, sizeof(_float));
		////_ifs.read((char*)&KeyFrame.vRotation.z, sizeof(_float));
		////_ifs.read((char*)&KeyFrame.vRotation.w, sizeof(_float));

		////_ifs.read((char*)&KeyFrame.Time, sizeof(_double));


		_ifs.read((char*)&KeyFrame, sizeof(KEYFRAME));
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

	vScale = XMVectorLerp(XMLoadFloat3(&m_KeyFrames[_iCurFrame].vScale), XMLoadFloat3(&_NextKeyFrame.vScale), _fRatio);
	vTranslation = XMVectorSetW(XMVectorLerp(XMLoadFloat3(&m_KeyFrames[_iCurFrame].vPosition), XMLoadFloat3(&_NextKeyFrame.vPosition), _fRatio), 1.f);
	vRotation = XMQuaternionSlerp(XMLoadFloat4(&m_KeyFrames[_iCurFrame].vRotation), XMLoadFloat4(&_NextKeyFrame.vRotation), _fRatio);

	TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);
	_Bones[m_iBoneIndex]->SetTranslationMatrix(TransformationMatrix);

	return TransformationMatrix;
}

shared_ptr<CChannel> CChannel::Create(ifstream& _ifs, shared_ptr<CModel> _pModel)
{
	shared_ptr<CChannel> pInstance = make_shared<CChannel>();

	if (FAILED(pInstance->Initialize(_ifs, _pModel)))
		MSG_BOX("Failed to Create : CChannel");

	return pInstance;
}
