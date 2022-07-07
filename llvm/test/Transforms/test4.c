// RUN: /home/syrmia/Desktop/llvm/llvm-project/build/bin/clang -target mips64-r6-linux-gnu -mabi=64 -Xclang -no-opaque-pointers %s -O2 -c -emit-llvm -S -o - | /home/syrmia/Desktop/llvm/llvm-project/build/bin/opt -aggressive-instcombine -mtriple mips64-r6-linux-gnu -S -o - | /home/syrmia/Desktop/llvm/llvm-project/build/bin/FileCheck %s

// CHECK: @llvm.cttz.i32
unsigned int foo(unsigned int num) {
    return __builtin_ctz(num);
}
