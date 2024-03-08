#include "Shader_Defines.hlsli"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D g_Texture;

//Final까지 그려진 최종 텍스쳐 
texture2D g_BackBufferTexture;
//백버퍼에 섞을 포스트 프로세싱이 끝난 텍스쳐 
texture2D g_BlendTexture;
texture2D g_OutLineTexture;

/*디스토션*/
texture2D g_DistortionTex;
float g_fDistortionTime; 
float g_fStrength;
float g_fDistortionSpeed;

texture2D g_NoiseTexture;


/*글로우*/
texture2D g_GlowTexture;

float g_fScreenWidth;
float g_fScreenHeight;

/*줌 블러*/

float g_fFocusPow = 40.f;
float2 g_fFocusCenter;
uint g_iFocusDetail = 5;

//외곽선 검출용 라플라시안 필터
float g_fLaplacianMask[9] =
{
    -1, -1, -1,
    -1,  8, -1,
    -1, -1, -1
};
float g_fCoord[3] = { -1.f, 0.f, 1.f };

/*블러*/
const float g_fWeight[17] = 
    { 0.0561, 0.1353, 0.278, 0.4868, 0.6534, 0.7261, 0.8253, 0.9231, 1,
    0.9231, 0.8253, 0.7261, 0.6534, 0.4868, 0.278, 0.1353, 0.0561 };


float g_fWeightTotal = 9.1682;



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

PS_OUT PS_TEST(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    Out.vColor = g_BlendTexture.Sample(g_LinearSampler, In.vTexcoord);
    
    return Out;


}


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

    vector vBackBufferCol = g_BackBufferTexture.Sample(g_PointSampler, In.vTexcoord);
    vector vBlendCol = g_BlendTexture.Sample(g_LinearSampler, In.vTexcoord);
    
    Out.vColor = vBackBufferCol + vBlendCol;
    
	return Out;
}


PS_OUT PS_ZOOMBLUR(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float2 vFocusPosition = float2(g_fFocusCenter.x / 2, g_fFocusCenter.y / 2);
    vFocusPosition.x /= g_fScreenWidth;
    vFocusPosition.y /= g_fScreenHeight;
    
    float2 vFocus = In.vTexcoord - vFocusPosition;
    
    for (uint i = 0; i < g_iFocusDetail; ++i)
    {
        float fScale = 1.f - g_fFocusPow * (1.f / g_fScreenWidth) * float(i);
        Out.vColor.rgb += g_BackBufferTexture.Sample(g_LinearSampler, fScale * vFocus + vFocusPosition).rgb;
        
    }
    
    Out.vColor.rgb *= 1.0 / float(g_iFocusDetail);
    
    return Out;


}

PS_OUT PS_BLUR_HORIZONTAL(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float fPixelSize = 1.0 / g_fScreenWidth;
    
    float2 UVPos = 0;
    
    for (int i = -8; i < 8; ++i)
    {
        UVPos = In.vTexcoord + float2(fPixelSize * i, 0);
        Out.vColor += g_fWeight[8 + i] * g_GlowTexture.Sample(g_LinearSampler, UVPos);   
        
        
    }
    
    Out.vColor /= g_fWeightTotal;
    
//    Out.vColor = (dot(Out.vColor.rgb, float3(0.2125f, 0.7154f, 0.0721f)) + 0.0001f, 1.f);

    
    return Out;

}

PS_OUT PS_BLUR_VERTICAL(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float fPixelSize = 1.0 / (g_fScreenHeight / 2.0);
    float2 UVPos = 0;
    
    for (int i = -8; i < 8; ++i)
    {
        UVPos = In.vTexcoord + float2(fPixelSize * i, 0);
        Out.vColor += g_fWeight[8 + i] * g_GlowTexture.Sample(g_LinearSampler, UVPos);
    }
    
    Out.vColor /= g_fWeightTotal;
    
 //   Out.vColor = (dot(Out.vColor.rgb, float3(0.2125f, 0.7154f, 0.0721f)) + 0.0001f, 1.f);

    return Out;


}

