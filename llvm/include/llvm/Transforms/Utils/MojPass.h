#ifndef LLVM_TRANSFORMS_UTILS_MOJPASS_H
#define LLVM_TRANSFORMS_UTILS_MOJPASS_H

#include "llvm/IR/PassManager.h"

namespace llvm{

class MojPassClass : public PassInfoMixin<MojPassClass> {
public:
	PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);

};

}

#endif
