#include "Shader_Defines.hlsli"

/*
���ؽ� ���̴�(���� ���̴�, VS) : ���� ������ �������� ������ �Ͽ� Ư���� ȿ���� �ִµ� ����ϴ� ���̴�
�ȼ� ���̴�(PS): ������ �� ������ �ȼ��� ���� ����ϴ� ����� ���̴�
������Ʈ�� ���̴�: ���ؽ� ���̴��� ����� ���� �����. ������ ���� Ȥ�� �� �߰� �� �� ����. 

[����]

���ؽ� ���̴� > ������Ʈ�� ���̴� > �����Ͷ����� > �ȼ� ���̴�

*/

//���������� Client����, Ȥ�� �ʱⰪ�� �־��ָ� �������ش�
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_Texture;

vector g_RGBColor = vector(1.f, 1.f, 1.f, 1.f);

//���� ���� �� �ۼ��ߴ� �Ͱ� ������ �ڷ���, ������ �ø�ƽ �̸��� ����ؾ���
struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0; //texcoord�� ��ȯ �� ���� ������ �ʴ´�
    
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION; //matrix�� �����߱� ������ output�� float4�� ���´�
    float2 vTexcoord : TEXCOORD0;

};

//_MAIN : ������ �Լ� , �Ϲ� �Լ��� �����ϰ� [�������][�Լ���][�Է�����]
// mul �Լ��� transformcoord�� �޸� ���� ���� w ������ ������ ���������� �ʴ´�
// ���� �������� w ������ ������ ���� float4 < format�� �����ؾ���
VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
    
    
    matrix matWV, matWVP; // World * View , World * View * Projection
    //������ �� ��ĵ鳢�� �̸� ������, �� ������� ������ �����ǿ� ���Ѵ�
    
    /* [��� 1]
    
    // mul : �������� ����� ��� ���Ϸ��� ���� ���� ������ ���ƾ� ���డ��
    //vector(In.vPosition, 1.f) : float3�� vPosition�� w = 1.f�� �߰��� �� ���� 
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
  
    //y�������� �Ʒ��� �������� alpha���� �����ϴ� ���� �ڵ�
    //max(a,b)�� a�� b���� ���� ��� b�� ���� ������ش�
  
    
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
    
    //y�������� �Ʒ��� �������� alpha���� �����ϴ� ���� �ڵ�
    //max(a,b)�� a�� b���� ���� ��� b�� ���� ������ش�
  
    
    return Out;

}

PS_OUT PS_HPBar(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture.Sample(g_LinearSampler, In.vTexcoord);
    
    //���� ��
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
    //pass�� �������� ������ �� ���̴� ���� �ȿ� ���� ������, �ٸ� ȿ���� �� ���� ����
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