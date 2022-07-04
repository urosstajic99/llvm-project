// RUN: /home/syrmia/Desktop/llvm/llvm-project/build/bin/clang -target mips-linux-gnu-gcc %s -O3 -Xclang -disable-llvm-passes -c -emit-llvm -S -o - | /home/syrmia/Desktop/llvm/llvm-project/build/bin/FileCheck %s

// CHECK: entry:
// CHECK: %x.addr = alloca i32, align 4
// CHECK: store i32 %x, ptr %x.addr, align 4, !tbaa !4
// CHECK: %0 = load i32, ptr %x.addr, align 4, !tbaa !4
// CHECK: %1 = load i32, ptr %x.addr, align 4, !tbaa !4
// CHECK: %sub = sub i32 0, %1
// CHECK: %and = and i32 %0, %sub
// CHECK: %rem = urem i32 %and, 37
// CHECK: store i32 %rem, ptr %x.addr, align 4, !tbaa !4
// CHECK: %2 = load i32, ptr %x.addr, align 4, !tbaa !4
// CHECK: %arrayidx = getelementptr inbounds [37 x i8], ptr @ctz.table, i32 0, i32 %2
// CHECK: %3 = load i8, ptr %arrayidx, align 1, !tbaa !8
// CHECK: %conv = sext i8 %3 to i32
// CHECK: ret i32 %conv


int ctz(unsigned x) {
	static char table[37] = {32, 0, 1, 26, 2, 23, 27,
	-1, 3, 16, 24, 30, 28, 11, -1, 13, 4,
	7, 17, -1, 25, 22, 31, 15, 29, 10, 12,
	6, -1, 21, 14, 9, 5, 20, 8, 19, 18};
	x = (x & -x)%37;
	return table[x];
}