PS_OUT PS_OUTLINE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
  //  Out.vColor = g_BackBufferTexture.Sample(g_LinearSampler, In.vTexcoord);

    for (uint i = 0; i < 9; ++i)
    {
        Out.vColor += g_fLaplacianMask[i] *
        g_BackBufferTexture.Sample(g_LinearSampler, In.vTexcoord + float2((g_fCoord[i % 3] / g_fScreenWidth), (g_fCoord[i % 3] / g_fScreenHeight)));
    }
    
    float fBlack = 1 - (Out.vColor.r * 0.3f + Out.vColor.g * 0.59f + Out.vColor.b * 0.11f);
    Out.vColor = vector(fBlack, fBlack, fBlack, 1);
    
    return Out;
}

PS_OUT PS_DISTORTION(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    vector vDistortionColor = g_DistortionTex.Sample(g_LinearSampler, In.vTexcoord);
    vector vBackBuffer = g_BackBufferTexture.Sample(g_LinearSampler, In.vTexcoord);
   

    float2 vTransUV = In.vTexcoord + g_fDistortionTime * g_fDistortionSpeed;

    vector vNoiseTex = g_NoiseTexture.Sample(g_LinearSampler, vTransUV);
    float2 vUV = In.vTexcoord + vNoiseTex.r * 0.05f;
    
     float2 fWeight = lerp(In.vTexcoord, vUV, 0.5f);
    vector vDistorBuffer;
    
    if (vDistortionColor.r > 0.f)
    {
        vDistorBuffer = g_BackBufferTexture.Sample(g_LinearSampler, fWeight);
 
    }
    else
    {
        vDistorBuffer = g_BackBufferTexture.Sample(g_LinearSampler, In.vTexcoord);

    }
    
    //vBackBuffer + 
    Out.vColor = vDistorBuffer;
    
    return Out;
}

PS_OUT PS_BLEND_OUTLINE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    vector vBackBuffer = g_BackBufferTexture.Sample(g_LinearSampler, In.vTexcoord);
    vector vOutLine = g_OutLineTexture.Sample(g_LinearSampler, In.vTexcoord);
    
    Out.vColor =  vBackBuffer * vOutLine;
    
    return Out;
}


struct PS_GLOW_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float2 texCoord1 : TEXCOORD1;
    float2 texCoord2 : TEXCOORD2;
    float2 texCoord3 : TEXCOORD3;
    float2 texCoord4 : TEXCOORD4;
    float2 texCoord5 : TEXCOORD5;
    float2 texCoord6 : TEXCOORD6;
    float2 texCoord7 : TEXCOORD7;
    float2 texCoord8 : TEXCOORD8;
    float2 texCoord9 : TEXCOORD9;
};

struct PS_GLOW_OUT
{
    vector vColor : SV_TARGET0;
};

PS_GLOW_OUT PS_GLOW(PS_GLOW_IN In)
{
    PS_GLOW_OUT Out = (PS_GLOW_OUT) 0;
    
    
    return Out;
   
}


technique11	DefaultTechnique
{
	pass Debug //0
	{
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None_ZTestAndWrite, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

    pass Glow_Horizontal //1
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None_ZTestAndWrite, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_BLUR_HORIZONTAL();
    }


    pass Glow_Vertical //2
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None_ZTestAndWrite, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_BLUR_VERTICAL();
    }

    pass OutLine //3
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None_ZTestAndWrite, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_OUTLINE();
    }

    pass Test //4
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_TEST();
    }

    pass OutLine_Blend //5
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None_ZTestAndWrite, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_BLEND_OUTLINE();
    }

    pass Distortion //6
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None_ZTestAndWrite, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_DISTORTION();
    }

    pass ZoomBlur //7
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None_ZTestAndWrite, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_ZOOMBLUR();
    }

}


