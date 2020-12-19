;
; Input signature:
;
; Name                 Index   Mask Register SysValue  Format   Used
; -------------------- ----- ------ -------- -------- ------- ------
; SV_Position              0   xyzw        0      POS   float       
; NORMAL                   0   xyz         1     NONE   float   xyz 
; TANGENT                  0   xyz         2     NONE   float       
; COLOR                    0   xyzw        3     NONE   float       
; TEXCOORD                 0   xy          4     NONE   float   xy  
; TEXCOORD                 1   x           5     NONE    uint   x   
;
;
; Output signature:
;
; Name                 Index   Mask Register SysValue  Format   Used
; -------------------- ----- ------ -------- -------- ------- ------
; SV_Target                0   xyzw        0   TARGET   float   xyzw
;
; shader hash: 1128b24d8f7c1b8cdcf4f281a18eb56d
;
; Pipeline Runtime Information: 
;
; Pixel Shader
; DepthOutput=0
; SampleFrequency=0
;
;
; Input signature:
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
; Output signature:
;
; Name                 Index             InterpMode DynIdx
; -------------------- ----- ---------------------- ------
; SV_Target                0                              
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
; Resource bind info for materialDatas
; {
;
;   struct struct.MaterialData
;   {
;
;       uint diffuseTextureIndex;                     ; Offset:    0
;       float padding0;                               ; Offset:    4
;       float padding1;                               ; Offset:    8
;       float padding2;                               ; Offset:   12
;   
;   } $Element;                                       ; Offset:    0 Size:    16
;
; }
;
;
; Resource Bindings:
;
; Name                                 Type  Format         Dim      ID      HLSL Bind  Count
; ------------------------------ ---------- ------- ----------- ------- -------------- ------
; globalConstants                   cbuffer      NA          NA     CB0            cb0     1
; samplerLinearWrap                 sampler      NA          NA      S0             s0     1
; materialDatas                     texture  struct         r/o      T0      t5,space1     1
; textures                          texture     f32          2d      T1             t0unbounded
;
;
; ViewId state:
;
; Number of inputs: 21, outputs: 4
; Outputs dependent on ViewId: {  }
; Inputs contributing to computation of Outputs:
;   output 0 depends on inputs: { 4, 5, 6, 16, 17, 20 }
;   output 1 depends on inputs: { 4, 5, 6, 16, 17, 20 }
;   output 2 depends on inputs: { 4, 5, 6, 16, 17, 20 }
;
target datalayout = "e-m:e-p:32:32-i1:32-i8:32-i16:32-i32:32-i64:64-f16:32-f32:32-f64:64-n8:16:32:64"
target triple = "dxil-ms-dx"

%dx.types.Handle = type { i8* }
%dx.types.ResRet.i32 = type { i32, i32, i32, i32, i32 }
%dx.types.ResRet.f32 = type { float, float, float, float, i32 }
%dx.types.CBufRet.f32 = type { float, float, float, float }
%"class.StructuredBuffer<MaterialData>" = type { %struct.MaterialData }
%struct.MaterialData = type { i32, float, float, float }
%"class.Texture2D<vector<float, 4> >" = type { <4 x float>, %"class.Texture2D<vector<float, 4> >::mips_type" }
%"class.Texture2D<vector<float, 4> >::mips_type" = type { i32 }
%dx.alignment.legacy.globalConstants = type { %dx.alignment.legacy.struct.GlobalConstants }
%dx.alignment.legacy.struct.GlobalConstants = type { [4 x <4 x float>], <3 x float>, float, <4 x float>, <3 x float>, float }
%struct.SamplerState = type { i32 }

