#include "Shader_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_Texture;

float g_HPRatio;
float g_PrevHPRatio;

vector g_RGBColor = vector(1.f, 1.f, 1.f, 1.f);

bool g_bSkillBarState = true;

//가로
float g_fStartRowUV;
float g_fEndRowUV;
//세로
float g_fStartColUV;
float g_fEndColUV;

float g_fBurstGaugeRatio = 0.f;
float g_fSkillGaugeRatio = 0.f;

float g_fBurstSkillRadian = 0.f;

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
   
    matrix matWVP = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWVP, g_ProjMatrix);
    Out.vPosition = float4(In.vPosition, 1.f);
    Out.vPSize = float2(g_WorldMatrix._11 * In.vPSize.x, g_WorldMatrix._22* In.vPSize.y);
    
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
    
    matrix matVP = mul(g_ViewMatrix, g_ProjMatrix);
    matVP = mul(g_WorldMatrix, matVP);

    
    Out[0].vPosition = In[0].vPosition + float4(-0.5f,-0.5f, 0.f, 0.f);
    Out[0].vPosition = mul(Out[0].vPosition, matVP);
    Out[0].vTexcoord = float2(0.f, 1.f);

    Out[1].vPosition = In[0].vPosition + float4(0.5f,-0.5f, 0.f, 0.f);
    Out[1].vPosition = mul(Out[1].vPosition, matVP);
    Out[1].vTexcoord = float2(1.f, 1.f);

    Out[2].vPosition = In[0].vPosition + float4( 0.5f, 0.5f, 0.f, 0.f);
    Out[2].vPosition = mul(Out[2].vPosition, matVP);
    Out[2].vTexcoord = float2(1.f, 0.f);
        
    Out[3].vPosition = In[0].vPosition + float4(-0.5f, 0.5f, 0.f, 0.f);
    Out[3].vPosition = mul(Out[3].vPosition, matVP);
    Out[3].vTexcoord = float2(0.f, 0.f);

    DataStream.Append(Out[3]);
    DataStream.Append(Out[2]);
    DataStream.Append(Out[1]);
    DataStream.RestartStrip();

    DataStream.Append(Out[3]);
    DataStream.Append(Out[1]);
    DataStream.Append(Out[0]);
    

    
}

[maxvertexcount(6)]
void GS_SEQUENCE_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> DataStream)
{
    GS_OUT Out[4];
    
    matrix matVP = mul(g_ViewMatrix, g_ProjMatrix);
    matVP = mul(g_WorldMatrix, matVP);

    
    //Out[0].vPosition = In[0].vPosition + float4(-0.5f, -0.5f, 0.f, 0.f);
    //Out[0].vPosition = mul(Out[0].vPosition, matVP);
    //Out[0].vTexcoord = float2(0.5f, 0.5f);
    //
    //Out[1].vPosition = In[0].vPosition + float4(0.5f, -0.5f, 0.f, 0.f);
    //Out[1].vPosition = mul(Out[1].vPosition, matVP);
    //Out[1].vTexcoord = float2(0.75f, 0.5f);
    //
    //Out[2].vPosition = In[0].vPosition + float4(0.5f, 0.5f, 0.f, 0.f);
    //Out[2].vPosition = mul(Out[2].vPosition, matVP);
    //Out[2].vTexcoord = float2(0.75f, 0.75f);
    //  
    //Out[3].vPosition = In[0].vPosition + float4(-0.5f, 0.5f, 0.f, 0.f);
    //Out[3].vPosition = mul(Out[3].vPosition, matVP);
    //Out[3].vTexcoord = float2(0.5f, 0.75f);
    
    Out[0].vPosition = In[0].vPosition + float4(-0.5f, -0.5f, 0.f, 0.f);
    Out[0].vPosition = mul(Out[0].vPosition, matVP);
    Out[0].vTexcoord = float2(g_fStartRowUV, g_fStartColUV);
    
    Out[1].vPosition = In[0].vPosition + float4(0.5f, -0.5f, 0.f, 0.f);
    Out[1].vPosition = mul(Out[1].vPosition, matVP);
    Out[1].vTexcoord = float2(g_fEndRowUV, g_fStartColUV);
    
    //g_fEndColUV g_fStartColUV
    Out[2].vPosition = In[0].vPosition + float4(0.5f, 0.5f, 0.f, 0.f);
    Out[2].vPosition = mul(Out[2].vPosition, matVP);
    Out[2].vTexcoord = float2(g_fEndRowUV, g_fEndColUV);
    
    Out[3].vPosition = In[0].vPosition + float4(-0.5f, 0.5f, 0.f, 0.f);
   Out[3].vPosition = mul(Out[3].vPosition, matVP);
    Out[3].vTexcoord = float2(g_fStartRowUV, g_fEndColUV);

    DataStream.Append(Out[3]);
    DataStream.Append(Out[2]);
    DataStream.Append(Out[1]);
    DataStream.RestartStrip();

    DataStream.Append(Out[3]);
    DataStream.Append(Out[1]);
    DataStream.Append(Out[0]);
    

    
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

    Out.vColor = g_Texture.Sample(g_LinearSampler, In.vTexcoord);    
  
    if (Out.vColor.a  < 0.2f)
        discard;   
 
   return Out;

}

