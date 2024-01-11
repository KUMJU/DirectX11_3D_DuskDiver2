#pragma once

#include "Engine_Defines.h"

#include "Bone.h"
#include "Model.h"

BEGIN(Engine)

class CAnimation
{
public:
	CAnimation();
	~CAnimation() = default;

public:
	HRESULT Initialize(const aiAnimation* _pAIAnimation, shared_ptr<CModel> _pModel);
	_bool InvalidateTransformationMatrix(_float _fTimeDelta, const vector<shared_ptr<CBone>>& _Bones, _bool _isLoop);

public:
	//애니메이션이 끝나기 전에 다른 애니메이션으로 바뀌거나.. 그런 일이 있을 때 다음에 재생할 때를 대비해서 초기화를 시켜준다
	void AnimStateReset();
	void ChangeAnimation(shared_ptr<CAnimation> _pNextAnim, const vector<shared_ptr<CBone>>& _Bones, _float _fRatio);

public:
	vector<shared_ptr<class CChannel>>* GetChannels() { return &m_Channels; }
	_uint GetCurrentKeyFrame() { return m_iCurrentKeyFrames.front(); }
	_uint GetChannelNum() { return m_iNumChannels; }

private:
	_uint m_iNumAnimations = { 0 };
	_char m_szName[MAX_PATH] = "";

	_double m_Duration = { 0.0 };
	_double m_TickPerSecond = { 0.0 };
	_double m_TrackPosition = { 0.0 };

	_uint m_iNumChannels = { 0 };
	vector<shared_ptr<class CChannel>> m_Channels = {};
	vector<_uint> m_iCurrentKeyFrames;

	_bool m_isFinished = { false };

public:
	static shared_ptr<CAnimation> Create(const aiAnimation* pAIAnimation, shared_ptr<CModel> _pModel);

};

END

