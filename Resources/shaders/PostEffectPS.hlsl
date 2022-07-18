#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0);   // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);        // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv);

    // UVずらし
    //texcolor = tex.Sample(smp, input.uv * float2(2,2) + float2(0.5f,0.5f) );

    // 通常
    //texcolor = float4(texcolor.rgb, 1);

    // RGB２倍
    texcolor = float4(texcolor.rgb * 2.0f, 1);

    // 色反転
    //texcolor = float4(float3(1,1,1) - texcolor.rgb, 1);

    return texcolor;
}
