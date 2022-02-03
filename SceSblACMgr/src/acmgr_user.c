
#include <psp2kern/kernel/sysmem.h>
#include "acmgr_is_program.h"

SceInt32 _sceSblACMgrIsGameProgramCore(SceBool *result){

	if(result == NULL)
		return 0x800F0916;

	*result = sceSblACMgrIsGameProgram(0);

	return 0;
}

SceInt32 _sceSblACMgrIsGameProgram(SceBool *result){

	int res, res2;
	SceBool game_program_result = 0;

	if(result == NULL){
		res = _sceSblACMgrIsGameProgramCore(result);
	}else{
		res2 = _sceSblACMgrIsGameProgramCore(&game_program_result);

		res = ksceKernelMemcpyToUser(result, &game_program_result, sizeof(*result));
		if(res >= 0){
			res = res2;
		}
	}

	return res;
}
