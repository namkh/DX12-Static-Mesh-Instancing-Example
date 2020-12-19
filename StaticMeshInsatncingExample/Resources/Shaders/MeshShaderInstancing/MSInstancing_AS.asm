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
; no parameters
; shader hash: 57e6cb220e4875806c4e7e226b59ff12
;
; Pipeline Runtime Information: 
;
;
;
; Buffer Definitions:
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
;
; Resource Bindings:
;
; Name                                 Type  Format         Dim      ID      HLSL Bind  Count
; ------------------------------ ---------- ------- ----------- ------- -------------- ------
; drawParam                         cbuffer      NA          NA     CB0            cb1     1
;
target datalayout = "e-m:e-p:32:32-i1:32-i8:32-i16:32-i32:32-i64:64-f16:32-f32:32-f64:64-n8:16:32:64"
target triple = "dxil-ms-dx"

%struct.Payload = type { i32, i32 }
%dx.types.Handle = type { i8* }
%dx.types.CBufRet.i32 = type { i32, i32, i32, i32 }
%drawParam = type { %struct.DrawParam }
%struct.DrawParam = type { i32, i32, i32, i32 }

@"\01?payload@@3UPayload@@A" = external addrspace(3) global %struct.Payload, align 4

define void @AS() {
  %1 = call %dx.types.Handle @dx.op.createHandle(i32 57, i8 2, i32 0, i32 1, i1 false)  ; CreateHandle(resourceClass,rangeId,index,nonUniformIndex)
  %2 = call i32 @dx.op.groupId.i32(i32 94, i32 0)  ; GroupId(component)
  %3 = call %dx.types.CBufRet.i32 @dx.op.cbufferLoadLegacy.i32(i32 59, %dx.types.Handle %1, i32 0)  ; CBufferLoadLegacy(handle,regIndex)
  %4 = extractvalue %dx.types.CBufRet.i32 %3, 1
  %5 = shl i32 %4, 6
  %6 = add i32 %5, -64
  %7 = extractvalue %dx.types.CBufRet.i32 %3, 3
  %8 = udiv i32 126, %7
  %9 = extractvalue %dx.types.CBufRet.i32 %3, 2
  %10 = udiv i32 64, %9
  %11 = call i32 @dx.op.binary.i32(i32 40, i32 %10, i32 %8)  ; UMin(a,b)
  %12 = add i32 %11, 63
  %13 = udiv i32 %12, %11
  %14 = add i32 %6, %13
  %15 = shl i32 %2, 6
  %16 = extractvalue %dx.types.CBufRet.i32 %3, 0
  %17 = add i32 %16, %15
  store i32 %17, i32 addrspace(3)* getelementptr inbounds (%struct.Payload, %struct.Payload addrspace(3)* @"\01?payload@@3UPayload@@A", i32 0, i32 1), align 4, !tbaa !10
  store i32 64, i32 addrspace(3)* getelementptr inbounds (%struct.Payload, %struct.Payload addrspace(3)* @"\01?payload@@3UPayload@@A", i32 0, i32 0), align 4, !tbaa !10
  call void @dx.op.dispatchMesh.struct.Payload(i32 173, i32 %14, i32 1, i32 1, %struct.Payload addrspace(3)* nonnull @"\01?payload@@3UPayload@@A")  ; DispatchMesh(threadGroupCountX,threadGroupCountY,threadGroupCountZ,payload)
  ret void
}

; Function Attrs: nounwind readnone
declare i32 @dx.op.groupId.i32(i32, i32) #0

; Function Attrs: nounwind readnone
declare i32 @dx.op.binary.i32(i32, i32, i32) #0

; Function Attrs: nounwind readonly
declare %dx.types.CBufRet.i32 @dx.op.cbufferLoadLegacy.i32(i32, %dx.types.Handle, i32) #1

; Function Attrs: nounwind
declare void @dx.op.dispatchMesh.struct.Payload(i32, i32, i32, i32, %struct.Payload addrspace(3)*) #2

; Function Attrs: nounwind readonly
declare %dx.types.Handle @dx.op.createHandle(i32, i8, i32, i32, i1) #1

attributes #0 = { nounwind readnone }
attributes #1 = { nounwind readonly }
attributes #2 = { nounwind }

!llvm.ident = !{!0}
!dx.version = !{!1}
!dx.valver = !{!1}
!dx.shaderModel = !{!2}
!dx.resources = !{!3}
!dx.entryPoints = !{!6}

!0 = !{!"clang version 3.7 (tags/RELEASE_370/final)"}
!1 = !{i32 1, i32 5}
!2 = !{!"as", i32 6, i32 5}
!3 = !{null, null, !4, null}
!4 = !{!5}
!5 = !{i32 0, %drawParam* undef, !"", i32 0, i32 1, i32 1, i32 16, null}
!6 = !{void ()* @AS, !"AS", null, !3, !7}
!7 = !{i32 10, !8}
!8 = !{!9, i32 8}
!9 = !{i32 64, i32 1, i32 1}
!10 = !{!11, !11, i64 0}
!11 = !{!"int", !12, i64 0}
!12 = !{!"omnipotent char", !13, i64 0}
!13 = !{!"Simple C/C++ TBAA"}