define void @PS() {
  %1 = call %dx.types.Handle @dx.op.createHandle(i32 57, i8 0, i32 0, i32 5, i1 false)  ; CreateHandle(resourceClass,rangeId,index,nonUniformIndex)
  %2 = call %dx.types.Handle @dx.op.createHandle(i32 57, i8 3, i32 0, i32 0, i1 false)  ; CreateHandle(resourceClass,rangeId,index,nonUniformIndex)
  %3 = call %dx.types.Handle @dx.op.createHandle(i32 57, i8 2, i32 0, i32 0, i1 false)  ; CreateHandle(resourceClass,rangeId,index,nonUniformIndex)
  %4 = call i32 @dx.op.loadInput.i32(i32 4, i32 5, i32 0, i8 0, i32 undef)  ; LoadInput(inputSigId,rowIndex,colIndex,gsVertexAxis)
  %5 = call float @dx.op.loadInput.f32(i32 4, i32 4, i32 0, i8 0, i32 undef)  ; LoadInput(inputSigId,rowIndex,colIndex,gsVertexAxis)
  %6 = call float @dx.op.loadInput.f32(i32 4, i32 4, i32 0, i8 1, i32 undef)  ; LoadInput(inputSigId,rowIndex,colIndex,gsVertexAxis)
  %7 = call float @dx.op.loadInput.f32(i32 4, i32 1, i32 0, i8 0, i32 undef)  ; LoadInput(inputSigId,rowIndex,colIndex,gsVertexAxis)
  %8 = call float @dx.op.loadInput.f32(i32 4, i32 1, i32 0, i8 1, i32 undef)  ; LoadInput(inputSigId,rowIndex,colIndex,gsVertexAxis)
  %9 = call float @dx.op.loadInput.f32(i32 4, i32 1, i32 0, i8 2, i32 undef)  ; LoadInput(inputSigId,rowIndex,colIndex,gsVertexAxis)
  %10 = uitofp i32 %4 to float
  %11 = call float @dx.op.unary.f32(i32 26, float %10)  ; Round_ne(value)
  %12 = fptoui float %11 to i32
  %13 = call %dx.types.ResRet.i32 @dx.op.rawBufferLoad.i32(i32 139, %dx.types.Handle %1, i32 %12, i32 0, i8 1, i32 4)  ; RawBufferLoad(srv,index,elementOffset,mask,alignment)
  %14 = extractvalue %dx.types.ResRet.i32 %13, 0
  %15 = add i32 %14, 0
  %16 = call %dx.types.Handle @dx.op.createHandle(i32 57, i8 0, i32 1, i32 %15, i1 false)  ; CreateHandle(resourceClass,rangeId,index,nonUniformIndex)
  %17 = call %dx.types.ResRet.f32 @dx.op.sample.f32(i32 60, %dx.types.Handle %16, %dx.types.Handle %2, float %5, float %6, float undef, float undef, i32 0, i32 0, i32 undef, float undef)  ; Sample(srv,sampler,coord0,coord1,coord2,coord3,offset0,offset1,offset2,clamp)
  %18 = extractvalue %dx.types.ResRet.f32 %17, 0
  %19 = extractvalue %dx.types.ResRet.f32 %17, 1
  %20 = extractvalue %dx.types.ResRet.f32 %17, 2
  %21 = call %dx.types.CBufRet.f32 @dx.op.cbufferLoadLegacy.f32(i32 59, %dx.types.Handle %3, i32 4)  ; CBufferLoadLegacy(handle,regIndex)
  %22 = extractvalue %dx.types.CBufRet.f32 %21, 0
  %23 = extractvalue %dx.types.CBufRet.f32 %21, 1
  %24 = extractvalue %dx.types.CBufRet.f32 %21, 2
  %25 = fsub fast float -0.000000e+00, %22
  %26 = fsub fast float -0.000000e+00, %23
  %27 = fsub fast float -0.000000e+00, %24
  %28 = call float @dx.op.dot3.f32(i32 55, float %7, float %8, float %9, float %25, float %26, float %27)  ; Dot3(ax,ay,az,bx,by,bz)
  %29 = call %dx.types.CBufRet.f32 @dx.op.cbufferLoadLegacy.f32(i32 59, %dx.types.Handle %3, i32 5)  ; CBufferLoadLegacy(handle,regIndex)
  %30 = extractvalue %dx.types.CBufRet.f32 %29, 0
  %31 = extractvalue %dx.types.CBufRet.f32 %29, 1
  %32 = extractvalue %dx.types.CBufRet.f32 %29, 2
  %33 = fadd fast float %30, %28
  %34 = fmul fast float %33, %18
  %35 = fadd fast float %31, %28
  %36 = fmul fast float %35, %19
  %37 = fadd fast float %32, %28
  %38 = fmul fast float %37, %20
  call void @dx.op.storeOutput.f32(i32 5, i32 0, i32 0, i8 0, float %34)  ; StoreOutput(outputSigId,rowIndex,colIndex,value)
  call void @dx.op.storeOutput.f32(i32 5, i32 0, i32 0, i8 1, float %36)  ; StoreOutput(outputSigId,rowIndex,colIndex,value)
  call void @dx.op.storeOutput.f32(i32 5, i32 0, i32 0, i8 2, float %38)  ; StoreOutput(outputSigId,rowIndex,colIndex,value)
  call void @dx.op.storeOutput.f32(i32 5, i32 0, i32 0, i8 3, float 1.000000e+00)  ; StoreOutput(outputSigId,rowIndex,colIndex,value)
  ret void
}

