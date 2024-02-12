#include "Animation.h"
#include "Channel.h"

#include <iostream>
#include <fstream>

CAnimation::CAnimation()
{
}

HRESULT CAnimation::Initialize(ifstream& _ifs, shared_ptr<class CModel> _pModel)
{
    _ifs.read(m_szName, sizeof(char) * MAX_PATH);
    _ifs.read((char*)&m_Duration, sizeof(_double));
    _ifs.read((char*)&m_TickPerSecond, sizeof(_double));
    _ifs.read((char*)&m_iNumChannels, sizeof(_uint));

    m_iCurrentKeyFrames.resize(m_iNumChannels);
    
    for (_uint i = 0; i < m_iNumChannels; ++i) {
        
        shared_ptr<CChannel> pChannel = CChannel::Create(_ifs, _pModel);

        if (!pChannel)
            return E_FAIL;

        m_Channels.push_back(pChannel);
    }


    return S_OK;
}

_bool CAnimation::InvalidateTransformationMatrix(_float _fTimeDelta, const vector<shared_ptr<CBone>>& _Bones , _bool _isLoop)
{
    //m_TrackPosition : 약간 재생바에 현재 진행 알려주는 그 네모 같은걸로 생각하면 편한듯..
    m_TrackPosition += m_TickPerSecond * _fTimeDelta * m_AnimSpeed * g_fSlowWeight;

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
        for (size_t j = 0; i < _pNextAnim->GetChannelNum(); ++j) {

            if (m_Channels[i]->GetBoneIndex() == pChannels[j]->GetBoneIndex()) {
                CChannel::KEYFRAME nextKey = pChannels[j]->GetKeyFrame(0);
                m_Channels[i]->LinearInterpolation(m_iCurrentKeyFrames[i], nextKey, _Bones, _fRatio);

                break;
            }
        }
    }
}

_uint CAnimation::GetKeyFramesNum()
{
    return m_Channels.back()->GetKeyFramesNum();
}

shared_ptr<CAnimation> CAnimation::Create(ifstream& _ifs, shared_ptr<CModel> _pModel)
{
    shared_ptr<CAnimation> pInstance = make_shared<CAnimation>();

    if (FAILED(pInstance->Initialize(_ifs, _pModel)))
        MSG_BOX("Faile to Create : CAnimation");

    return pInstance;
}

shared_ptr<CAnimation> CAnimation::Clone()
{
    shared_ptr<CAnimation> pAnim = make_shared<CAnimation>(*(shared_from_this().get()));
    return pAnim;
}
