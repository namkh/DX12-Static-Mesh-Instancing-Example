;
; Input signature:
;
; Name                 Index   Mask Register SysValue  Format   Used
; -------------------- ----- ------ -------- -------- ------- ------
; no parameters
;
; Output signature:
;
; Name                 Index   Mask Register SysValue  Format   Used
; -------------------- ----- ------ -------- -------- ------- ------
; SV_Position              0   xyzw        0      POS   float   xyzw
; NORMAL                   0   xyz         1     NONE   float   xyz 
; TANGENT                  0   xyz         2     NONE   float   xyz 
; COLOR                    0   xyzw        3     NONE   float   xyzw
; TEXCOORD                 0   xy          4     NONE   float   xy  
; TEXCOORD                 1   x           5     NONE    uint   x   
;
; shader hash: 2e4cd52bf324c543e6496091798c3816
;
; Pipeline Runtime Information: 
;
;
;
; Vertex Output signature:
;
; Name                 Index             InterpMode DynIdx
; -------------------- ----- ---------------------- ------
; SV_Position              0          noperspective       
; NORMAL                   0                 linear       
; TANGENT                  0                 linear       
; COLOR                    0                 linear       
; TEXCOORD                 0                 linear       
; TEXCOORD                 1        nointerpolation       
;
; Buffer Definitions:
;
; cbuffer globalConstants
; {
;
;   struct dx.alignment.legacy.globalConstants
;   {
;
;       struct dx.alignment.legacy.struct.GlobalConstants
;       {
;
;           column_major float4x4 matViewProj;        ; Offset:    0
;           float3 lightDir;                          ; Offset:   64
;           float padding0;                           ; Offset:   76
;           float4 globalAmbient;                     ; Offset:   80
;           float3 camPos;                            ; Offset:   96
;           float padding1;                           ; Offset:  108
;       
;       } globalConstants;                            ; Offset:    0
;
;   
;   } globalConstants;                                ; Offset:    0 Size:   112
;
; }
;
; cbuffer drawParam
; {
;
;   struct drawParam
;   {
;
;       struct struct.DrawParam
;       {
;
;           uint instanceOffset;                      ; Offset:    0
;           uint meshletCount;                        ; Offset:    4
;           uint lastMeshletVertCount;                ; Offset:    8
;           uint lastMeshletPrimCount;                ; Offset:   12
;       
;       } drawParam;                                  ; Offset:    0
;
;   
;   } drawParam;                                      ; Offset:    0 Size:    16
;
; }
;
; Resource bind info for vertices
; {
;
;   struct struct.Vertex
;   {
;
;       float3 position;                              ; Offset:    0
;       float3 normal;                                ; Offset:   12
;       float3 tangent;                               ; Offset:   24
;       float4 color;                                 ; Offset:   36
;       float2 uv;                                    ; Offset:   52
;   
;   } $Element;                                       ; Offset:    0 Size:    60
;
; }
;
; Resource bind info for meshlets
; {
;
;   struct struct.Meshlet
;   {
;
;       uint vertexCount;                             ; Offset:    0
;       uint vertexOffset;                            ; Offset:    4
;       uint primitiveCount;                          ; Offset:    8
;       uint primitiveOffset;                         ; Offset:   12
;   
;   } $Element;                                       ; Offset:    0 Size:    16
;
; }
;
; Resource bind info for localVertexIndices
; {
;
;   uint $Element;                                    ; Offset:    0 Size:     4
;
; }
;
; Resource bind info for localPrimIndices
; {
;
;   uint $Element;                                    ; Offset:    0 Size:     4
;
; }
;
; Resource bind info for instanceDatas
; {
;
;   struct dx.alignment.legacy.struct.InstanceData
;   {
;
;       column_major float4x4 matWorld;               ; Offset:    0
;       uint materialID;                              ; Offset:   64
;       float padding0;                               ; Offset:   68
;       float padding1;                               ; Offset:   72
;       float padding2;                               ; Offset:   76
;   
;   } $Element;                                       ; Offset:    0 Size:    80
;
; }
;
;
; Resource Bindings:
;
; Name                                 Type  Format         Dim      ID      HLSL Bind  Count
; ------------------------------ ---------- ------- ----------- ------- -------------- ------
; globalConstants                   cbuffer      NA          NA     CB0            cb0     1
; drawParam                         cbuffer      NA          NA     CB1            cb1     1
; vertices                          texture  struct         r/o      T0      t0,space1     1
; meshlets                          texture  struct         r/o      T1      t1,space1     1
; localVertexIndices                texture  struct         r/o      T2      t2,space1     1
; localPrimIndices                  texture  struct         r/o      T3      t3,space1     1
; instanceDatas                     texture  struct         r/o      T4      t4,space1     1
;
;
; ViewId state:
;
; Number of inputs: 0, outputs: 21, primitive outputs: 0
; Outputs dependent on ViewId: {  }
; Primitive Outputs dependent on ViewId: {  }
; Inputs contributing to computation of Outputs:
; Inputs contributing to computation of Primitive Outputs:
;
target datalayout = "e-m:e-p:32:32-i1:32-i8:32-i16:32-i32:32-i64:64-f16:32-f32:32-f64:64-n8:16:32:64"
target triple = "dxil-ms-dx"

