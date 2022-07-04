// RUN: /home/syrmia/Desktop/llvm/llvm-project/build/bin/clang -target mips-linux-gnu-gcc %s -O3 -Xclang -disable-llvm-passes -c -emit-llvm -S -o - | /home/syrmia/Desktop/llvm/llvm-project/build/bin/FileCheck %s

// CHECK: entry:
// CHECK: %retval = alloca i32, align 4
// CHECK: %x.addr = alloca i32, align 4
// CHECK: store i32 %x, ptr %x.addr, align 4, !tbaa !4
// CHECK: %0 = load i32, ptr %x.addr, align 4, !tbaa !4
// CHECK: %cmp = icmp eq i32 %0, 0
// CHECK: br i1 %cmp, label %if.then, label %if.end
// CHECK: if.then:
// CHECK: store i32 32, ptr %retval, align 4
// CHECK: br label %return
// CHECK: if.end:
// CHECK: %1 = load i32, ptr %x.addr, align 4, !tbaa !4
// CHECK: %2 = load i32, ptr %x.addr, align 4, !tbaa !4
// CHECK: %sub = sub i32 0, %2
// CHECK: %and = and i32 %1, %sub
// CHECK: %mul = mul i32 %and, 81224991
// CHECK: store i32 %mul, ptr %x.addr, align 4, !tbaa !4
// CHECK: %3 = load i32, ptr %x.addr, align 4, !tbaa !4
// CHECK: %shr = lshr i32 %3, 27
// CHECK: %arrayidx = getelementptr inbounds [32 x i8], ptr @cttz.table, i32 0, i32 %shr
// CHECK: %4 = load i8, ptr %arrayidx, align 1, !tbaa !8
// CHECK: %conv = sext i8 %4 to i32
// CHECK: store i32 %conv, ptr %retval, align 4
// CHECK: br label %return
// CHECK: return:
// CHECK: %5 = load i32, ptr %retval, align 4
// CHECK: ret i32 %5

int cttz(unsigned x) {
	static char table[32] =
	{ 0, 1, 2,24, 3,19, 6,25, 22, 4,20,10,16, 7,12,26,
	31,23,18, 5,21, 9,15,11, 30,17, 8,14,29,13,28,27};
	if (x == 0) return 32;
	x = (x & -x)*0x04D7651F;
	return table[x >> 27];
}

