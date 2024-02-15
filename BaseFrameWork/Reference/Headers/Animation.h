#pragma once

#include "Engine_Defines.h"

#include "Bone.h"
#include "Model.h"

BEGIN(Engine)


class ENGINE_DLL CAnimation : public std::enable_shared_from_this<CAnimation>
{
public:
	CAnimation();
	~CAnimation() = default;

public:
	HRESULT Initialize(ifstream& _ifs, shared_ptr<CModel> _pModel);
	_bool InvalidateTransformationMatrix(_float _fTimeDelta, const vector<shared_ptr<CBone>>& _Bones, _bool _isLoop);

public:
	//애니메이션이 끝나기 전에 다른 애니메이션으로 바뀌거나.. 그런 일이 있을 때 다음에 재생할 때를 대비해서 초기화를 시켜준다
	void AnimStateReset();
	void ChangeAnimation(shared_ptr<CAnimation> _pNextAnim, const vector<shared_ptr<CBone>>& _Bones, _float _fRatio);

public:
	vector<shared_ptr<class CChannel>>* GetChannels() { return &m_Channels; }
	_uint GetCurrentKeyFrame() { return m_iCurrentKeyFrames.front(); }
	_uint GetChannelNum() { return m_iNumChannels; }

	_char* GetAnimName() { return m_szName;}

	_double GetCurrentTrackPosition() { return m_TrackPosition; }
	_uint GetKeyFramesNum();

	_double GetAnimRatio() { return m_TrackPosition / m_Duration; }
	_double GetAnimDuration() { return m_Duration; }

public:
	void SetAnimSpeed(_float _fSpeed) { m_AnimSpeed = _fSpeed; }
	 
private:
	_float m_AnimSpeed = 1.f;


private:
	_char m_szName[MAX_PATH] = "";

	_double m_Duration = { 0.0 };
	_double m_TickPerSecond = { 0.0 };
	_double m_TrackPosition = { 0.0 };

	_uint m_iNumChannels = { 0 };
	vector<shared_ptr<class CChannel>> m_Channels = {};
	vector<_uint> m_iCurrentKeyFrames;

	_bool m_isFinished = { false };

public:
	static shared_ptr<CAnimation> Create(ifstream& _ifs, shared_ptr<CModel> _pModel);
	shared_ptr<CAnimation> Clone();

};

END