%dx.types.Handle = type { i8* }
%struct.Payload = type { i32, i32 }
%dx.types.ResRet.i32 = type { i32, i32, i32, i32, i32 }
%dx.types.CBufRet.i32 = type { i32, i32, i32, i32 }
%dx.types.ResRet.f32 = type { float, float, float, float, i32 }
%dx.types.CBufRet.f32 = type { float, float, float, float }
%"class.StructuredBuffer<Vertex>" = type { %struct.Vertex }
%struct.Vertex = type { <3 x float>, <3 x float>, <3 x float>, <4 x float>, <2 x float> }
%"class.StructuredBuffer<Meshlet>" = type { %struct.Meshlet }
%struct.Meshlet = type { i32, i32, i32, i32 }
%"class.StructuredBuffer<unsigned int>" = type { i32 }
%"dx.alignment.legacy.class.StructuredBuffer<InstanceData>" = type { %dx.alignment.legacy.struct.InstanceData }
%dx.alignment.legacy.struct.InstanceData = type { [4 x <4 x float>], i32, float, float, float }
%dx.alignment.legacy.globalConstants = type { %dx.alignment.legacy.struct.GlobalConstants }
%dx.alignment.legacy.struct.GlobalConstants = type { [4 x <4 x float>], <3 x float>, float, <4 x float>, <3 x float>, float }
%drawParam = type { %struct.DrawParam }
%struct.DrawParam = type { i32, i32, i32, i32 }

