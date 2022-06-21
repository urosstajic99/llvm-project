#include "llvm/Transforms/Utils/MojPass.h"
#include <string>
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/IntrinsicInst.h"

using namespace llvm;

PreservedAnalyses MojPassClass::run(Function& F, FunctionAnalysisManager& AM){
	llvm::dbgs()<<"definisana je funckija sa imenom " + F.getName()<<"\n";
	int br=0;
	//errs()<<"hahahah\n";
	for(Function::iterator funkc = F.begin(), e=F.end();funkc!=e;++funkc){
	
		for(BasicBlock::iterator i = funkc->begin(), h=funkc->end();i!=h;++i){
			if(isa<CallInst>(i)){
			  br++;
			  continue;
			}
			if(isa<DbgInfoIntrinsic>(i)){
			  br++;
  			  continue;
			}
			if(i->getDebugLoc()){
				i->setDebugLoc(nullptr);
				br++;
			}
			
		}
	
	}
	errs()<<"broj instrukcija "<<br<<"\n";
	return PreservedAnalyses::all();

}
