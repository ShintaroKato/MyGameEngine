#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0);   // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);        // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv);

    // UV���炵
    //texcolor = tex.Sample(smp, input.uv * float2(2,2) + float2(0.5f,0.5f) );

    // �ʏ�
    //texcolor = float4(texcolor.rgb, 1);

    // RGB�Q�{
    texcolor = float4(texcolor.rgb * 2.0f, 1);

    // �F���]
    //texcolor = float4(float3(1,1,1) - texcolor.rgb, 1);

    return texcolor;
}
