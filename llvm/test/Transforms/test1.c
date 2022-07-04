// RUN: /home/syrmia/Desktop/llvm/llvm-project/build/bin/clang -target mips-linux-gnu-gcc %s -O3 -Xclang -disable-llvm-passes -c -emit-llvm -S -o - | /home/syrmia/Desktop/llvm/llvm-project/build/bin/FileCheck %s


// CHECK: entry:
// CHECK: %x.addr = alloca i32, align 4
// CHECK: store i32 %x, ptr %x.addr, align 4, !tbaa !4
// CHECK: %0 = load i32, ptr %x.addr, align 4, !tbaa !4
// CHECK: %1 = load i32, ptr %x.addr, align 4, !tbaa !4
// CHECK: %sub = sub i32 0, %1
// CHECK: %and = and i32 %0, %sub
// CHECK: %mul = mul i32 %and, 72416175
// CHECK: store i32 %mul, ptr %x.addr, align 4, !tbaa !4
// CHECK: %2 = load i32, ptr %x.addr, align 4, !tbaa !4
// CHECK: %shr = lshr i32 %2, 26
// CHECK: %arrayidx = getelementptr inbounds [64 x i8], ptr @ctz.table, i32 0, i32 %shr
// CHECK: %3 = load i8, ptr %arrayidx, align 1, !tbaa !8
// CHECK: %conv = sext i8 %3 to i32
// CHECK: ret i32 %conv

int ctz(unsigned x) {

	static char table[64] =
	{32, 0, 1,12, 2, 6, -1,13, 3, -1, 7, -1, -1, -1, -1,14,
	10, 4, -1, -1, 8, -1, -1,25, -1, -1, -1, -1, -1,21,27,15,
	31,11, 5, -1, -1, -1, -1, -1, 9, -1, -1,24, -1, -1,20,26,
	30, -1, -1, -1, -1,23, -1,19, 29, -1,22,18,28,17,16, -1};
	
	x = (x & -x)*0x0450FBAF;
	
	return table[x >> 26];
}
