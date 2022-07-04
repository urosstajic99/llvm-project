// RUN: /home/syrmia/Desktop/llvm/llvm-project/build/bin/clang %s -O3 -c -emit-llvm -S -o - | /home/syrmia/Desktop/llvm/llvm-project/build/bin/FileCheck %s

// CHECK: @llvm.cttz.i32
unsigned int foo(unsigned int num) {
    return __builtin_ctz(num);
}
