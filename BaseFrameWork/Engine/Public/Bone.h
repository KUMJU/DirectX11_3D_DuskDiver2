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

	_float4x4 GetTransformationMatrix() {
		return m_TransformationMatrix;
	}

	void SetTranslationMatrix(_fmatrix _TranslationMatrix) {
		XMStoreFloat4x4(&m_TransformationMatrix, _TranslationMatrix);
	}

public:
	HRESULT Initialize(char* _pName, _int _iParentBoneIndex, ifstream& _ifs);
	void InvalidateCombinedTransformationMatrix(const vector<shared_ptr<CBone>>& _Bones, _float3* _vRootPos, _bool _IsLinearState);

private:
	_char m_szName[MAX_PATH] = "";
	_float4x4 m_TransformationMatrix; 
	_float4x4 m_CombinedTransformationMatrix;

	_int m_iParentBoneIndex = { 0 };

	_matrix m_PrevTransformationMatrix = XMMatrixIdentity();

public:
	static shared_ptr<CBone> Create(char* _pName, _int _iParentBoneIndex, ifstream& _ifs);

};

END