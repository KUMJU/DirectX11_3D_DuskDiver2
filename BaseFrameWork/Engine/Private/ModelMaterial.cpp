#include "ModelMaterial.h"
#include "Texture.h"

CMaterial::CMaterial(map<_uint, shared_ptr<class CTexture>> _Textures)
    :m_Textures(_Textures)
{
}

shared_ptr<CMaterial> CMaterial::Create(map<_uint, shared_ptr<class CTexture>> _Textures)
{
    shared_ptr<CMaterial> pInstance = make_shared<CMaterial>(_Textures);
    return pInstance;
}
