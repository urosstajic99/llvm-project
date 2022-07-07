// RUN: /home/syrmia/Desktop/llvm/llvm-project/build/bin/clang -target mips64-r6-linux-gnu -mabi=64 -Xclang -no-opaque-pointers  %s -O2 -c -emit-llvm -S -o - | /home/syrmia/Desktop/llvm/llvm-project/build/bin/opt -aggressive-instcombine -mtriple mips64-r6-linux-gnu -S -o - | /home/syrmia/Desktop/llvm/llvm-project/build/bin/FileCheck %s


// CHECK: entry:
// CHECK: %sub = sub i32 0, %x
// CHECK: %and = and i32 %sub, %x
// CHECK: %mul = mul i32 %and, 72416175
// CHECK: %shr = lshr i32 %mul, 26
// CHECK: %idxprom = zext i32 %shr to i64
// CHECK: %arrayidx = getelementptr inbounds [64 x i16], [64 x i16]* @ctz.table, i64 0, i64 %idxprom
// CHECK: %0 = load i16, i16* %arrayidx, align 2, !tbaa !4
// CHECK: %conv = sext i16 %0 to i32
// CHECK: ret i32 %conv

#define u 0

int ctz(unsigned x) {

	static char table[64] =
	{32, 0, 1,12, 2, 6, u,13, 3, u, 7, u, u, u, u,14,
	10, 4, u, u, 8, u, u,25, u, u, u, u, u,21,27,15,
	31,11, 5, u, u, u, u, u, 9, u, u,24, u, u,20,26,
	30, u, u, u, u,23, u,19, 29, u,22,18,28,17,16, u};
	
	x = (x & -x)*0x0450FBAF;
	
	return table[x >> 26];
}
