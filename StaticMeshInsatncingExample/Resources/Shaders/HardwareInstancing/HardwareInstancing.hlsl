
#define PI  3.141592653589

struct MaterialData
{
    int diffuseTextureIndex;
    float padding0;
    float padding1;
    float padding2;
};

struct InstanceData
{
    float4x4 matWorld;
    int materialIndex;
    float padding0;
    float padding1;
    float padding2;
};

cbuffer GlobalConstants : register(b0)
{
    float4x4 matViewProj;
    float3 lightDir;
    float padding0;
    float4 ambient;
};

SamplerState samplerLinearWrap        : register(s0);

Texture2D    textures[] : register(t0, space0);

StructuredBuffer<InstanceData> instanceDatas : register(t0, space1);
StructuredBuffer<MaterialData> materialDatas : register(t1, space1);

struct VS_IN
{
    float3 position : POSITION;
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;
    float4 color    : COLOR;
    float2 uv       : TEXCOORD;
};

struct VS_OUT
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float materialID : TEXCOORD1;
};

VS_OUT VS(VS_IN vsIn, uint instanceID : SV_InstanceID)
{
    VS_OUT vsOut;
    InstanceData instData = instanceDatas[instanceID];

    float4 worldPos = mul(float4(vsIn.position, 1.0f), instData.matWorld);
    vsOut.position = mul(worldPos, matViewProj);
    vsOut.normal = mul(vsIn.normal, (float3x3)instData.matWorld);
    vsOut.uv = vsIn.uv;
    vsOut.materialID = instData.materialIndex;

    return vsOut;
}

float4 PS(VS_OUT psIn) : SV_Target
{
    int texIndex = materialDatas[round(psIn.materialID)].diffuseTextureIndex;
    float4 diffuse = textures[texIndex].Sample(samplerLinearWrap, psIn.uv);
    float3 res = dot(psIn.normal, -lightDir) * diffuse.xyz;

    return float4(res + diffuse.xyz * ambient.xyz, 1.0f);
}