
#include "Shader_Defines.hlsli"

matrix g_BoneMatrices[512];

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_DiffuseTexture;

/*림라이트*/
bool g_bRimLight = false;
float3 g_vRimColor;
float4 g_vCamLook;

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
    float4 vProjPos : TEXCOORD2;

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
    vector vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);
    
    Out.vPosition = mul(vPosition, matWVP);
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    Out.vNormal = normalize(mul(vNormal, g_WorldMatrix));
    Out.vProjPos = Out.vPosition;
    
    return Out;

}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

struct PS_OUT
{
    vector vDiffuse : SV_TARGET0;
    vector vNormal : SV_TARGET1;
    vector vDepth : SV_TARGET2;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

   
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(g_LinearSampler, In.vTexcoord);
   
    


    if (vMtrlDiffuse.a < 0.1f)
        discard;
    
    Out.vDiffuse = float4(vMtrlDiffuse.xyz, 1);
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    
    //Far 받아와서 처리 
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

    
    return Out;

}

PS_OUT PS_RIMLIGHT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

   
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(g_LinearSampler, In.vTexcoord);
   
    if (vMtrlDiffuse.a < 0.1f)
        discard;
    
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    //Far 받아와서 처리 
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

    
    if (g_bRimLight)
    {
        float rim = 1 - saturate(dot(In.vNormal, -g_vCamLook));
        rim = pow(rim, 2.f);
        
        float3 rimColor = rim * g_vRimColor;
        Out.vDiffuse.rgb = Out.vDiffuse.rgb + rimColor.rgb;
    }
    
    return Out;

}


PS_OUT PS_MOTIONTRAIL(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;


    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    //Far 받아와서 처리 
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);    

   float rim = 1 - saturate(dot(In.vNormal, -g_vCamLook));
   rim = pow(rim, 2.f);
   
    vector vRimColor = vector(0.8f, 0.7f, 0.1f, 1.f);
    
    float3 rimColor = rim * vRimColor;
    Out.vDiffuse = vector(rimColor,0.1f);
    
    return Out;

}


technique11 DefaultTechnique
{
    pass Default //0
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass RimLight //1
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_RIMLIGHT();
    }

    pass MotionTrail //2
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MOTIONTRAIL();
    }


}