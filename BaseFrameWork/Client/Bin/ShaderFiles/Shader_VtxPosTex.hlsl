#include "Shader_Defines.hlsli"

/*
버텍스 쉐이더(정점 쉐이더, VS) : 정점 정보에 수학적인 연산을 하여 특별한 효과를 주는데 사용하는 쉐이더
픽셀 쉐이더(PS): 렌더링 될 각각의 픽셀의 색을 계산하는 방식의 쉐이더
지오메트리 쉐이더: 버텍스 쉐이더가 수행된 이후 수행됨. 정점을 삭제 혹은 더 추가 할 수 있음. 

[순서]

버텍스 쉐이더 > 지오메트리 쉐이더 > 레스터라이즈 > 픽셀 쉐이더

*/

//전역변수는 Client에서, 혹은 초기값을 넣어주며 세팅해준다
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_Texture;

vector g_RGBColor = vector(1.f, 1.f, 1.f, 1.f);

//버퍼 생성 때 작성했던 것과 동일한 자료형, 동일한 시맨틱 이름을 사용해야함
struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0; //texcoord는 변환 중 값이 변하지 않는다
    
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION; //matrix와 연산했기 때문에 output은 float4로 나온다
    float2 vTexcoord : TEXCOORD0;

};

//_MAIN : 진입점 함수 , 일반 함수와 동일하게 [출력인자][함수명][입력인자]
// mul 함수는 transformcoord와 달리 곱한 이후 w 나누기 연산을 수행해주지 않는다
// 따라서 마지막에 w 나누기 연산을 위해 float4 < format을 유지해야함
VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
    
    
    matrix matWV, matWVP; // World * View , World * View * Projection
    //보통은 이 행렬들끼리 미리 곱한후, 이 결과값을 들어오는 포지션에 곱한다
    
    /* [방법 1]
    
    // mul : 곱해지는 행렬의 행과 곱하려는 행의 열의 갯수가 같아야 수행가능
    //vector(In.vPosition, 1.f) : float3인 vPosition에 w = 1.f를 추가한 후 연산 
    float4 vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    vPosition = mul(vPosition, g_ViewMatrix);
    vPosition = mul(vPosition, g_ProjMatrix);
    */
    
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
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture.Sample(g_LinearSampler, In.vTexcoord);    
    
    if (Out.vColor.a  < 0.4f)
        discard;   
  
    //y포지션이 아래에 있을수록 alpha값을 낮게하는 예시 코드
    //max(a,b)면 a가 b보다 작을 경우 b로 값을 대신해준다
  
    
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

PS_OUT PS_HPBar(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture.Sample(g_LinearSampler, In.vTexcoord);
    
    //원래 색
    if (In.vTexcoord.r < g_HPRatio)
    {
        Out.vColor.rgb = float3(0.f, 1.f, 0.8f);
    }
    else
    {
        Out.vColor.rgb = float3(0.25f, 0.25f, 0.25f);
    }
    
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
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass PointPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_POINT();
    }

    pass HPBar //2 
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_HPBar();
    }

}