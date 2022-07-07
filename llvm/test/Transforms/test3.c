// RUN: /home/syrmia/Desktop/llvm/llvm-project/build/bin/clang -target mips64-r6-linux-gnu -mabi=64 -Xclang -no-opaque-pointers %s -O2 -c -emit-llvm -S -o - | /home/syrmia/Desktop/llvm/llvm-project/build/bin/opt -aggressive-instcombine -mtriple mips64-r6-linux-gnu -S -o - | /home/syrmia/Desktop/llvm/llvm-project/build/bin/FileCheck %s

// CHECK: entry:
// CHECK: %sub = sub i32 0, %x
// CHECK: %and = and i32 %sub, %x
// CHECK: %rem = urem i32 %and, 37
// CHECK: %idxprom = zext i32 %rem to i64
// CHECK: %arrayidx = getelementptr inbounds [37 x i8], [37 x i8]* @ctz.table, i64 0, i64 %idxprom
// CHECK: %0 = load i8, i8* %arrayidx, align 1, !tbaa !4
// CHECK: %conv = sext i8 %0 to i32
// CHECK: ret i32 %conv

int ctz(unsigned x) {
	static char table[37] = {32, 0, 1, 26, 2, 23, 27,
	-1, 3, 16, 24, 30, 28, 11, -1, 13, 4,
	7, 17, -1, 25, 22, 31, 15, 29, 10, 12,
	6, -1, 21, 14, 9, 5, 20, 8, 19, 18};
	x = (x & -x)%37;
	return table[x];
}