PS_OUT PS_MAIN_POINT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture.Sample(g_LinearSampler, In.vTexcoord);
    
    if (Out.vColor.a < 0.4f)
        discard;
  
    Out.vColor.r = 0.8f;
    Out.vColor.g = 0.2f;
    Out.vColor.b = 0.5f;
    
    //y포지션이 아래에 있을수록 alpha값을 낮게하는 예시 코드
    //max(a,b)면 a가 b보다 작을 경우 b로 값을 대신해준다
  
    
    return Out;

}

PS_OUT PS_MAIN_BLEND(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture.Sample(g_LinearSampler, In.vTexcoord);
  
    if (Out.vColor.a == 0.f)
        discard;
 
    return Out;

}

PS_OUT PS_HPBar(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture.Sample(g_LinearSampler, In.vTexcoord);
    
    //원래 색
    
   
    if (In.vTexcoord.x < g_PrevHPRatio && g_HPRatio < In.vTexcoord.x)
    {
        Out.vColor.rgb = float3(1.f, 0.3f, 0.1f);
    }
    else if(In.vTexcoord.r <= g_HPRatio)
    {
        Out.vColor.rgb = float3(0.f, 1.f, 0.8f);
    }
    else
    {
        Out.vColor.rgb = float3(0.25f, 0.25f, 0.25f);
    }
    
    return Out;

}


PS_OUT PS_ENEMY_HPBAR(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    Out.vColor = g_Texture.Sample(g_LinearSampler, In.vTexcoord);
    
    
    if (In.vTexcoord.x < g_PrevHPRatio && g_HPRatio < In.vTexcoord.x)
    {
        Out.vColor.rgb = float3(1.f, 0.3f, 0.1f);
    }
    else if (g_PrevHPRatio <= In.vTexcoord.x)
    {
        Out.vColor.rgb = float3(0.25f, 0.25f, 0.25f);
    }
    
    return Out;
}

PS_OUT PS_SKILLBAR(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture.Sample(g_LinearSampler, In.vTexcoord);
    
    if (In.vTexcoord.x <= g_fSkillGaugeRatio)
    {
        Out.vColor.rgb = float3(0.03f, 0.63f, 0.94f);
    }
    else
    {
        Out.vColor.rgb = float3(0.25f, 0.25f, 0.25f);
    }
 
    return Out;

}

PS_OUT PS_BURSTBAR(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    if (In.vTexcoord.x <= g_fBurstGaugeRatio)
    {
        Out.vColor.rgb = float3(0.99f, 0.99f, 0.007f);
    }
    else
    {
        Out.vColor.rgb = float3(0.25f, 0.25f, 0.25f);
    }
 
    return Out;

}

PS_OUT PS_BURSTSKILLBAR(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
   
    Out.vColor = g_Texture.Sample(g_LinearSampler, In.vTexcoord);

    float2x2 RotationMat = { cos(radians(-90.f)), 
        -sin(radians(-90.f)), 
        sin(radians(-90.f)), 
        cos(radians(-90.f))};
    
    //극축 기준이 (0,0)이라서 변환이 필요함
    //내가 극축 기준으로 잡고 싶은 좌표는 (0.5f , 1.f)  
       
    float2 vDirVec = In.vTexcoord - 0.5f;
    
    vDirVec = mul(vDirVec, RotationMat);

    float fPolarCoord = atan2(-vDirVec.y, vDirVec.x);
   
    
    if (fPolarCoord <= g_fBurstSkillRadian)
    {
        Out.vColor.rgb = float3(0.99f, 0.99f, 0.007f);
    }
    else
    {
        Out.vColor.rgb = float3(0.25f, 0.25f, 0.25f);
    }
    
    
    if (Out.vColor.a == 0.f)
        discard;
 
    return Out;

}


technique11 DefaultTechnique
{
    //pass를 여러개로 나눠서 한 셰이더 파일 안에 여러 진입점, 다른 효과를 줄 수도 있음
    pass DefaultPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass PointPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN_POINT();
    }

    pass HPBar //2 
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_HPBar();
    }

    pass EnemyHPBar //3 
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_ENEMY_HPBAR();
    }

    pass SequenceUI //4
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_SEQUENCE_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass SkillBar //5
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_SKILLBAR();
    }

    pass BurstBar //6
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_BURSTBAR();
    }

    pass BurstSkillBar //7
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_BURSTSKILLBAR();
    }

    pass AlphaBlend //8
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN_BLEND();
    }


}