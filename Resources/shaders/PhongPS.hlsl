#include "FBX.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float3 eyePos = camPos;
    float3 normal = input.normal;
    float3 light = float3(10, 10, 1);

    // �A���r�G���g
    float4 constColor = tex.Sample(smp, input.uv);
    // �Â߂ɂ��邽�߂�0.3���|����
    float4 ambient = constColor * 0.3f;
    // �����x��1�ɖ߂���
    ambient.a = 1;

    // �f�B�t���[�Y
    // �����x�N�g���Ɩ@���x�N�g���̓��ς��Z�o���A0.0�`1.0�̊ԂŃN�����v
    float intensity = saturate(dot(normalize(normal), light));
    float4 diffuse = constColor * 0.5f * intensity;

    // �X�y�L�����[
    // �����x�N�g���𐳋K��
    float3 eyeDir = normalize(eyePos - input.svpos);
    // �����x�N�g���Ǝ����x�N�g���̐��K�����A�@���x�N�g���Ƃ̓��ς��Z�o
    // �����0.0�`1.0�̊ԂŃN�����v
    intensity = saturate(dot(normalize(normal), normalize(light + eyeDir)));

    float4 specular = float4(1, 1, 1, 1) * pow(intensity, 30);

    // �A���r�G���g�A�f�B�t���[�Y�A�X�y�L�����[�����Z
    float4 ads = ambient + diffuse + specular;

    return ads;
}