define void @MS() {
  %1 = call %dx.types.Handle @dx.op.createHandle(i32 57, i8 0, i32 4, i32 4, i1 false)  ; CreateHandle(resourceClass,rangeId,index,nonUniformIndex)
  %2 = call %dx.types.Handle @dx.op.createHandle(i32 57, i8 0, i32 3, i32 3, i1 false)  ; CreateHandle(resourceClass,rangeId,index,nonUniformIndex)
  %3 = call %dx.types.Handle @dx.op.createHandle(i32 57, i8 0, i32 2, i32 2, i1 false)  ; CreateHandle(resourceClass,rangeId,index,nonUniformIndex)
  %4 = call %dx.types.Handle @dx.op.createHandle(i32 57, i8 0, i32 1, i32 1, i1 false)  ; CreateHandle(resourceClass,rangeId,index,nonUniformIndex)
  %5 = call %dx.types.Handle @dx.op.createHandle(i32 57, i8 0, i32 0, i32 0, i1 false)  ; CreateHandle(resourceClass,rangeId,index,nonUniformIndex)
  %6 = call %dx.types.Handle @dx.op.createHandle(i32 57, i8 2, i32 1, i32 1, i1 false)  ; CreateHandle(resourceClass,rangeId,index,nonUniformIndex)
  %7 = call %dx.types.Handle @dx.op.createHandle(i32 57, i8 2, i32 0, i32 0, i1 false)  ; CreateHandle(resourceClass,rangeId,index,nonUniformIndex)
  %8 = call i32 @dx.op.groupId.i32(i32 94, i32 0)  ; GroupId(component)
  %9 = call i32 @dx.op.threadIdInGroup.i32(i32 95, i32 0)  ; ThreadIdInGroup(component)
  %10 = call %struct.Payload* @dx.op.getMeshPayload.struct.Payload(i32 170)  ; GetMeshPayload()
  %11 = getelementptr inbounds %struct.Payload, %struct.Payload* %10, i32 0, i32 0
  %12 = load i32, i32* %11, align 4, !tbaa !36
  %13 = udiv i32 %8, %12
  %14 = call %dx.types.ResRet.i32 @dx.op.rawBufferLoad.i32(i32 139, %dx.types.Handle %4, i32 %13, i32 0, i8 1, i32 4)  ; RawBufferLoad(srv,index,elementOffset,mask,alignment)
  %15 = extractvalue %dx.types.ResRet.i32 %14, 0
  %16 = call %dx.types.ResRet.i32 @dx.op.rawBufferLoad.i32(i32 139, %dx.types.Handle %4, i32 %13, i32 4, i8 1, i32 4)  ; RawBufferLoad(srv,index,elementOffset,mask,alignment)
  %17 = extractvalue %dx.types.ResRet.i32 %16, 0
  %18 = call %dx.types.ResRet.i32 @dx.op.rawBufferLoad.i32(i32 139, %dx.types.Handle %4, i32 %13, i32 8, i8 1, i32 4)  ; RawBufferLoad(srv,index,elementOffset,mask,alignment)
  %19 = extractvalue %dx.types.ResRet.i32 %18, 0
  %20 = call %dx.types.ResRet.i32 @dx.op.rawBufferLoad.i32(i32 139, %dx.types.Handle %4, i32 %13, i32 12, i8 1, i32 4)  ; RawBufferLoad(srv,index,elementOffset,mask,alignment)
  %21 = extractvalue %dx.types.ResRet.i32 %20, 0
  %22 = urem i32 %8, %12
  %23 = call %dx.types.CBufRet.i32 @dx.op.cbufferLoadLegacy.i32(i32 59, %dx.types.Handle %6, i32 0)  ; CBufferLoadLegacy(handle,regIndex)
  %24 = extractvalue %dx.types.CBufRet.i32 %23, 1
  %25 = add i32 %24, -1
  %26 = icmp eq i32 %13, %25
  br i1 %26, label %27, label %36

; <label>:27                                      ; preds = %0
  %28 = udiv i32 126, %19
  %29 = udiv i32 64, %15
  %30 = call i32 @dx.op.binary.i32(i32 40, i32 %29, i32 %28)  ; UMin(a,b)
  %31 = mul i32 %13, %12
  %32 = sub i32 %8, %31
  %33 = mul i32 %32, %30
  %34 = sub i32 %12, %33
  %35 = call i32 @dx.op.binary.i32(i32 40, i32 %34, i32 %30)  ; UMin(a,b)
  br label %36

; <label>:36                                      ; preds = %27, %0
  %37 = phi i32 [ %33, %27 ], [ %22, %0 ]
  %38 = phi i32 [ %35, %27 ], [ 1, %0 ]
  %39 = mul i32 %38, %15
  %40 = mul i32 %38, %19
  call void @dx.op.setMeshOutputCounts(i32 168, i32 %39, i32 %40)  ; SetMeshOutputCounts(numVertices,numPrimitives)
  %41 = icmp ult i32 %9, %39
  br i1 %41, label %42, label %164

; <label>:42                                      ; preds = %36
  %43 = urem i32 %9, %15
  %44 = udiv i32 %9, %15
  %45 = add i32 %43, %17
  %46 = call %dx.types.ResRet.i32 @dx.op.rawBufferLoad.i32(i32 139, %dx.types.Handle %3, i32 %45, i32 0, i8 1, i32 4)  ; RawBufferLoad(srv,index,elementOffset,mask,alignment)
  %47 = extractvalue %dx.types.ResRet.i32 %46, 0
  %48 = add i32 %44, %37
  %49 = getelementptr inbounds %struct.Payload, %struct.Payload* %10, i32 0, i32 1
  %50 = load i32, i32* %49, align 4, !tbaa !36
  %51 = add i32 %48, %50
  %52 = call %dx.types.ResRet.f32 @dx.op.rawBufferLoad.f32(i32 139, %dx.types.Handle %1, i32 %51, i32 0, i8 15, i32 4)  ; RawBufferLoad(srv,index,elementOffset,mask,alignment)
  %53 = extractvalue %dx.types.ResRet.f32 %52, 0
  %54 = extractvalue %dx.types.ResRet.f32 %52, 1
  %55 = extractvalue %dx.types.ResRet.f32 %52, 2
  %56 = extractvalue %dx.types.ResRet.f32 %52, 3
  %57 = call %dx.types.ResRet.f32 @dx.op.rawBufferLoad.f32(i32 139, %dx.types.Handle %1, i32 %51, i32 16, i8 15, i32 4)  ; RawBufferLoad(srv,index,elementOffset,mask,alignment)
  %58 = extractvalue %dx.types.ResRet.f32 %57, 0
  %59 = extractvalue %dx.types.ResRet.f32 %57, 1
  %60 = extractvalue %dx.types.ResRet.f32 %57, 2
  %61 = extractvalue %dx.types.ResRet.f32 %57, 3
  %62 = call %dx.types.ResRet.f32 @dx.op.rawBufferLoad.f32(i32 139, %dx.types.Handle %1, i32 %51, i32 32, i8 15, i32 4)  ; RawBufferLoad(srv,index,elementOffset,mask,alignment)
  %63 = extractvalue %dx.types.ResRet.f32 %62, 0
  %64 = extractvalue %dx.types.ResRet.f32 %62, 1
  %65 = extractvalue %dx.types.ResRet.f32 %62, 2
  %66 = extractvalue %dx.types.ResRet.f32 %62, 3
  %67 = call %dx.types.ResRet.f32 @dx.op.rawBufferLoad.f32(i32 139, %dx.types.Handle %1, i32 %51, i32 48, i8 15, i32 4)  ; RawBufferLoad(srv,index,elementOffset,mask,alignment)
  %68 = extractvalue %dx.types.ResRet.f32 %67, 0
  %69 = extractvalue %dx.types.ResRet.f32 %67, 1
  %70 = extractvalue %dx.types.ResRet.f32 %67, 2
  %71 = extractvalue %dx.types.ResRet.f32 %67, 3
  %72 = call %dx.types.ResRet.i32 @dx.op.rawBufferLoad.i32(i32 139, %dx.types.Handle %1, i32 %51, i32 64, i8 1, i32 4)  ; RawBufferLoad(srv,index,elementOffset,mask,alignment)
  %73 = extractvalue %dx.types.ResRet.i32 %72, 0
  %74 = call %dx.types.ResRet.f32 @dx.op.rawBufferLoad.f32(i32 139, %dx.types.Handle %5, i32 %47, i32 0, i8 7, i32 4)  ; RawBufferLoad(srv,index,elementOffset,mask,alignment)
  %75 = extractvalue %dx.types.ResRet.f32 %74, 0
  %76 = extractvalue %dx.types.ResRet.f32 %74, 1
  %77 = extractvalue %dx.types.ResRet.f32 %74, 2
  %78 = call %dx.types.ResRet.f32 @dx.op.rawBufferLoad.f32(i32 139, %dx.types.Handle %5, i32 %47, i32 12, i8 7, i32 4)  ; RawBufferLoad(srv,index,elementOffset,mask,alignment)
  %79 = extractvalue %dx.types.ResRet.f32 %78, 0
  %80 = extractvalue %dx.types.ResRet.f32 %78, 1
  %81 = extractvalue %dx.types.ResRet.f32 %78, 2
  %82 = call %dx.types.ResRet.f32 @dx.op.rawBufferLoad.f32(i32 139, %dx.types.Handle %5, i32 %47, i32 24, i8 7, i32 4)  ; RawBufferLoad(srv,index,elementOffset,mask,alignment)
  %83 = extractvalue %dx.types.ResRet.f32 %82, 0
  %84 = extractvalue %dx.types.ResRet.f32 %82, 1
  %85 = extractvalue %dx.types.ResRet.f32 %82, 2
  %86 = call %dx.types.ResRet.f32 @dx.op.rawBufferLoad.f32(i32 139, %dx.types.Handle %5, i32 %47, i32 36, i8 15, i32 4)  ; RawBufferLoad(srv,index,elementOffset,mask,alignment)
  %87 = extractvalue %dx.types.ResRet.f32 %86, 0
  %88 = extractvalue %dx.types.ResRet.f32 %86, 1
  %89 = extractvalue %dx.types.ResRet.f32 %86, 2
  %90 = extractvalue %dx.types.ResRet.f32 %86, 3
  %91 = call %dx.types.ResRet.f32 @dx.op.rawBufferLoad.f32(i32 139, %dx.types.Handle %5, i32 %47, i32 52, i8 3, i32 4)  ; RawBufferLoad(srv,index,elementOffset,mask,alignment)
  %92 = extractvalue %dx.types.ResRet.f32 %91, 0
  %93 = extractvalue %dx.types.ResRet.f32 %91, 1
  %94 = fmul fast float %75, %53
  %95 = call float @dx.op.tertiary.f32(i32 46, float %76, float %54, float %94)  ; FMad(a,b,c)
  %96 = call float @dx.op.tertiary.f32(i32 46, float %77, float %55, float %95)  ; FMad(a,b,c)
  %97 = fadd fast float %96, %56
  %98 = fmul fast float %75, %58
  %99 = call float @dx.op.tertiary.f32(i32 46, float %76, float %59, float %98)  ; FMad(a,b,c)
  %100 = call float @dx.op.tertiary.f32(i32 46, float %77, float %60, float %99)  ; FMad(a,b,c)
  %101 = fadd fast float %100, %61
  %102 = fmul fast float %75, %63
  %103 = call float @dx.op.tertiary.f32(i32 46, float %76, float %64, float %102)  ; FMad(a,b,c)
  %104 = call float @dx.op.tertiary.f32(i32 46, float %77, float %65, float %103)  ; FMad(a,b,c)
  %105 = fadd fast float %104, %66
  %106 = fmul fast float %75, %68
  %107 = call float @dx.op.tertiary.f32(i32 46, float %76, float %69, float %106)  ; FMad(a,b,c)
  %108 = call float @dx.op.tertiary.f32(i32 46, float %77, float %70, float %107)  ; FMad(a,b,c)
  %109 = fadd fast float %108, %71
  %110 = fmul fast float %79, %53
  %111 = call float @dx.op.tertiary.f32(i32 46, float %80, float %54, float %110)  ; FMad(a,b,c)
  %112 = call float @dx.op.tertiary.f32(i32 46, float %81, float %55, float %111)  ; FMad(a,b,c)
  %113 = fmul fast float %79, %58
  %114 = call float @dx.op.tertiary.f32(i32 46, float %80, float %59, float %113)  ; FMad(a,b,c)
  %115 = call float @dx.op.tertiary.f32(i32 46, float %81, float %60, float %114)  ; FMad(a,b,c)
  %116 = fmul fast float %79, %63
  %117 = call float @dx.op.tertiary.f32(i32 46, float %80, float %64, float %116)  ; FMad(a,b,c)
  %118 = call float @dx.op.tertiary.f32(i32 46, float %81, float %65, float %117)  ; FMad(a,b,c)
  %119 = fmul fast float %83, %53
  %120 = call float @dx.op.tertiary.f32(i32 46, float %84, float %54, float %119)  ; FMad(a,b,c)
  %121 = call float @dx.op.tertiary.f32(i32 46, float %85, float %55, float %120)  ; FMad(a,b,c)
  %122 = fmul fast float %83, %58
  %123 = call float @dx.op.tertiary.f32(i32 46, float %84, float %59, float %122)  ; FMad(a,b,c)
  %124 = call float @dx.op.tertiary.f32(i32 46, float %85, float %60, float %123)  ; FMad(a,b,c)
  %125 = fmul fast float %83, %63
  %126 = call float @dx.op.tertiary.f32(i32 46, float %84, float %64, float %125)  ; FMad(a,b,c)
  %127 = call float @dx.op.tertiary.f32(i32 46, float %85, float %65, float %126)  ; FMad(a,b,c)
  %128 = call %dx.types.CBufRet.f32 @dx.op.cbufferLoadLegacy.f32(i32 59, %dx.types.Handle %7, i32 0)  ; CBufferLoadLegacy(handle,regIndex)
  %129 = extractvalue %dx.types.CBufRet.f32 %128, 0
  %130 = extractvalue %dx.types.CBufRet.f32 %128, 1
  %131 = extractvalue %dx.types.CBufRet.f32 %128, 2
  %132 = extractvalue %dx.types.CBufRet.f32 %128, 3
  %133 = call %dx.types.CBufRet.f32 @dx.op.cbufferLoadLegacy.f32(i32 59, %dx.types.Handle %7, i32 1)  ; CBufferLoadLegacy(handle,regIndex)
  %134 = extractvalue %dx.types.CBufRet.f32 %133, 0
  %135 = extractvalue %dx.types.CBufRet.f32 %133, 1
  %136 = extractvalue %dx.types.CBufRet.f32 %133, 2
  %137 = extractvalue %dx.types.CBufRet.f32 %133, 3
  %138 = call %dx.types.CBufRet.f32 @dx.op.cbufferLoadLegacy.f32(i32 59, %dx.types.Handle %7, i32 2)  ; CBufferLoadLegacy(handle,regIndex)
  %139 = extractvalue %dx.types.CBufRet.f32 %138, 0
  %140 = extractvalue %dx.types.CBufRet.f32 %138, 1
  %141 = extractvalue %dx.types.CBufRet.f32 %138, 2
  %142 = extractvalue %dx.types.CBufRet.f32 %138, 3
  %143 = call %dx.types.CBufRet.f32 @dx.op.cbufferLoadLegacy.f32(i32 59, %dx.types.Handle %7, i32 3)  ; CBufferLoadLegacy(handle,regIndex)
  %144 = extractvalue %dx.types.CBufRet.f32 %143, 0
  %145 = extractvalue %dx.types.CBufRet.f32 %143, 1
  %146 = extractvalue %dx.types.CBufRet.f32 %143, 2
  %147 = extractvalue %dx.types.CBufRet.f32 %143, 3
  %148 = fmul fast float %129, %97
  %149 = call float @dx.op.tertiary.f32(i32 46, float %101, float %130, float %148)  ; FMad(a,b,c)
  %150 = call float @dx.op.tertiary.f32(i32 46, float %105, float %131, float %149)  ; FMad(a,b,c)
  %151 = call float @dx.op.tertiary.f32(i32 46, float %109, float %132, float %150)  ; FMad(a,b,c)
  %152 = fmul fast float %134, %97
  %153 = call float @dx.op.tertiary.f32(i32 46, float %101, float %135, float %152)  ; FMad(a,b,c)
  %154 = call float @dx.op.tertiary.f32(i32 46, float %105, float %136, float %153)  ; FMad(a,b,c)
  %155 = call float @dx.op.tertiary.f32(i32 46, float %109, float %137, float %154)  ; FMad(a,b,c)
  %156 = fmul fast float %139, %97
  %157 = call float @dx.op.tertiary.f32(i32 46, float %101, float %140, float %156)  ; FMad(a,b,c)
  %158 = call float @dx.op.tertiary.f32(i32 46, float %105, float %141, float %157)  ; FMad(a,b,c)
  %159 = call float @dx.op.tertiary.f32(i32 46, float %109, float %142, float %158)  ; FMad(a,b,c)
  %160 = fmul fast float %144, %97
  %161 = call float @dx.op.tertiary.f32(i32 46, float %101, float %145, float %160)  ; FMad(a,b,c)
  %162 = call float @dx.op.tertiary.f32(i32 46, float %105, float %146, float %161)  ; FMad(a,b,c)
  %163 = call float @dx.op.tertiary.f32(i32 46, float %109, float %147, float %162)  ; FMad(a,b,c)
  call void @dx.op.storeVertexOutput.f32(i32 171, i32 0, i32 0, i8 0, float %151, i32 %9)  ; StoreVertexOutput(outputSigId,rowIndex,colIndex,value,vertexIndex)
  call void @dx.op.storeVertexOutput.f32(i32 171, i32 0, i32 0, i8 1, float %155, i32 %9)  ; StoreVertexOutput(outputSigId,rowIndex,colIndex,value,vertexIndex)
  call void @dx.op.storeVertexOutput.f32(i32 171, i32 0, i32 0, i8 2, float %159, i32 %9)  ; StoreVertexOutput(outputSigId,rowIndex,colIndex,value,vertexIndex)
  call void @dx.op.storeVertexOutput.f32(i32 171, i32 0, i32 0, i8 3, float %163, i32 %9)  ; StoreVertexOutput(outputSigId,rowIndex,colIndex,value,vertexIndex)
  call void @dx.op.storeVertexOutput.f32(i32 171, i32 1, i32 0, i8 0, float %112, i32 %9)  ; StoreVertexOutput(outputSigId,rowIndex,colIndex,value,vertexIndex)
  call void @dx.op.storeVertexOutput.f32(i32 171, i32 1, i32 0, i8 1, float %115, i32 %9)  ; StoreVertexOutput(outputSigId,rowIndex,colIndex,value,vertexIndex)
  call void @dx.op.storeVertexOutput.f32(i32 171, i32 1, i32 0, i8 2, float %118, i32 %9)  ; StoreVertexOutput(outputSigId,rowIndex,colIndex,value,vertexIndex)
  call void @dx.op.storeVertexOutput.f32(i32 171, i32 2, i32 0, i8 0, float %121, i32 %9)  ; StoreVertexOutput(outputSigId,rowIndex,colIndex,value,vertexIndex)
  call void @dx.op.storeVertexOutput.f32(i32 171, i32 2, i32 0, i8 1, float %124, i32 %9)  ; StoreVertexOutput(outputSigId,rowIndex,colIndex,value,vertexIndex)
  call void @dx.op.storeVertexOutput.f32(i32 171, i32 2, i32 0, i8 2, float %127, i32 %9)  ; StoreVertexOutput(outputSigId,rowIndex,colIndex,value,vertexIndex)
  call void @dx.op.storeVertexOutput.f32(i32 171, i32 3, i32 0, i8 0, float %87, i32 %9)  ; StoreVertexOutput(outputSigId,rowIndex,colIndex,value,vertexIndex)
  call void @dx.op.storeVertexOutput.f32(i32 171, i32 3, i32 0, i8 1, float %88, i32 %9)  ; StoreVertexOutput(outputSigId,rowIndex,colIndex,value,vertexIndex)
  call void @dx.op.storeVertexOutput.f32(i32 171, i32 3, i32 0, i8 2, float %89, i32 %9)  ; StoreVertexOutput(outputSigId,rowIndex,colIndex,value,vertexIndex)
  call void @dx.op.storeVertexOutput.f32(i32 171, i32 3, i32 0, i8 3, float %90, i32 %9)  ; StoreVertexOutput(outputSigId,rowIndex,colIndex,value,vertexIndex)
  call void @dx.op.storeVertexOutput.f32(i32 171, i32 4, i32 0, i8 0, float %92, i32 %9)  ; StoreVertexOutput(outputSigId,rowIndex,colIndex,value,vertexIndex)
  call void @dx.op.storeVertexOutput.f32(i32 171, i32 4, i32 0, i8 1, float %93, i32 %9)  ; StoreVertexOutput(outputSigId,rowIndex,colIndex,value,vertexIndex)
  call void @dx.op.storeVertexOutput.i32(i32 171, i32 5, i32 0, i8 0, i32 %73, i32 %9)  ; StoreVertexOutput(outputSigId,rowIndex,colIndex,value,vertexIndex)
  br label %164

; <label>:164                                     ; preds = %42, %36
  %165 = icmp ult i32 %9, %40
  br i1 %165, label %166, label %181

; <label>:166                                     ; preds = %164
  %167 = urem i32 %9, %19
  %168 = udiv i32 %9, %19
  %169 = add i32 %167, %21
  %170 = call %dx.types.ResRet.i32 @dx.op.rawBufferLoad.i32(i32 139, %dx.types.Handle %2, i32 %169, i32 0, i8 1, i32 4)  ; RawBufferLoad(srv,index,elementOffset,mask,alignment)
  %171 = extractvalue %dx.types.ResRet.i32 %170, 0
  %172 = and i32 %171, 1023
  %173 = lshr i32 %171, 10
  %174 = and i32 %173, 1023
  %175 = lshr i32 %171, 20
  %176 = and i32 %175, 1023
  %177 = mul i32 %168, %15
  %178 = add i32 %172, %177
  %179 = add i32 %174, %177
  %180 = add i32 %176, %177
  call void @dx.op.emitIndices(i32 169, i32 %9, i32 %178, i32 %179, i32 %180)  ; EmitIndices(PrimitiveIndex,VertexIndex0,VertexIndex1,VertexIndex2)
  br label %181

; <label>:181                                     ; preds = %166, %164
  ret void
}

