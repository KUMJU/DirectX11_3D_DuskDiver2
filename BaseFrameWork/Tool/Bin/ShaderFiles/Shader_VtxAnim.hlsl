
#include "Shader_Defines.hlsli"

matrix g_BoneMatrices[512];

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
vector g_vLightDiffuse = vector(1.f, 1.f, 1.f, 1.f);
vector g_vLightAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector g_vLightSpecular = vector(1.f, 1.f, 1.f, 1.f);

texture2D g_DiffuseTexture;
vector g_vMtrlAmbient = vector(0.4f, 0.4f, 0.4f, 1.f);
vector g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

vector g_vLightDir = vector(1.f, -1.f, 1.f, 0.f);
vector g_vCamPosition;

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float3 vTangent : TANGENT;
    
    uint4 vBlendIndices : BLENDINDEX;
    float4 vBlendWeights : BLENDWEIGHT;
    
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;

};


VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
    
    matrix matWV, matWVP;
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    float fWeightW = 1.f - (In.vBlendWeights.x + In.vBlendWeights.y + In.vBlendWeights.z);

    matrix BoneMatrix = g_BoneMatrices[In.vBlendIndices.x] * In.vBlendWeights.x +
    g_BoneMatrices[In.vBlendIndices.y] * In.vBlendWeights.y +
    g_BoneMatrices[In.vBlendIndices.z] * In.vBlendWeights.z +
    g_BoneMatrices[In.vBlendIndices.w] * fWeightW;
    
    vector vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
    
    Out.vPosition = mul(vPosition, matWVP);
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    Out.vNormal = mul(vector(In.vNormal, 0.f), g_WorldMatrix);
    
    return Out;

}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
    vector vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

   
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(g_LinearSampler, In.vTexcoord);
   
    if (vMtrlDiffuse.a < 0.1f)
        discard;
    
    vector vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
    vector vLook = In.vWorldPos - g_vCamPosition;
    
    /* 0~1 */
    
    float fSpecular = pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 30.f);
    float fShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(In.vNormal)), 0.f);
    
    Out.vColor = ((g_vLightDiffuse * vMtrlDiffuse) * min(fShade + (g_vLightAmbient * g_vMtrlAmbient), 1.f)
		+ (g_vLightSpecular * g_vMtrlSpecular) * fSpecular);
    
    return Out;

}

technique11 DefaultTechnique
{
    pass Default
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

}