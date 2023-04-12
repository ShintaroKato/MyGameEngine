#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0);   // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);        // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 Blur(VSOutput input, float2 sigma = 5) : SV_TARGET;

float4 DepthOfField(VSOutput input) : SV_TARGET;

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv);

    if (mode == 0)
    {
        // �ʏ�
        texcolor = float4(texcolor.rgb, alpha);
    }
    if (mode == 1)
    {
        // RGB�Q�{
        texcolor = float4(texcolor.rgb * 2.0f, alpha);
    }
    if (mode == 2)
    {
        // �F���]
        texcolor = float4(float3(1, 1, 1) - texcolor.rgb, alpha);
    }
    if (mode == 3)
    {
        // �ڂ���
        texcolor = Blur(input, float2(5, 5));
    }
    if (mode == 4)
    {
        texcolor = float4(texcolor.rgb, alpha);
    }
    if (mode == 5)
    {
        // UV���炵�A�^�C�����O
        texcolor = tex.Sample(smp, input.uv * float2(2, 2) + float2(0.5f, 0.5f));
        texcolor = float4(texcolor.rgb, alpha);
    }
    if (mode == 6)
    {
        // ��ʊE�[�x(��肩��)
        texcolor = DepthOfField(input);
        texcolor = float4(texcolor.rgb, alpha);
    }

    return texcolor;
}

float4 Blur(VSOutput input, float2 sigma) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv);

    float4 countColor = float4(0,0,0,0); // ���v�l�ۑ��p
    float count = 0; // �T���v�����O�����s�N�Z���̐����J�E���g

    for (float y = -sigma.y; y < sigma.y; y++)
    {
        for (float x = -sigma.x; x < sigma.x; x++)
        {
            //�s�N�Z���̐F���T���v�����O���č��v�ɉ��Z
            countColor += tex.Sample(smp, input.uv + float2(x, y) * 0.001f);
            count++;
        }
    }

    // ���ϒl���Z�o
    texcolor = float4(countColor.rgb / count, alpha);

    return texcolor;
}

float4 DepthOfField(VSOutput input) : SV_TARGET
{
    float _Depth = 0.05;
    float _Width = 0.05;

    float4 pintColor = tex.Sample(smp, input.uv);
    float4 overColor = Blur(input);
    float pint = smoothstep(0, _Width / 2, _Depth);

    return pint * pintColor + (1 - pint) * overColor;
}