#include "Obj.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

float4 PhongShading(VSOutput input) : SV_TARGET;

float4 main(VSOutput input) : SV_TARGET
{
    float4 result;

    switch (mode)
    {
    case 0:
        float3 light = normalize(float3(1, -1, 1)); // 右下奥　向きのライト
        float light_diffuse = saturate(dot(-light, input.normal));
        float3 shade_color;
        shade_color = m_ambient; // アンビエント項
        shade_color += m_diffuse * light_diffuse;	// ディフューズ項
        float4 texcolor = tex.Sample(smp, input.uv);

        result = float4(texcolor.rgb * shade_color * constColor.rgb, texcolor.a * m_alpha);

        break;

    case 1:
        result = PhongShading(input);

        break;
    }

    return result;
}

float4 PhongShading(VSOutput input) : SV_TARGET
{
    float3 eyePos = camPos;
    float3 normal = input.normal;
    float3 light = float3(10, 10, 1);

    // アンビエント
    float4 constColor = tex.Sample(smp, input.uv);
    // 暗めにするために0.3を掛けた
    float4 ambient = constColor * 0.3f;
    // 透明度を1に戻した
    ambient.a = 1;

    // ディフューズ
    // 光源ベクトルと法線ベクトルの内積を算出し、0.0～1.0の間でクランプ
    float intensity = saturate(dot(normalize(normal), light));
    float4 diffuse = constColor * 0.5f * intensity;

    // スペキュラー
    // 視線ベクトルを正規化
    float3 eyeDir = normalize(eyePos - input.svpos);
    // 光源ベクトルと視線ベクトルの正規化し、法線ベクトルとの内積を算出
    // これを0.0～1.0の間でクランプ
    intensity = saturate(dot(normalize(normal), normalize(light + eyeDir)));

    float4 specular = float4(1, 1, 1, 1) * pow(intensity, 30);

    // アンビエント、ディフューズ、スペキュラーを加算
    float4 ads = ambient + diffuse + specular;

    return ads;
}