// RUN: /home/syrmia/Desktop/llvm/llvm-project/build/bin/clang -target mips64-r6-linux-gnu -mabi=64 %s -Xclang -no-opaque-pointers -O2 -emit-llvm -S -o - | /home/syrmia/Desktop/llvm/llvm-project/build/bin/opt -aggressive-instcombine -mtriple mips64-r6-linux-gnu -S -o - | /home/syrmia/Desktop/llvm/llvm-project/build/bin/FileCheck %s

// CHECK: %0 = call i32 @llvm.cttz.i32(i32 %x, i1 false)
// CHECK: %1 = trunc i32 %0 to i16
// CHECK: %conv = sext i16 %1 to i32
// CHECK: ret i32 %conv



#define u 0

int ctz(unsigned x) {

	static short table[64] =
	{32, 0, 1,12, 2, 6, u,13, 3, u, 7, u, u, u, u,14,
	10, 4, u, u, 8, u, u,25, u, u, u, u, u,21,27,15,
	31,11, 5, u, u, u, u, u, 9, u, u,24, u, u,20,26,
	30, u, u, u, u,23, u,19, 29, u,22,18,28,17,16, u};
	
	x = (x & -x)*0x0450FBAF;
	
	return table[x >> 26];
}
