#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class CBone final
{
public:
	CBone();
	~CBone() = default;

public:
	const _char* GetBoneName() const {
		return m_szName;
	}

	_float4x4 GetCombiendTransformationMatrix() const {
		return m_CombinedTransformationMatrix;
	}

public:
	HRESULT Initialize(const aiNode* _pAIBone, _int _iParentBoneIndex);
	void InvalidateCombinedTransformationMatrix(const vector<shared_ptr<CBone>>& _Bones);

private:
	_char m_szName[MAX_PATH] = "";
	_float4x4 m_TransformationMatrix; 
	_float4x4 m_CombinedTransformationMatrix;

	_int m_iParentBoneIndex = { 0 };

public:
	static shared_ptr<CBone> Create(const aiNode* _pAIBone, _int _iParentBoneIndex);

};

END