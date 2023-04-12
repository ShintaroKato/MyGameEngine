#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0);   // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);        // 0番スロットに設定されたサンプラー

float4 Blur(VSOutput input, float2 sigma = 5) : SV_TARGET;

float4 DepthOfField(VSOutput input) : SV_TARGET;

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv);

    if (mode == 0)
    {
        // 通常
        texcolor = float4(texcolor.rgb, alpha);
    }
    if (mode == 1)
    {
        // RGB２倍
        texcolor = float4(texcolor.rgb * 2.0f, alpha);
    }
    if (mode == 2)
    {
        // 色反転
        texcolor = float4(float3(1, 1, 1) - texcolor.rgb, alpha);
    }
    if (mode == 3)
    {
        // ぼかし
        texcolor = Blur(input, float2(5, 5));
    }
    if (mode == 4)
    {
        texcolor = float4(texcolor.rgb, alpha);
    }
    if (mode == 5)
    {
        // UVずらし、タイリング
        texcolor = tex.Sample(smp, input.uv * float2(2, 2) + float2(0.5f, 0.5f));
        texcolor = float4(texcolor.rgb, alpha);
    }
    if (mode == 6)
    {
        // 被写界深度(作りかけ)
        texcolor = DepthOfField(input);
        texcolor = float4(texcolor.rgb, alpha);
    }

    return texcolor;
}

float4 Blur(VSOutput input, float2 sigma) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv);

    float4 countColor = float4(0,0,0,0); // 合計値保存用
    float count = 0; // サンプリングしたピクセルの数をカウント

    for (float y = -sigma.y; y < sigma.y; y++)
    {
        for (float x = -sigma.x; x < sigma.x; x++)
        {
            //ピクセルの色をサンプリングして合計に加算
            countColor += tex.Sample(smp, input.uv + float2(x, y) * 0.001f);
            count++;
        }
    }

    // 平均値を算出
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