; RUN: opt -passes=mojpass %s -o %t 
; RUN: cat %t | FileCheck %s

; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"


; Function Attrs: nounwind uwtable
define dso_local i32 @main() #0 !dbg !9 {
entry:
  %retval = alloca i32, align 4
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  store i32 0, ptr %retval, align 4
  ; CHECK-NOT: !dbg !17
  call void @llvm.lifetime.start.p0(i64 4, ptr %a) #3, !dbg !17
  ; CHECK-NOT: !dbg !18
  call void @llvm.dbg.declare(metadata ptr %a, metadata !14, metadata !DIExpression()), !dbg !18
  store i32 5, ptr %a, align 4, !dbg !18, !tbaa !19
  ; CHECK-NOT: !dbg !23
  call void @llvm.lifetime.start.p0(i64 4, ptr %b) #3, !dbg !23
  ; CHECK-NOT: !dbg !24
  call void @llvm.dbg.declare(metadata ptr %b, metadata !15, metadata !DIExpression()), !dbg !24
  store i32 3, ptr %b, align 4, !dbg !24, !tbaa !19
  ; CHECK-NOT: !dbg !25
  call void @llvm.lifetime.start.p0(i64 4, ptr %c) #3, !dbg !25
  ; CHECK-NOT: !dbg !26
  call void @llvm.dbg.declare(metadata ptr %c, metadata !16, metadata !DIExpression()), !dbg !26
  %0 = load i32, ptr %a, align 4, !dbg !27, !tbaa !19
  %1 = load i32, ptr %b, align 4, !dbg !28, !tbaa !19
  ; CHECK-NOT: !dbg !29
  %add = add nsw i32 %0, %1, !dbg !29
  store i32 %add, ptr %c, align 4, !dbg !26, !tbaa !19
  %2 = load i32, ptr %c, align 4, !dbg !30, !tbaa !19
  ; CHECK-NOT: !dbg !31
  call void @llvm.lifetime.end.p0(i64 4, ptr %c) #3, !dbg !31
  call void @llvm.lifetime.end.p0(i64 4, ptr %b) #3, !dbg !31
  call void @llvm.lifetime.end.p0(i64 4, ptr %a) #3, !dbg !31
  ; CHECK-NOT: !dbg !32
  ret i32 %2, !dbg !32
}

; Function Attrs: argmemonly nocallback nofree nosync nounwind willreturn
declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #1

; Function Attrs: nocallback nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #2

; Function Attrs: argmemonly nocallback nofree nosync nounwind willreturn
declare void @llvm.lifetime.end.p0(i64 immarg, ptr nocapture) #1

attributes #0 = { nounwind uwtable "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { argmemonly nocallback nofree nosync nounwind willreturn }
attributes #2 = { nocallback nofree nosync nounwind readnone speculatable willreturn }
attributes #3 = { nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3, !4, !5, !6, !7}
!llvm.ident = !{!8}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 15.0.0 (https://github.com/llvm/llvm-project.git 41d5033eb162cb92b684855166cabfa3983b74c6)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "test.c", directory: "/home/syrmia/llvm/nekitest", checksumkind: CSK_MD5, checksum: "ca5d10bf9c5d9518273736e63bda268f")
!2 = !{i32 7, !"Dwarf Version", i32 5}
!3 = !{i32 2, !"Debug Info Version", i32 3}
!4 = !{i32 1, !"wchar_size", i32 4}
!5 = !{i32 7, !"PIC Level", i32 2}
!6 = !{i32 7, !"PIE Level", i32 2}
!7 = !{i32 7, !"uwtable", i32 2}
!8 = !{!"clang version 15.0.0 (https://github.com/llvm/llvm-project.git 41d5033eb162cb92b684855166cabfa3983b74c6)"}
!9 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 1, type: !10, scopeLine: 1, flags: DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !13)
!10 = !DISubroutineType(types: !11)
!11 = !{!12}
!12 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!13 = !{!14, !15, !16}
!14 = !DILocalVariable(name: "a", scope: !9, file: !1, line: 2, type: !12)
!15 = !DILocalVariable(name: "b", scope: !9, file: !1, line: 3, type: !12)
!16 = !DILocalVariable(name: "c", scope: !9, file: !1, line: 4, type: !12)
!17 = !DILocation(line: 2, column: 1, scope: !9)
!18 = !DILocation(line: 2, column: 5, scope: !9)
!19 = !{!20, !20, i64 0}
!20 = !{!"int", !21, i64 0}
!21 = !{!"omnipotent char", !22, i64 0}
!22 = !{!"Simple C/C++ TBAA"}
!23 = !DILocation(line: 3, column: 1, scope: !9)
!24 = !DILocation(line: 3, column: 5, scope: !9)
!25 = !DILocation(line: 4, column: 1, scope: !9)
!26 = !DILocation(line: 4, column: 5, scope: !9)
!27 = !DILocation(line: 4, column: 7, scope: !9)
!28 = !DILocation(line: 4, column: 9, scope: !9)
!29 = !DILocation(line: 4, column: 8, scope: !9)
!30 = !DILocation(line: 5, column: 8, scope: !9)
!31 = !DILocation(line: 6, column: 1, scope: !9)
!32 = !DILocation(line: 5, column: 1, scope: !9)
