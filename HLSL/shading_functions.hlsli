//--------------------------------------------
//	�����o�[�g�g�U���ˌv�Z�֐�
//--------------------------------------------
// N:�@��(���K���ς�)
// L:���˃x�N�g��(���K���ς�)
// C:���ˌ�(�F�E����)
// K:���˗�
float3 CalcLambert(float3 N,float3 L,float3 C,float3 K)
{
    float power = saturate(dot(N, -L));
    return C * power * K;
}

//--------------------------------------------
//	�t�H���̋��ʔ��ˌv�Z�֐�
//--------------------------------------------
// N:�@��(���K���ς�)
// L:���˃x�N�g��(���K���ς�)
// E:�����x�N�g��(���K���ς�)
// C:���ˌ�(�F�E����)
// K:���˗�
float3 CalcPhongSpecular(float3 N, float3 L, float3 E, float3 C, float3 K)
{
    float3 R = reflect(L, N);
    float power = max(dot(-E, R), 0);
    power = pow(power, 128);
    return C * power * K;
}

//--------------------------------------------
//	�n�[�t�����o�[�g�g�U���ˌv�Z�֐�
//--------------------------------------------
// N:�@��(���K���ς�)
// L:���˃x�N�g��(���K���ς�)
// C:���ˌ�(�F�E����)
// K:���˗�
float3 CalcHarfLambert(float3 N, float3 L, float3 C, float3 K)
{
    float D = saturate(dot(N, -L) * 0.5f + 0.5f);
    return C * D * K;
}

//--------------------------------------------
// �������C�g
//--------------------------------------------
// N:�@��(���K���ς�)
// E:���_�����x�N�g��(���K���ς�)
// L:���˃x�N�g��(���K���ς�)
// C :���C�g�F
// RimPower : �������C�g�̋���(�����l�̓e�L�g�[�Ȃ̂Ŏ����Őݒ肷�邪�g)
float3 CalcRimLight(float3 N,float3 E, float3 L, float3 C, float RimPower = 3.0f)
{
    float rim = 1.0f - saturate(dot(N, -E));
    return C * pow(rim, RimPower) * saturate(dot(L, -E));
}

//--------------------------------------------
// �����v�V�F�[�f�B���O
//--------------------------------------------
// tex:�����v�V�F�[�f�B���O�p�e�N�X�`��
// samp:�����v�V�F�[�f�B���O�p�T���v���X�e�[�g
// N:�@��(���K���ς�)
// L:���˃x�N�g��(���K���ς�)
// C:���ˌ�(�F�E����)
// K:���˗�
float3 CalcRampShading(Texture2D tex,SamplerState samp, float3 N,float3 L, float3 C, float3 K)
{
    float D = saturate(dot(N, -L) * 0.5f + 0.5f);
    float Ramp = tex.Sample(samp, float2(D, 0.5f)).r;
    return C * Ramp * K.rgb;
}

//--------------------------------------------
// ���̊��}�b�s���O
//--------------------------------------------
// tex:�����v�V�F�[�f�B���O�p�e�N�X�`��
// samp:�����v�V�F�[�f�B���O�p�T���v���X�e�[�g
// color: ���݂̃s�N�Z���F
// N:�@��(���K���ς�)
// C:���ˌ�(�F�E����)
// value:�K����
float3 CalcSphereEnvironment(Texture2D tex, SamplerState samp, in float3 color, float3 N, float3 E, float
value)
{
    float3 R = reflect(E, N);
    float2 texcoord = R.xy * 0.5f + 0.5f;
    return lerp(color.rgb, tex.Sample(samp, texcoord).rgb, value);
}

//--------------------------------------------
// �������C�e�B���O
//--------------------------------------------
// normal:�@��(���K���ς�)
// up:������i�Е��j
// sky_color:��(��)�F
// ground_color:�n��(��)�F
// hemisphere_weight:�d��
float3 CalcHemiSphereLight(float3 normal, float3 up, float3 sky_color, float3 ground_color, float4 hemisphere_weight)
{
    float factor = dot(normal, up) * 0.5f + 0.5f;
    return lerp(ground_color, sky_color, factor) * hemisphere_weight.x;
}

//--------------------------------------------
//	�t�H�O
//--------------------------------------------
//color:���݂̃s�N�Z���F
//fog_color:�t�H�O�̐F
//fog_range:�t�H�O�͈̔͏��
//eye_length:���_����̋���
float4 CalcFog(in float4 color, float4 fog_color, float2 fog_range, float eye_length)
{
    float fogAlpha = saturate((eye_length - fog_range.x) / (fog_range.y - fog_range.x));
    return lerp(color, fog_color, fogAlpha);
}