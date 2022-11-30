#include "RenderTexture.hlsli"

Texture2D<float4> tex0 : register(t0);   // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex1 : register(t1);   // 1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);        // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor0 = tex0.Sample(smp, input.uv);
    float4 texcolor1 = tex1.Sample(smp, input.uv);

    float4 constColor = texcolor0;
    if (fmod(input.uv.y, 0.1f) < 0.05f)
    {
        constColor = texcolor1;
    }

    // UV���炵
    //texcolor = tex.Sample(smp, input.uv * float2(2,2) + float2(0.5f,0.5f) );

    // �ʏ�
    //texcolor = float4(texcolor.rgb, 1);

    // RGB�Q�{
    //texcolor = float4(texcolor.rgb * 2.0f, 1);

    // �F���]
    //color = float4(float3(1,1,1) - color.rgb, 1);

    return constColor;
}
