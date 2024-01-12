#include "Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
}

HRESULT CAnimation::Initialize(HANDLE _handle, shared_ptr<class CModel> _pModel)
{
    _ulong dwByte = 0;

    ReadFile(_handle, m_szName, sizeof(char) * MAX_PATH, &dwByte, nullptr);

    ReadFile(_handle, &m_Duration, sizeof(_double), &dwByte, nullptr);
    ReadFile(_handle, &m_TickPerSecond, sizeof(_double), &dwByte, nullptr);
    ReadFile(_handle, &m_iNumChannels, sizeof(_uint), &dwByte, nullptr);

    m_iCurrentKeyFrames.resize(m_iNumChannels);
    
    for (size_t i = 0; i < m_iNumChannels; i++) {

        shared_ptr<CChannel> pChannel = CChannel::Create(_handle, _pModel);

        if (!pChannel)
            return E_FAIL;

        m_Channels.push_back(pChannel);
    }


    return S_OK;
}

_bool CAnimation::InvalidateTransformationMatrix(_float _fTimeDelta, const vector<shared_ptr<CBone>>& _Bones , _bool _isLoop)
{
    //m_TrackPosition : 약간 재생바에 현재 진행 알려주는 그 네모 같은걸로 생각하면 편한듯..
    m_TrackPosition += m_TickPerSecond * _fTimeDelta;

    if (m_TrackPosition >= m_Duration) {

        if (!_isLoop) {

            m_isFinished = true;
            return m_isFinished;

        }
        else {

            m_TrackPosition = 0.0;
        }
    }

    for (size_t i = 0; i < m_iNumChannels; i++)
        m_Channels[i]->InvalidateTransformationMatrix(m_TrackPosition, &m_iCurrentKeyFrames[i], _Bones);

    return false;
}

void CAnimation::AnimStateReset()
{
    m_TrackPosition = 0.0;
    
    for (auto& iter : m_iCurrentKeyFrames)
        iter = 0;
}

void CAnimation::ChangeAnimation(shared_ptr<CAnimation> _pNextAnim, const vector<shared_ptr<CBone>>& _Bones, _float _fRatio)
{
    vector<shared_ptr<CChannel>> pChannels = *(_pNextAnim->GetChannels());
    _uint iCurrentKeyFrame = _pNextAnim->GetCurrentKeyFrame();

    for (size_t i = 0; i < m_iNumChannels; ++i) {
        CChannel::KEYFRAME nextKey = pChannels[i]->GetKeyFrame(0);
        m_Channels[i]->LinearInterpolation(m_iCurrentKeyFrames[i], nextKey, _Bones, _fRatio);
    }
}

shared_ptr<CAnimation> CAnimation::Create(HANDLE _handle, shared_ptr<CModel> _pModel)
{
    shared_ptr<CAnimation> pInstance = make_shared<CAnimation>();

    if (FAILED(pInstance->Initialize(_handle, _pModel)))
        MSG_BOX("Faile to Create : CAnimation");

    return pInstance;
}
