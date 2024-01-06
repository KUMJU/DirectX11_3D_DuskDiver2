#include "Shader.h"

CShader::CShader(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
    :m_pDevice(_pDevice) , m_pContext(_pContext)
{
}




HRESULT CShader::Initialize(const wstring& strShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
    _uint iHlslFlag = { 0 };

#ifdef _DEBUG
    iHlslFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    iHlslFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1; // =0
#endif

    if (FAILED(D3DX11CompileEffectFromFile(strShaderFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iHlslFlag, 0, m_pDevice.Get(), &m_pEffect, nullptr)))
        return E_FAIL;

    D3DX11_EFFECT_DESC EffectDesc = {};
    m_pEffect->GetDesc(&EffectDesc);

    for (size_t i = 0; i < EffectDesc.Techniques; i++) {

        wrl::ComPtr<ID3DX11EffectTechnique> pTechnique = m_pEffect->GetTechniqueByIndex((uint32_t)i);
        if (!pTechnique)
            return E_FAIL;

        D3DX11_TECHNIQUE_DESC TechniqueDesc = {};
        pTechnique->GetDesc(&TechniqueDesc);

        for (size_t j = 0; j < TechniqueDesc.Passes; j++) {

            wrl::ComPtr<ID3D11InputLayout> pInputLayout = { nullptr };
            wrl::ComPtr<ID3DX11EffectPass> pPass = pTechnique->GetPassByIndex((uint32_t)i);
            if (!pPass)
                return E_FAIL;

            D3DX11_PASS_DESC PassDesc = {};
            pPass->GetDesc(&PassDesc);

            if (FAILED(m_pDevice->CreateInputLayout(pElements, iNumElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pInputLayout)))
                return E_FAIL;

            m_InputLayouts.push_back(pInputLayout);
        }
    }

    return S_OK;
}

HRESULT CShader::Begin(_uint iPassIndex, _uint iTechniqueIndex)
{
    wrl::ComPtr<ID3DX11EffectTechnique> pTechnique = m_pEffect->GetTechniqueByIndex(iTechniqueIndex);
    if (!pTechnique)
        return E_FAIL;

    wrl::ComPtr<ID3DX11EffectPass> pPass = pTechnique->GetPassByIndex(iPassIndex);
    if (!pPass)
        return E_FAIL;

    pPass->Apply(0, m_pContext.Get());
    m_pContext->IASetInputLayout(m_InputLayouts[iPassIndex].Get());

    return S_OK;
}

HRESULT CShader::BindRawValue(const _char* _pConstantName, const void* _pData, _uint _iLength)
{
    ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(_pConstantName);
    if (nullptr == pVariable)
        return E_FAIL;

    return pVariable->SetRawValue(_pData, 0, _iLength);
}

HRESULT CShader::BindMatrix(const _char* pConstantName, const _float4x4* pMatrix)
{
    wrl::ComPtr<ID3DX11EffectVariable> pVariable = m_pEffect->GetVariableByName(pConstantName);
    if (!pVariable)
        return E_FAIL;

    wrl::ComPtr<ID3DX11EffectMatrixVariable> pMatrixVariable = pVariable->AsMatrix();
    if (!pMatrixVariable)
        return E_FAIL;

    return pMatrixVariable->SetMatrix((_float*)pMatrix);
}

HRESULT CShader::BindMatrices(const _char* pConstantName, const _float4x4* pMatrices, _uint iNumMatrices)
{
    wrl::ComPtr<ID3DX11EffectVariable> pVariable = m_pEffect->GetVariableByName(pConstantName);
    if (!pVariable)
        return E_FAIL;

    wrl::ComPtr<ID3DX11EffectMatrixVariable> pMatrixVariable = pVariable->AsMatrix();
    if (!pMatrixVariable)
        return E_FAIL;

    return pMatrixVariable->SetMatrixArray((_float*)pMatrices, 0, iNumMatrices);
}

HRESULT CShader::BindSRV(const _char* pConstantName, wrl::ComPtr<ID3D11ShaderResourceView> _pSRV)
{
    wrl::ComPtr<ID3DX11EffectVariable> pVariable = m_pEffect->GetVariableByName(pConstantName);

    if (!pVariable)
        return E_FAIL;

    wrl::ComPtr<ID3DX11EffectShaderResourceVariable> pSRVariable = pVariable->AsShaderResource();

    if (!pSRVariable)
        return E_FAIL;

    return pSRVariable->SetResource(_pSRV.Get());
}

HRESULT CShader::BindSRVs(const _char* pConstantName, ID3D11ShaderResourceView** _pSRV, _uint _iNumTextures)
{
    wrl::ComPtr<ID3DX11EffectVariable> pVariable = m_pEffect->GetVariableByName(pConstantName);

    if (!pVariable)
        return E_FAIL;

    wrl::ComPtr<ID3DX11EffectShaderResourceVariable> pSRVariable = pVariable->AsShaderResource();

    if (!pSRVariable)
        return E_FAIL;

    return pSRVariable->SetResourceArray(_pSRV , 0, _iNumTextures);
}

shared_ptr<CShader> CShader::Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, const wstring& strShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
    shared_ptr<CShader> pInstance = make_shared<CShader>(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize(strShaderFilePath, pElements, iNumElements)))
        MSG_BOX("Failed to Created : CShader");

    return pInstance;
}