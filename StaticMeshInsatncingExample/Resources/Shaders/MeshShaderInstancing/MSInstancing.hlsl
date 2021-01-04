#define MAX_VERTS 64
#define MAX_PRIMS 126
#define INST_GROUP_SIZE 64

struct DrawParam
{
    uint instanceCountPerDispatch;
    uint instanceOffset;
    uint meshletCount;
    uint lastMeshletVertCount;
    uint lastMeshletPrimCount;
};

struct Vertex
{
    float3 position;
    float3 normal;
    float3 tangent;
    float4 color;
    float2 uv;
};

struct VertexOut
{
    float4 position      : SV_POSITION;
    float3 normal        : NORMAL0;
    float3 tangent       : TANGENT;
    float4 color         : COLOR;
    float2 uv            : TEXCOORD0;
    uint   materialID    : TEXCOORD1;
};

struct Meshlet
{
    uint vertexCount;
    uint vertexOffset;
    uint primitiveCount;
    uint primitiveOffset;
};

struct InstanceData
{
    float4x4 matWorld;
    uint materialID;
    float padding0;
    float padding1;
    float padding2;
};

struct MaterialData
{
    uint diffuseTextureIndex;
    float padding0;
    float padding1;
    float padding2;
};

struct GlobalConstants
{
    float4x4 matViewProj;
    float3 lightDir;
    float padding0;
    float4 globalAmbient;
    float3 camPos;
    float padding1;
};

struct Payload
{
    uint instanceCount;
    uint instanceOffset;
};

groupshared Payload payload;

ConstantBuffer<GlobalConstants>  globalConstants  : register(b0);
ConstantBuffer<DrawParam>        drawParam        : register(b1);

StructuredBuffer<Vertex>         vertices             : register(t0, space1);
StructuredBuffer<Meshlet>        meshlets             : register(t1, space1);
StructuredBuffer<uint>           localVertexIndices   : register(t2, space1);
StructuredBuffer<uint>           localPrimIndices     : register(t3, space1);
StructuredBuffer<InstanceData>   instanceDatas        : register(t4, space1);
StructuredBuffer<MaterialData>   materialDatas        : register(t5, space1);

SamplerState samplerLinearWrap : register(s0);

Texture2D textures[] : register(t0, space0);

uint DivRoundUp(uint num, uint denom)
{
    return (num + denom - 1) / denom;
}

[NumThreads(1, 1, 1)]
void AS(uint dtid : SV_DISPATCHTHREADID, uint gtid : SV_GROUPTHREADID, uint gid : SV_GROUPID)
{
    uint instanceCount = min(drawParam.instanceCountPerDispatch - (dtid * INST_GROUP_SIZE), INST_GROUP_SIZE);
    uint unpackedGroupCount = (drawParam.meshletCount - 1) * instanceCount;
    uint packCount = min(MAX_VERTS / drawParam.lastMeshletVertCount, MAX_PRIMS / drawParam.lastMeshletPrimCount);
    uint packedGroupCount = DivRoundUp(instanceCount, packCount);
    uint dispatchCount = unpackedGroupCount + packedGroupCount;

    payload.instanceOffset = gid * instanceCount + drawParam.instanceOffset;
    payload.instanceCount = instanceCount;

    DispatchMesh(dispatchCount, 1, 1, payload);
}

uint3 UnpackPrimitive(uint primitive)
{
    return uint3(primitive & 0x3FF, (primitive >> 10) & 0x3FF, (primitive >> 20) & 0x3FF);
}

uint3 GetPrimitive(Meshlet meshlet, uint index)
{
    return UnpackPrimitive(localPrimIndices[meshlet.primitiveOffset + index]);
}

uint GetVertexIndex(Meshlet meshlet, uint localIndex)
{
    localIndex = meshlet.vertexOffset + localIndex;
    return localVertexIndices[localIndex];
}

VertexOut GetVertexAttributes(uint vertexIndex, uint instanceIndex)
{
    InstanceData instData = instanceDatas[instanceIndex];
    Vertex v = vertices[vertexIndex];

    VertexOut outVert;
    float4 worldPosition    = mul(float4(v.position, 1.0f), instData.matWorld);
    outVert.normal          = mul(v.normal, (float3x3)instData.matWorld);
    outVert.tangent         = mul(v.tangent, (float3x3)instData.matWorld);
    outVert.position        = mul(worldPosition, globalConstants.matViewProj);
    outVert.color           = v.color;
    outVert.uv              = v.uv;
    outVert.materialID      = instData.materialID;

    return outVert;
}

[NumThreads(128, 1, 1)]
[OutputTopology("triangle")]
void MS(uint gtid : SV_GROUPTHREADID, 
        uint gid : SV_GROUPID, 
        in payload Payload payload,
        out indices uint3 tris[MAX_PRIMS],
        out vertices VertexOut verts[MAX_VERTS])
{
    uint meshletIndex = gid / payload.instanceCount;
    Meshlet meshlet = meshlets[meshletIndex];

    uint startInstance = gid % payload.instanceCount;
    uint instanceCount = 1;

    if(meshletIndex == drawParam.meshletCount - 1)
    {
        const int instanceDataPerGroups = min(MAX_VERTS / meshlet.vertexCount, MAX_PRIMS / meshlet.primitiveCount);
        uint unpackedGroupCount = (drawParam.meshletCount - 1) * payload.instanceCount;
        uint packedIndex = gid - unpackedGroupCount;

        startInstance = packedIndex * instanceDataPerGroups;
        instanceCount = min(payload.instanceCount - startInstance, instanceDataPerGroups);
    }

    uint vertCount = meshlet.vertexCount * instanceCount;
    uint primCount = meshlet.primitiveCount * instanceCount;

    SetMeshOutputCounts(vertCount, primCount);

    if(gtid < vertCount)
    {
        uint readIndex = gtid % meshlet.vertexCount;
        uint instanceId = gtid / meshlet.vertexCount;

        uint vertexIndex = GetVertexIndex(meshlet, readIndex);
        uint instanceIndex = startInstance + instanceId + payload.instanceOffset;
        verts[gtid] = GetVertexAttributes(vertexIndex, instanceIndex);
    }
    
    if(gtid < primCount)
    {
        uint readIndex = gtid % meshlet.primitiveCount;
        uint instanceId = gtid / meshlet.primitiveCount;

        tris[gtid] = GetPrimitive(meshlet, readIndex) + (meshlet.vertexCount * instanceId);
    }
}

float4 PS(VertexOut psIn) : SV_Target
{
    int texIndex = materialDatas[round(psIn.materialID)].diffuseTextureIndex;
    float4 diffuse = textures[texIndex].Sample(samplerLinearWrap, psIn.uv);
    float nDotL = dot(psIn.normal, -globalConstants.lightDir);
    return float4(diffuse.xyz * nDotL + diffuse.xyz * globalConstants.globalAmbient.xyz, 1.0f);
}