; Function Attrs: nounwind readnone
declare float @dx.op.loadInput.f32(i32, i32, i32, i8, i32) #0

; Function Attrs: nounwind readnone
declare i32 @dx.op.loadInput.i32(i32, i32, i32, i8, i32) #0

; Function Attrs: nounwind
declare void @dx.op.storeOutput.f32(i32, i32, i32, i8, float) #1

; Function Attrs: nounwind readonly
declare %dx.types.ResRet.i32 @dx.op.rawBufferLoad.i32(i32, %dx.types.Handle, i32, i32, i8, i32) #2

; Function Attrs: nounwind readnone
declare float @dx.op.unary.f32(i32, float) #0

; Function Attrs: nounwind readonly
declare %dx.types.ResRet.f32 @dx.op.sample.f32(i32, %dx.types.Handle, %dx.types.Handle, float, float, float, float, i32, i32, i32, float) #2

; Function Attrs: nounwind readnone
declare float @dx.op.dot3.f32(i32, float, float, float, float, float, float) #0

; Function Attrs: nounwind readonly
declare %dx.types.CBufRet.f32 @dx.op.cbufferLoadLegacy.f32(i32, %dx.types.Handle, i32) #2

; Function Attrs: nounwind readonly
declare %dx.types.Handle @dx.op.createHandle(i32, i8, i32, i32, i1) #2

attributes #0 = { nounwind readnone }
attributes #1 = { nounwind }
attributes #2 = { nounwind readonly }

!llvm.ident = !{!0}
!dx.version = !{!1}
!dx.valver = !{!1}
!dx.shaderModel = !{!2}
!dx.resources = !{!3}
!dx.viewIdState = !{!13}
!dx.entryPoints = !{!14}

!0 = !{!"clang version 3.7 (tags/RELEASE_370/final)"}
!1 = !{i32 1, i32 5}
!2 = !{!"ps", i32 6, i32 5}
!3 = !{!4, null, !9, !11}
!4 = !{!5, !7}
!5 = !{i32 0, %"class.StructuredBuffer<MaterialData>"* undef, !"", i32 1, i32 5, i32 1, i32 12, i32 0, !6}
!6 = !{i32 1, i32 16}
!7 = !{i32 1, [0 x %"class.Texture2D<vector<float, 4> >"]* undef, !"", i32 0, i32 0, i32 -1, i32 2, i32 0, !8}
!8 = !{i32 0, i32 9}
!9 = !{!10}
!10 = !{i32 0, %dx.alignment.legacy.globalConstants* undef, !"", i32 0, i32 0, i32 1, i32 112, null}
!11 = !{!12}
!12 = !{i32 0, %struct.SamplerState* undef, !"", i32 0, i32 0, i32 1, i32 0, null}
!13 = !{[23 x i32] [i32 21, i32 4, i32 0, i32 0, i32 0, i32 0, i32 7, i32 7, i32 7, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 7, i32 7, i32 0, i32 0, i32 7]}
!14 = !{void ()* @PS, !"PS", !15, !3, !31}
!15 = !{!16, !28, null}
!16 = !{!17, !19, !21, !22, !23, !25}
!17 = !{i32 0, !"SV_Position", i8 9, i8 3, !18, i8 4, i32 1, i8 4, i32 0, i8 0, null}
!18 = !{i32 0}
!19 = !{i32 1, !"NORMAL", i8 9, i8 0, !18, i8 2, i32 1, i8 3, i32 1, i8 0, !20}
!20 = !{i32 3, i32 7}
!21 = !{i32 2, !"TANGENT", i8 9, i8 0, !18, i8 2, i32 1, i8 3, i32 2, i8 0, null}
!22 = !{i32 3, !"COLOR", i8 9, i8 0, !18, i8 2, i32 1, i8 4, i32 3, i8 0, null}
!23 = !{i32 4, !"TEXCOORD", i8 9, i8 0, !18, i8 2, i32 1, i8 2, i32 4, i8 0, !24}
!24 = !{i32 3, i32 3}
!25 = !{i32 5, !"TEXCOORD", i8 5, i8 0, !26, i8 1, i32 1, i8 1, i32 5, i8 0, !27}
!26 = !{i32 1}
!27 = !{i32 3, i32 1}
!28 = !{!29}
!29 = !{i32 0, !"SV_Target", i8 9, i8 16, !18, i8 0, i32 1, i8 4, i32 0, i8 0, !30}
!30 = !{i32 3, i32 15}
!31 = !{i32 0, i64 16}
