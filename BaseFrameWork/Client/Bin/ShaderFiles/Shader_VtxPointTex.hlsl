#include "Shader_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_Texture;

vector g_RGBColor = vector(1.f, 1.f, 1.f, 1.f);

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vPSize : PSIZE; 
    
};

struct VS_OUT
{
    float4 vPosition : POSITION;
    float2 vPSize : PSIZE;
    
};


VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
   
    vector vPosition = vector(In.vPosition, 1.f);
    Out.vPosition = float4(In.vPosition, 1.f);
    
    Out.vPSize = float2(length(g_WorldMatrix._11_12_13) * In.vPSize.x,
		length(g_WorldMatrix._21_22_23) * In.vPSize.y);
    
    
    return Out;

}

struct GS_IN
{
    float4 vPosition : POSITION;
    float2 vPSize : PSIZE;
    
};

struct GS_OUT
{
    float4 vPosition : SV_Position;
    float2 vTexcoord : TEXCOORD0;
};


[maxvertexcount(6)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> DataStream)
{
    
    GS_OUT Out[4];
    
    matrix matVP = mul(g_WorldMatrix, g_ViewMatrix);
    matVP = mul(matVP, g_ProjMatrix);

    Out[0].vPosition = In[0].vPosition + float4(-0.5f , 0.5f , 0.f, 0.f);
    Out[0].vPosition = mul(Out[0].vPosition, matVP);
    Out[0].vTexcoord = float2(0.f, 0.f);


    Out[1].vPosition = In[0].vPosition + float4(0.5f , 0.5f , 0.f, 0.f);
    Out[1].vPosition = mul(Out[1].vPosition, matVP);
    Out[1].vTexcoord = float2(1.f, 0.f);

    Out[2].vPosition = In[0].vPosition + float4(0.5f , -0.5f , 0.f, 0.f);
    Out[2].vPosition = mul(Out[2].vPosition, matVP);
    Out[2].vTexcoord = float2(1.f, 1.f);

    Out[3].vPosition = In[0].vPosition + float4(-0.5f , -0.5f , 0.f, 0.f);
    Out[3].vPosition = mul(Out[3].vPosition, matVP);
    Out[3].vTexcoord = float2(0.f, 1.f);



    DataStream.Append(Out[0]);
    DataStream.Append(Out[1]);
    DataStream.Append(Out[2]);
    DataStream.RestartStrip();

    DataStream.Append(Out[0]);
    DataStream.Append(Out[2]);
    DataStream.Append(Out[3]);
    
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
    PS_OUT Out = (PS_OUT) 0;
    
    Out.vColor = g_Texture.Sample(g_LinearSampler, In.vTexcoord) * g_RGBColor;

    if (0 == Out.vColor.a)
        discard;
    
    
    return Out;

}



technique11 DefaultTechnique
{
    pass DefaultPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}