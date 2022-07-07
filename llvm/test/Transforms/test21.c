// RUN: /home/syrmia/Desktop/llvm/llvm-project/build/bin/clang -target mips64-r6-linux-gnu -mabi=64 -Xclang -no-opaque-pointers %s -O2 -c -emit-llvm -S -o - | /home/syrmia/Desktop/llvm/llvm-project/build/bin/opt -aggressive-instcombine -mtriple mips64-r6-linux-gnu -S -o - | /home/syrmia/Desktop/llvm/llvm-project/build/bin/FileCheck %s

// CHECK: entry:
// CHECK: %cmp = icmp eq i32 %x, 0
// CHECK: br i1 %cmp, label %return, label %if.end
// CHECK: if.end:                                           ; preds = %entry
// CHECK: %0 = call i32 @llvm.cttz.i32(i32 %x, i1 true)
// CHECK: %1 = icmp eq i32 %x, 0
// CHECK: %2 = trunc i32 %0 to i8
// CHECK: %conv = sext i8 %2 to i32
// CHECK: br label %return
// CHECK: return:                                           ; preds = %if.end, %entry
// CHECK:  %retval.0 = phi i32 [ %conv, %if.end ], [ 32, %entry ]
// CHECK:  ret i32 %retval.0



int cttz(unsigned x) {
	static char table[32] =
	{ 0, 1, 2,24, 3,19, 6,25, 22, 4,20,10,16, 7,12,26,
	31,23,18, 5,21, 9,15,11, 30,17, 8,14,29,13,28,27};
	if (x == 0) return 32;
	x = (x & -x)*0x04D7651F;
	return table[x >> 27];
}
