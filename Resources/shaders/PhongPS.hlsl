#include "FBX.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float3 eyePos = camPos;
    float3 normal = input.normal;
    float3 light = float3(10, 10, 1);

    float intensity = saturate(dot(normalize(normal), light));

    // アンビエント
    float4 color = tex.Sample(smp, input.uv);
    float4 ambient = color * 0.3f;
    ambient.a = 1;

    // ディフューズ
    float4 diffuse = color * 0.5f * intensity;

    // スペキュラー
    float3 eyeDir = normalize(eyePos - input.svpos);
    intensity = saturate(dot(normalize(normal), normalize(light + eyeDir)));

    float4 specular = float4(1, 1, 1, 1) * pow(intensity, 30);

    float4 ads = ambient + diffuse + specular;

    return ads;
}