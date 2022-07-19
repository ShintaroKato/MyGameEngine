#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0);   // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);        // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv);

    // UV���炵
    if (mode == 4)
    {
        texcolor = tex.Sample(smp, input.uv * float2(2, 2) + float2(0.5f, 0.5f));
    }
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
        float4 color = float4(0,0,0,0);
        float count = 0;

        for (float y = -10; y < 10; y++)
        {
            for (float x = -10; x < 10; x++)
            {
                color += tex.Sample(smp, input.uv + float2(x, y) * 0.001f);
                count++;
            }
        }

        texcolor.rgb = color.rgb / count;
    }

    return texcolor;
}
