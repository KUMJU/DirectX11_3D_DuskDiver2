#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class CMaterial
{
public:
	CMaterial(map<_uint, shared_ptr<class CTexture>> _Textures);
	~CMaterial() = default;

public:
	map<_uint, shared_ptr<class CTexture>> GetTextures() { return m_Textures; }

private:
	map<_uint, shared_ptr<class CTexture>> m_Textures;

public:
	static shared_ptr<CMaterial> Create(map<_uint, shared_ptr<class CTexture>> _Textures);

};

END