
#include <psp2kern/types.h>
#include "intrmgr_internal.h"

SceUInt32 DAT_81008248[4];

ScePVoid DAT_81088280;
ScePVoid DAT_81088284;
ScePVoid DAT_81088288;

// callThreadmgrBeforeIntrHandler
void SceIntrmgrForKernel_772BE54F(void *a1){
	DAT_81088280 = a1;
}

void SceIntrmgrForKernel_4F611A63(void *a1){
	DAT_81088284 = a1;
}

// setupNextContext_intr
void SceIntrmgrForKernel_53DD3BF5(void *a1){
	DAT_81088288 = a1;
}

void SceIntrmgrForKernel_CAAC949E(void *a1){
	for(int i=0;i<4;i++){
		DAT_81008248[i] = ((SceUInt32 *)a1)[i];
	}
}