; Function Attrs: nounwind
declare void @dx.op.emitIndices(i32, i32, i32, i32, i32) #0

; Function Attrs: nounwind readonly
declare %struct.Payload* @dx.op.getMeshPayload.struct.Payload(i32) #1

; Function Attrs: nounwind readnone
declare i32 @dx.op.threadIdInGroup.i32(i32, i32) #2

; Function Attrs: nounwind readnone
declare i32 @dx.op.groupId.i32(i32, i32) #2

; Function Attrs: nounwind
declare void @dx.op.storeVertexOutput.f32(i32, i32, i32, i8, float, i32) #0

; Function Attrs: nounwind
declare void @dx.op.storeVertexOutput.i32(i32, i32, i32, i8, i32, i32) #0

; Function Attrs: nounwind readonly
declare %dx.types.ResRet.i32 @dx.op.rawBufferLoad.i32(i32, %dx.types.Handle, i32, i32, i8, i32) #1

; Function Attrs: nounwind readnone
declare i32 @dx.op.binary.i32(i32, i32, i32) #2

; Function Attrs: nounwind
declare void @dx.op.setMeshOutputCounts(i32, i32, i32) #0

; Function Attrs: nounwind readonly
declare %dx.types.ResRet.f32 @dx.op.rawBufferLoad.f32(i32, %dx.types.Handle, i32, i32, i8, i32) #1

