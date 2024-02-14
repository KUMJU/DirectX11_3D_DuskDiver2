
#include "Shader_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D g_DiffuseTexture;

bool g_bDiffuseTex = false;

texture2D g_MaskTexture;
bool g_bMaskTex = false;
float g_vMaskSpeed;
float2 g_vMaskDirection;

texture2D g_NoiseTexture;
bool g_bNoiseTex = false;
float g_vNoiseSpeed;
float2 g_vNoiseDirection;


vector g_vColor;
vector g_vLerpColor;

float g_fDeltaTime;

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float3 vTangent : TANGENT;
    
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
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
    
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
    vector vDiffuse : SV_TARGET0;
    vector vNormal : SV_TARGET1;
    
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

   
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(g_LinearSampler, In.vTexcoord);
   
    if (vMtrlDiffuse.a < 0.1f)
        discard;
    

    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

    return Out;

}

PS_OUT PS_COLOR(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(g_LinearSampler, In.vTexcoord) * g_vColor;
   
    if (vMtrlDiffuse.a < 0.5f)
        discard;
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    
    return Out;
}


struct PS_EFFECT_OUT
{
    vector vDiffuse : SV_TARGET0;    
};

PS_EFFECT_OUT PS_EFFECT_LOOP_MAIN(PS_IN In)
{
    PS_EFFECT_OUT Out = (PS_EFFECT_OUT) 0;

    float fR = lerp(g_vColor.r, g_vLerpColor.r, In.vTexcoord.y);
    float fG = lerp(g_vColor.g, g_vLerpColor.g, In.vTexcoord.y);
    float fB = lerp(g_vColor.b, g_vLerpColor.b, In.vTexcoord.y);
        
        
    Out.vDiffuse.rgb = float3(fR, fG, fB);
    
    
    
    if (g_bDiffuseTex)
    {
        Out.vDiffuse = g_DiffuseTexture.Sample(g_LinearSampler, In.vTexcoord);

    }
    
    if (g_bMaskTex)
    {
        float2 MaskUVOffset = In.vTexcoord;
    
        MaskUVOffset.x += g_vMaskDirection.x * g_vMaskSpeed * g_fDeltaTime;
        MaskUVOffset.y += g_vMaskDirection.y * g_vMaskSpeed * g_fDeltaTime;

        vector vMask = g_MaskTexture.Sample(g_LinearSampler, MaskUVOffset);

        
        Out.vDiffuse.a = vMask.r;  
    }
    
    if (g_bNoiseTex)
    {
        float2 NoiseUVOffset = In.vTexcoord;
    
        NoiseUVOffset.x += g_vNoiseDirection.x * g_vNoiseSpeed * g_fDeltaTime;
        NoiseUVOffset.y += g_vNoiseDirection.y * g_vNoiseSpeed * g_fDeltaTime;

        vector vNoise = g_NoiseTexture.Sample(g_LinearSampler, NoiseUVOffset);
        
        float lerpValue = 0.5f + (sin(g_fDeltaTime * 5.0f) * 0.5f);
        float vNoiseTex = lerp(vNoise.r, vNoise.r * 1.5f, lerpValue);
        
        Out.vDiffuse.a *= vNoiseTex;
    }

    if (0 == Out.vDiffuse.a)
        discard;
   
    
    return Out;

}


PS_EFFECT_OUT PS_EFFECT_ONCE_MAIN(PS_IN In)
{
    PS_EFFECT_OUT Out = (PS_EFFECT_OUT) 0;

    if (g_bDiffuseTex)
    {
        Out.vDiffuse = g_DiffuseTexture.Sample(g_LinearSampler, In.vTexcoord);

    }
    
    if (g_bMaskTex)
    {
        float2 MaskUVOffset = In.vTexcoord;
    
        MaskUVOffset.x += g_vMaskDirection.x * g_vMaskSpeed * g_fDeltaTime;
        MaskUVOffset.y += g_vMaskDirection.y * g_vMaskSpeed * g_fDeltaTime;
        
        
        vector vMask = g_MaskTexture.Sample(g_LinearClampSampler, MaskUVOffset);

        
        Out.vDiffuse.a = vMask.r;
        
        float fR = lerp(g_vColor.r, g_vLerpColor.r, In.vTexcoord.y);
        float fG = lerp(g_vColor.g, g_vLerpColor.g, In.vTexcoord.y);
        float fB = lerp(g_vColor.b, g_vLerpColor.b, In.vTexcoord.y);
        
        
        Out.vDiffuse.rgb = float3(fR, fG, fB);
        
    }
    
    if (g_bNoiseTex)
    {
        float2 NoiseUVOffset = In.vTexcoord;
    
        NoiseUVOffset.x += g_vNoiseDirection.x * g_vNoiseSpeed * g_fDeltaTime;
        NoiseUVOffset.y += g_vNoiseDirection.y * g_vNoiseSpeed * g_fDeltaTime;

        vector vNoise = g_NoiseTexture.Sample(g_LinearSampler, NoiseUVOffset);
        
        float lerpValue = 0.5f + (sin(g_fDeltaTime * 5.0f) * 0.5f);
        float vNoiseTex = lerp(vNoise.r, vNoise.r * 1.5f, lerpValue);
        
        Out.vDiffuse.a *= vNoiseTex;
    }

    if (0 == Out.vDiffuse.a)
        discard;
   
    
    return Out;
}

technique11 DefaultTechnique
{
    pass Default // 0
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass Color // 1 
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_COLOR();
    }


    pass Effect_Loop // 2
    {
        SetRasterizerState(RS_None_Cull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_EFFECT_LOOP_MAIN();
    }

    pass Effect_Once // 3
    {
        SetRasterizerState(RS_None_Cull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_EFFECT_ONCE_MAIN();
    }

}