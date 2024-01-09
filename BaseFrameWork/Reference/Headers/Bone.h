#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class CBone final
{
public:
	CBone();
	~CBone() = default;

public:
	HRESULT Initialize(const aiNode* _pAIBone, _int _iParentBoneIndex);

private:
	_char m_szName[MAX_PATH] = "";
	_float4x4 m_TransformationMatrix; 
	_float4x4 m_CombinedTransformationMatrix;

	_int m_iParentBoneIndex = { 0 };

public:
	static shared_ptr<CBone> Create(const aiNode* _pAIBone, _int _iParentBoneIndex);

};

END