; Function Attrs: nounwind readonly
declare %dx.types.CBufRet.f32 @dx.op.cbufferLoadLegacy.f32(i32, %dx.types.Handle, i32) #1

; Function Attrs: nounwind readonly
declare %dx.types.CBufRet.i32 @dx.op.cbufferLoadLegacy.i32(i32, %dx.types.Handle, i32) #1

; Function Attrs: nounwind readnone
declare float @dx.op.tertiary.f32(i32, float, float, float) #2

; Function Attrs: nounwind readonly
declare %dx.types.Handle @dx.op.createHandle(i32, i8, i32, i32, i1) #1

attributes #0 = { nounwind }
attributes #1 = { nounwind readonly }
attributes #2 = { nounwind readnone }

!llvm.ident = !{!0}
!dx.version = !{!1}
!dx.valver = !{!1}
!dx.shaderModel = !{!2}
!dx.resources = !{!3}
!dx.viewIdState = !{!17}
!dx.entryPoints = !{!18}

!0 = !{!"clang version 3.7 (tags/RELEASE_370/final)"}
!1 = !{i32 1, i32 5}
!2 = !{!"ms", i32 6, i32 5}
!3 = !{!4, null, !14, null}
!4 = !{!5, !7, !9, !11, !12}
!5 = !{i32 0, %"class.StructuredBuffer<Vertex>"* undef, !"", i32 1, i32 0, i32 1, i32 12, i32 0, !6}
!6 = !{i32 1, i32 60}
!7 = !{i32 1, %"class.StructuredBuffer<Meshlet>"* undef, !"", i32 1, i32 1, i32 1, i32 12, i32 0, !8}
!8 = !{i32 1, i32 16}
!9 = !{i32 2, %"class.StructuredBuffer<unsigned int>"* undef, !"", i32 1, i32 2, i32 1, i32 12, i32 0, !10}
!10 = !{i32 1, i32 4}
!11 = !{i32 3, %"class.StructuredBuffer<unsigned int>"* undef, !"", i32 1, i32 3, i32 1, i32 12, i32 0, !10}
!12 = !{i32 4, %"dx.alignment.legacy.class.StructuredBuffer<InstanceData>"* undef, !"", i32 1, i32 4, i32 1, i32 12, i32 0, !13}
!13 = !{i32 1, i32 80}
!14 = !{!15, !16}
!15 = !{i32 0, %dx.alignment.legacy.globalConstants* undef, !"", i32 0, i32 0, i32 1, i32 112, null}
!16 = !{i32 1, %drawParam* undef, !"", i32 0, i32 1, i32 1, i32 16, null}
!17 = !{[3 x i32] [i32 0, i32 21, i32 0]}
!18 = !{void ()* @MS, !"MS", !19, !3, !33}
!19 = !{null, !20, null}
!20 = !{!21, !24, !26, !27, !28, !30}
!21 = !{i32 0, !"SV_Position", i8 9, i8 3, !22, i8 4, i32 1, i8 4, i32 0, i8 0, !23}
!22 = !{i32 0}
!23 = !{i32 3, i32 15}
!24 = !{i32 1, !"NORMAL", i8 9, i8 0, !22, i8 2, i32 1, i8 3, i32 1, i8 0, !25}
!25 = !{i32 3, i32 7}
!26 = !{i32 2, !"TANGENT", i8 9, i8 0, !22, i8 2, i32 1, i8 3, i32 2, i8 0, !25}
!27 = !{i32 3, !"COLOR", i8 9, i8 0, !22, i8 2, i32 1, i8 4, i32 3, i8 0, !23}
!28 = !{i32 4, !"TEXCOORD", i8 9, i8 0, !22, i8 2, i32 1, i8 2, i32 4, i8 0, !29}
!29 = !{i32 3, i32 3}
!30 = !{i32 5, !"TEXCOORD", i8 5, i8 0, !31, i8 1, i32 1, i8 1, i32 5, i8 0, !32}
!31 = !{i32 1}
!32 = !{i32 3, i32 1}
!33 = !{i32 0, i64 16, i32 9, !34}
!34 = !{!35, i32 64, i32 126, i32 2, i32 8}
!35 = !{i32 128, i32 1, i32 1}
!36 = !{!37, !37, i64 0}
!37 = !{!"int", !38, i64 0}
!38 = !{!"omnipotent char", !39, i64 0}
!39 = !{!"Simple C/C++ TBAA"}
