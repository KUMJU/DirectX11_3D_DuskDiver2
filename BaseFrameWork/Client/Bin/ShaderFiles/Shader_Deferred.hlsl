#include "Shader_Defines.hlsli"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix  g_ViewMatrixInv, g_ProjMatrixInv;

texture2D g_Texture;

vector g_vLightDir;
vector g_vLightDiffuse;
vector g_vLightAmbient;
vector g_vLightSpecular;
vector g_vLightPos;

float g_fLightRange;

vector g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

vector g_vCamPosition;

texture2D g_NormalTexture;
texture2D g_DiffuseTexture;
texture2D g_ShadeTexture;
texture2D g_DepthTexture;
texture2D g_SpecularTexture;

float g_fScreenWidth;
float g_fScreenHeight;

struct VS_IN
{
	float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;

};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
	
	return Out;
}

struct PS_IN
{
	float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
	vector vColor : SV_TARGET0;
};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

    Out.vColor = g_Texture.Sample(g_LinearSampler, In.vTexcoord);

	return Out;
}

struct PS_OUT_LIGHT
{
    vector vShade : SV_TARGET0;
    vector vSpecular : SV_TARGET1;
};

PS_OUT_LIGHT PS_MAIN_DIRECTIONAL(PS_IN In)
{
    PS_OUT_LIGHT Out = (PS_OUT_LIGHT) 0;
	
    vector vNormalDesc = g_NormalTexture.Sample(g_PointSampler, In.vTexcoord);
    vector vDepthDesc = g_DepthTexture.Sample(g_PointSampler, In.vTexcoord);

    vector vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);
	
    
    vector vShade = g_vLightDiffuse * 
    saturate(dot(normalize(g_vLightDir) * -1.f, vNormal))+
    (g_vLightAmbient * g_vMtrlAmbient);
	
    vector vToonShade = saturate(vShade);
    
    vToonShade = 0.5 * smoothstep(0.16, 0.34, vToonShade);
    Out.vShade = smoothstep(0.33, 0.67, vToonShade);
   
    vector vReflect = normalize(reflect(normalize(g_vLightDir), vNormal));
    vector vWorldPos;
    
    
    vWorldPos.x = In.vTexcoord * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord * -2.f + 1.f;
    
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.f;
    
    
    float fViewZ = vDepthDesc.y * 1000.f;
    vWorldPos = vWorldPos * fViewZ;
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
    
    vector vLook = vWorldPos - g_vCamPosition;
    
    Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vLook) * -1.f, vReflect)), 30.f);
    
    return Out;
}


PS_OUT_LIGHT PS_MAIN_POINT(PS_IN In)
{
    PS_OUT_LIGHT Out = (PS_OUT_LIGHT) 0;
	
    vector vNormalDesc = g_NormalTexture.Sample(g_PointSampler, In.vTexcoord);
    vector vDepthDesc = g_DepthTexture.Sample(g_PointSampler, In.vTexcoord);
    vector vWorldPos;
    
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.f;
    
    //Far받아와서 바꾸기 
    float fViewZ = vDepthDesc.y * 1000.f;

    vWorldPos = vWorldPos * fViewZ;
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
    
    vector vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);
    
    vector vLightDir = vWorldPos - g_vLightPos;
    float fDistance = length(vLightDir);
    
    float fAtt = saturate((g_fLightRange - fDistance) / g_fLightRange);
    Out.vShade = g_vLightDiffuse * 
    saturate(saturate(dot(normalize(vLightDir) * -1.f , vNormal)) +
    (g_vLightAmbient * g_vMtrlAmbient)) * 1.2f * fAtt;
    
    vector vReflect = normalize(reflect(normalize(vLightDir), vNormal));
    vector vLook = vWorldPos - g_vCamPosition;
    
    Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vLook) * -1.f, vReflect)), 30.f) * fAtt;
        
    return Out;
}

PS_OUT PS_MAIN_FINAL(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    vector vDiffuse = g_DiffuseTexture.Sample(g_LinearSampler, In.vTexcoord);
	
    if (0.f == vDiffuse.a)
        discard;
	
    vector vShade = g_ShadeTexture.Sample(g_LinearSampler, In.vTexcoord);
    vector vSpecular = g_SpecularTexture.Sample(g_LinearSampler, In.vTexcoord);
    	
    Out.vColor = vDiffuse * vShade + vSpecular;

    return Out;

}


technique11	DefaultTechnique
{
	pass Debug //0
	{
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

    pass Light_Directional // 1
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None_ZTestAndWrite, 0);
        SetBlendState(BS_Blending, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DIRECTIONAL();
    }

    pass Light_Point //2
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None_ZTestAndWrite, 0);
        SetBlendState(BS_Blending, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_POINT();
    }


    pass Deferred_Final //3 
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None_ZTestAndWrite, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FINAL();
    }

}


