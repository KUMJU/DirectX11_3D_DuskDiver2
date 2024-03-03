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
float g_fDeltaTime; 
float g_fStrength;
float g_fDistortionSpeed;

/*글로우*/
texture2D g_GlowTexture;

float g_fScreenWidth;
float g_fScreenHeight;

//외곽선 검출용 라플라시안 필터
float g_fLaplacianMask[9] =
{
    -1, -1, -1,
    -1,  8, -1,
    -1, -1, -1
};
float g_fCoord[3] = { -1.f, 0.f, 1.f };

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

struct VS_GLOW_OUT
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

VS_GLOW_OUT VS_GLOW_HORIZONTAL(VS_IN In)
{
    VS_GLOW_OUT Out = (VS_GLOW_OUT) 0;
    
    vector vPos = vector(In.vPosition, 1.f);
    
    //matrix WV = g_WorldMatrix * g_ViewMatrix;
    //matrix WVP = WV * g_ProjMatrix;
    
    Out.vPosition = vPos;
   // Out.vPosition = mul(vPos, WVP);
    Out.vTexcoord = In.vTexcoord;
    
    float texelSize = 1.f / g_fScreenWidth; 
    
    Out.texCoord1 = In.vTexcoord + float2(texelSize * -4.f, 0.f);
    Out.texCoord2 = In.vTexcoord + float2(texelSize * -3.f, 0.f);
    Out.texCoord3 = In.vTexcoord + float2(texelSize * -2.f, 0.f);
    Out.texCoord4 = In.vTexcoord + float2(texelSize * -1.f, 0.f);
    Out.texCoord5 = In.vTexcoord + float2(texelSize * -0.f, 0.f);
    Out.texCoord6 = In.vTexcoord + float2(texelSize * 1.f, 0.f);
    Out.texCoord7 = In.vTexcoord + float2(texelSize * 2.f, 0.f);
    Out.texCoord8 = In.vTexcoord + float2(texelSize * 3.f, 0.f);
    Out.texCoord9 = In.vTexcoord + float2(texelSize * 4.f, 0.f);
    
    return Out;
}

VS_GLOW_OUT VS_GLOW_VERTICAL(VS_IN In)
{
    VS_GLOW_OUT Out = (VS_GLOW_OUT) 0;
   
    vector vPos = vector(In.vPosition, 1.f);
    
    Out.vPosition = vPos;
    Out.vTexcoord = In.vTexcoord;
    
    float texelSize = 1.f / g_fScreenHeight;
    
    Out.texCoord1 = In.vTexcoord + float2(0.f, texelSize * -4.f);
    Out.texCoord2 = In.vTexcoord + float2(0.f, texelSize * -3.f);
    Out.texCoord3 = In.vTexcoord + float2(0.f, texelSize * -2.f);
    Out.texCoord4 = In.vTexcoord + float2(0.f, texelSize * -1.f);
    Out.texCoord5 = In.vTexcoord + float2(0.f, texelSize * -0.f);
    Out.texCoord6 = In.vTexcoord + float2(0.f, texelSize * 1.f);
    Out.texCoord7 = In.vTexcoord + float2(0.f, texelSize * 2.f);
    Out.texCoord8 = In.vTexcoord + float2(0.f, texelSize * 3.f);
    Out.texCoord9 = In.vTexcoord + float2(0.f, texelSize * 4.f);
    
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
    
    float2 vTrans = In.vTexcoord + g_fDeltaTime;
    vector vNoiseTex = g_DistortionTex.Sample(g_LinearSampler, vTrans);
    float2 vUV = In.vTexcoord + vNoiseTex.xy * g_fDistortionSpeed;
    vector vBackBuffer = g_BackBufferTexture.Sample(g_LinearSampler, vUV);
    
    Out.vColor = vBackBuffer;
    
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
    
    float weight0, weight1, weight2, weight3, weight4;
    float normalization;
    float4 color;
    
    vector vGlowTex = g_GlowTexture.Sample(g_LinearSampler, In.vTexcoord);

    weight0 = 1.f;
    weight1 = 0.9f;
    weight2 = 0.55f;
    weight3 = 0.18f;
    weight4 = 0.1f;
    
    normalization = (weight0 + 2.f * (weight1 + weight2 + weight3 + weight4));
    
    weight0 = weight0 / normalization;
    weight1 = weight1 / normalization;
    weight2 = weight2 / normalization;
    weight3 = weight3 / normalization;
    weight4 = weight4 / normalization;
    
    color = float4(1.f, 0.7f, 0.f, 0.5f);
    
    color += g_Texture.Sample(g_LinearSampler, In.texCoord1) * weight4;
    color += g_Texture.Sample(g_LinearSampler, In.texCoord2) * weight3;
    color += g_Texture.Sample(g_LinearSampler, In.texCoord3) * weight2;
    color += g_Texture.Sample(g_LinearSampler, In.texCoord4) * weight1;
    color += g_Texture.Sample(g_LinearSampler, In.texCoord5) * weight0;
    color += g_Texture.Sample(g_LinearSampler, In.texCoord6) * weight1;
    color += g_Texture.Sample(g_LinearSampler, In.texCoord7) * weight2;
    color += g_Texture.Sample(g_LinearSampler, In.texCoord8) * weight3;
    color += g_Texture.Sample(g_LinearSampler, In.texCoord9) * weight4;
    
    color.a = 1.f;
    
    Out.vColor = color;
    
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
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_GLOW_HORIZONTAL();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_GLOW();
    }


    pass Glow_Vertical //2
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_GLOW_VERTICAL();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_GLOW();
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
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_DISTORTION();
    }

}


