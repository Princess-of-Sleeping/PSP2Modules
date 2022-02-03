
#include <psp2kern/kernel/dipsw.h>
#include <psp2kern/kernel/sysclib.h>
#include "imports.h"
#include "acmgr.h"
#include "acmgr_has_flag.h"

SceBool SceSblACMgrForKernel_FBA1A256(void){
	return SCE_FALSE;
}

SceBool SceSblACMgrForKernel_1948E9DB(void){
	return SCE_FALSE;
}

typedef struct SceSblACMgrMediaType {
	const SceChar8 *drive;
	SceSize drive_length;
	const SceChar8 *path;
	SceSize path_length;
	SceUInt32 mediaType;
} SceSblACMgrMediaType;

const SceSblACMgrMediaType mediaType[0x1E] = {
	{
		.drive = (const SceChar8 [0x3]){
		~('s'), ~('d'), 0
		},
		.drive_length = 0x2,
		.path = (const SceChar8 [0x2]){
		~(':'), 0
		},
		.path_length = 0x1,
		.mediaType = 0x1
	},
	{
		.drive = (const SceChar8 [0x3]){
		~('o'), ~('s'), 0
		},
		.drive_length = 0x2,
		.path = (const SceChar8 [0x2]){
		~(':'), 0
		},
		.path_length = 0x1,
		.mediaType = 0x2
	},
	{
		.drive = (const SceChar8 [0x3]){
		~('v'), ~('s'), 0
		},
		.drive_length = 0x2,
		.path = (const SceChar8 [0x2]){
		~(':'), 0
		},
		.path_length = 0x1,
		.mediaType = 0x3
	},
	{
		.drive = (const SceChar8 [0x3]){
		~('v'), ~('d'), 0
		},
		.drive_length = 0x2,
		.path = (const SceChar8 [0x2]){
		~(':'), 0
		},
		.path_length = 0x1,
		.mediaType = 0x4
	},
	{
		.drive = (const SceChar8 [0x3]){
		~('t'), ~('m'), 0
		},
		.drive_length = 0x2,
		.path = (const SceChar8 [0x2]){
		~(':'), 0
		},
		.path_length = 0x1,
		.mediaType = 0x5
	},
	{
		.drive = (const SceChar8 [0x3]){
		~('u'), ~('r'), 0
		},
		.drive_length = 0x2,
		.path = (const SceChar8 [0x2]){
		~(':'), 0
		},
		.path_length = 0x1,
		.mediaType = 0x6
	},
	{
		.drive = (const SceChar8 [0x5]){
		~('h'), ~('o'), ~('s'), ~('t'), 0
		},
		.drive_length = 0x4,
		.path = (const SceChar8 [0x2]){
		~(':'), 0
		},
		.path_length = 0x1,
		.mediaType = 0x7
	},
	{
		.drive = (const SceChar8 [0x3]){
		~('u'), ~('d'), 0
		},
		.drive_length = 0x2,
		.path = (const SceChar8 [0x2]){
		~(':'), 0
		},
		.path_length = 0x1,
		.mediaType = 0xB
	},
	{
		.drive = (const SceChar8 [0x3]){
		~('u'), ~('x'), 0
		},
		.drive_length = 0x2,
		.path = (const SceChar8 [0x5]){
		~(':'), ~('a'), ~('p'), ~('p'), 0
		},
		.path_length = 0x4,
		.mediaType = 0x17
	},
	{
		.drive = (const SceChar8 [0x3]){
		~('u'), ~('x'), 0
		},
		.drive_length = 0x2,
		.path = (const SceChar8 [0x6]){
		~(':'), ~('/'), ~('a'), ~('p'), ~('p'), 0
		},
		.path_length = 0x5,
		.mediaType = 0x17
	},
	{
		.drive = (const SceChar8 [0x3]){
		~('u'), ~('x'), 0
		},
		.drive_length = 0x2,
		.path = (const SceChar8 [0x7]){
		~(':'), ~('p'), ~('a'), ~('t'), ~('c'), ~('h'), 0
		},
		.path_length = 0x6,
		.mediaType = 0x18
	},
	{
		.drive = (const SceChar8 [0x3]){
		~('u'), ~('x'), 0
		},
		.drive_length = 0x2,
		.path = (const SceChar8 [0x8]){
		~(':'), ~('/'), ~('p'), ~('a'), ~('t'), ~('c'), ~('h'), 0
		},
		.path_length = 0x7,
		.mediaType = 0x18
	},
	{
		.drive = (const SceChar8 [0x3]){
		~('u'), ~('x'), 0
		},
		.drive_length = 0x2,
		.path = (const SceChar8 [0x6]){
		~(':'), ~('d'), ~('a'), ~('t'), ~('a'), 0
		},
		.path_length = 0x5,
		.mediaType = 0x19
	},
	{
		.drive = (const SceChar8 [0x3]){
		~('u'), ~('x'), 0
		},
		.drive_length = 0x2,
		.path = (const SceChar8 [0x7]){
		~(':'), ~('/'), ~('d'), ~('a'), ~('t'), ~('a'), 0
		},
		.path_length = 0x6,
		.mediaType = 0x19
	},
	{
		.drive = (const SceChar8 [0x3]){
		~('u'), ~('x'), 0
		},
		.drive_length = 0x2,
		.path = (const SceChar8 [0x6]){
		~(':'), ~('u'), ~('s'), ~('e'), ~('r'), 0
		},
		.path_length = 0x5,
		.mediaType = 0x0
	},
	{
		.drive = (const SceChar8 [0x3]){
		~('u'), ~('x'), 0
		},
		.drive_length = 0x2,
		.path = (const SceChar8 [0x7]){
		~(':'), ~('/'), ~('u'), ~('s'), ~('e'), ~('r'), 0
		},
		.path_length = 0x6,
		.mediaType = 0x0
	},
	{
		.drive = (const SceChar8 [0x3]){
		~('u'), ~('x'), 0
		},
		.drive_length = 0x2,
		.path = (const SceChar8 [0x2]){
		~(':'), 0
		},
		.path_length = 0x1,
		.mediaType = 0xC
	},
	{
		.drive = (const SceChar8 [0x4]){
		~('g'), ~('r'), ~('o'), 0
		},
		.drive_length = 0x3,
		.path = (const SceChar8 [0x5]){
		~(':'), ~('a'), ~('p'), ~('p'), 0
		},
		.path_length = 0x4,
		.mediaType = 0xD
	},
	{
		.drive = (const SceChar8 [0x4]){
		~('g'), ~('r'), ~('o'), 0
		},
		.drive_length = 0x3,
		.path = (const SceChar8 [0x6]){
		~(':'), ~('/'), ~('a'), ~('p'), ~('p'), 0
		},
		.path_length = 0x5,
		.mediaType = 0xD
	},
	{
		.drive = (const SceChar8 [0x4]){
		~('g'), ~('r'), ~('w'), 0
		},
		.drive_length = 0x3,
		.path = (const SceChar8 [0x7]){
		~(':'), ~('p'), ~('a'), ~('t'), ~('c'), ~('h'), 0
		},
		.path_length = 0x6,
		.mediaType = 0xE
	},
	{
		.drive = (const SceChar8 [0x4]){
		~('g'), ~('r'), ~('w'), 0
		},
		.drive_length = 0x3,
		.path = (const SceChar8 [0x8]){
		~(':'), ~('/'), ~('p'), ~('a'), ~('t'), ~('c'), ~('h'), 0
		},
		.path_length = 0x7,
		.mediaType = 0xE
	},
	{
		.drive = (const SceChar8 [0x3]){
		~('s'), ~('a'), 0
		},
		.drive_length = 0x2,
		.path = (const SceChar8 [0x2]){
		~(':'), 0
		},
		.path_length = 0x1,
		.mediaType = 0xF
	},
	{
		.drive = (const SceChar8 [0x4]){
		~('m'), ~('f'), ~('a'), 0
		},
		.drive_length = 0x3,
		.path = (const SceChar8 [0x2]){
		~(':'), 0
		},
		.path_length = 0x1,
		.mediaType = 0x10
	},
	{
		.drive = (const SceChar8 [0x4]){
		~('m'), ~('f'), ~('b'), 0
		},
		.drive_length = 0x3,
		.path = (const SceChar8 [0x2]){
		~(':'), 0
		},
		.path_length = 0x1,
		.mediaType = 0x11
	},
	{
		.drive = (const SceChar8 [0x4]){
		~('l'), ~('m'), ~('a'), 0
		},
		.drive_length = 0x3,
		.path = (const SceChar8 [0x2]){
		~(':'), 0
		},
		.path_length = 0x1,
		.mediaType = 0x12
	},
	{
		.drive = (const SceChar8 [0x4]){
		~('l'), ~('m'), ~('b'), 0
		},
		.drive_length = 0x3,
		.path = (const SceChar8 [0x2]){
		~(':'), 0
		},
		.path_length = 0x1,
		.mediaType = 0x13
	},
	{
		.drive = (const SceChar8 [0x4]){
		~('l'), ~('m'), ~('c'), 0
		},
		.drive_length = 0x3,
		.path = (const SceChar8 [0x2]){
		~(':'), 0
		},
		.path_length = 0x1,
		.mediaType = 0x14
	},
	{
		.drive = (const SceChar8 [0x4]){
		~('l'), ~('m'), ~('d'), 0
		},
		.drive_length = 0x3,
		.path = (const SceChar8 [0x2]){
		~(':'), 0
		},
		.path_length = 0x1,
		.mediaType = 0x15
	},
	{
		.drive = (const SceChar8 [0x3]){
		~('p'), ~('d'), 0
		},
		.drive_length = 0x2,
		.path = (const SceChar8 [0x2]){
		~(':'), 0
		},
		.path_length = 0x1,
		.mediaType = 0x16
	},
	{
		.drive = (const SceChar8 [0x4]){
		~('u'), ~('m'), ~('a'), 0
		},
		.drive_length = 0x3,
		.path = (const SceChar8 [0x2]){
		~(':'), 0
		},
		.path_length = 0x1,
		.mediaType = 0x1B
	},
};

SceInt32 sceSblACMgrGetMediaType(const SceChar8 *path, SceUInt32 *pMediaType){

	if(mediaType == NULL || path == NULL)
		return 0x800F090E;

	*pMediaType = 0;

	for(int i=0;i<0x1E;i++){

		int ch_pos = 0, is_valid = 1;

		for(int n=0;n<mediaType[i].drive_length;n++){
			is_valid &= (path[ch_pos++] == (SceChar8)(~mediaType[i].drive[n]));
		}

		if(is_valid != 0){

			while((look_ctype_table(path[ch_pos]) & 4) != 0){
				ch_pos++;
			}

			for(int n=0;n<mediaType[i].path_length;n++){
				is_valid &= (path[ch_pos++] == (SceChar8)(~mediaType[i].path[n]));
			}

			if(is_valid != 0){

				*pMediaType = mediaType[i].mediaType;
				return 0;
			}
		}
	}

	return 0x800F0903;
}

typedef struct SceSblACMgrInvPathInfo {
	const SceChar8 *drive;
	SceSize drive_length;
	const SceChar8 *path;
	SceSize path_length;
} SceSblACMgrInvPathInfo;

const SceChar8 inv_strings_sd0[0x5] = {
	~('s'), ~('d'), ~('0'), ~(':'), 0
};

const SceChar8 inv_strings_os0[0x5] = {
	~('o'), ~('s'), ~('0'), ~(':'), 0
};

const SceChar8 inv_strings_vs0[5] = {
	~('v'), ~('s'), ~('0'), ~(':'), 0
};

const SceChar8 inv_strings_vd0[0x5] = {
	~('v'), ~('d'), ~('0'), ~(':'), 0
};

const SceChar8 inv_strings_tm0[0x5] = {
	~('t'), ~('m'), ~('0'), ~(':'), 0
};

const SceChar8 inv_strings_ur0[0x5] = {
	~('u'), ~('r'), ~('0'), ~(':'), 0
};

const SceChar8 inv_strings_ud0[0x5] = {
	~('u'), ~('d'), ~('0'), ~(':'), 0
};

const SceChar8 inv_strings_ux0[0x5] = {
	~('u'), ~('x'), ~('0'), ~(':'), 0
};

const SceChar8 inv_strings_gro0[0x6] = {
	~('g'), ~('r'), ~('o'), ~('0'), ~(':'), 0
};

const SceChar8 inv_strings_grw0[0x6] = {
	~('g'), ~('r'), ~('w'), ~('0'), ~(':'), 0
};

const SceChar8 inv_strings_sa0[0x5] = {
	~('s'), ~('a'), ~('0'), ~(':'), 0
};

const SceChar8 inv_strings_mfa0[0x6] = {
	~('m'), ~('f'), ~('a'), ~('0'), ~(':'), 0
};

const SceChar8 inv_strings_mfb0[0x6] = {
	~('m'), ~('f'), ~('b'), ~('0'), ~(':'), 0
};

const SceChar8 inv_strings_lma0[0x6] = {
	~('l'), ~('m'), ~('a'), ~('0'), ~(':'), 0
};

const SceChar8 inv_strings_lmb0[0x6] = {
	~('l'), ~('m'), ~('b'), ~('0'), ~(':'), 0
};

const SceChar8 inv_strings_lmc0[0x6] = {
	~('l'), ~('m'), ~('c'), ~('0'), ~(':'), 0
};

const SceChar8 inv_strings_lmd0[0x6] = {
	~('l'), ~('m'), ~('d'), ~('0'), ~(':'), 0
};

const SceChar8 inv_strings_pd0[0x5] = {
	~('p'), ~('d'), ~('0'), ~(':'), 0
};

const SceChar8 inv_strings_imc0[0x6] = {
	~('i'), ~('m'), ~('c'), ~('0'), ~(':'), 0
};

const SceChar8 inv_strings_xmc0[0x6] = {
	~('x'), ~('m'), ~('c'), ~('0'), ~(':'), 0
};

const SceChar8 inv_strings_uma0[0x6] = {
	~('u'), ~('m'), ~('a'), ~('0'), ~(':'), 0
};

const SceChar8 inv_strings_host0[0x7] = {
	~('h'), ~('o'), ~('s'), ~('t'), ~('0'), ~(':'), 0
};




const SceChar8 inv_strings_data_external[0xE] = {
	~('d'), ~('a'), ~('t'), ~('a'), ~('/'),
	~('e'), ~('x'), ~('t'), ~('e'), ~('r'), ~('n'), ~('a'), ~('l'), 0
};

const SceChar8 inv_strings_data_external_cert[0x14] = {
	~('d'), ~('a'), ~('t'), ~('a'), ~('/'),
	~('e'), ~('x'), ~('t'), ~('e'), ~('r'), ~('n'), ~('a'), ~('l'), ~('/'),
	~('c'), ~('e'), ~('r'), ~('t'), ~('/'), 0
};

const SceChar8 inv_strings_data_external_webcore[0x17] = {
	~('d'), ~('a'), ~('t'), ~('a'), ~('/'),
	~('e'), ~('x'), ~('t'), ~('e'), ~('r'), ~('n'), ~('a'), ~('l'), ~('/'),
	~('w'), ~('e'), ~('b'), ~('c'), ~('o'), ~('r'), ~('e'), ~('/'), 0
};

const SceChar8 inv_strings_sys_external[0xD] = {
	~('s'), ~('y'), ~('s'), ~('/'), ~('e'), ~('x'), ~('t'), ~('e'), ~('r'), ~('n'), ~('a'), ~('l'), 0
};

const SceSblACMgrInvPathInfo inv_path_vs0_data_external_cert[3] = {
	{
		.drive = inv_strings_vs0,
		.drive_length = 4,
		.path = inv_strings_data_external_cert,
		.path_length = 0x13
	},
	{
		.drive = inv_strings_vs0,
		.drive_length = 4,
		.path = inv_strings_data_external_webcore,
		.path_length = 0x16
	},
	{
		.drive        = NULL,
		.drive_length = 0,
		.path         = NULL,
		.path_length  = 0
	}
};

const SceSblACMgrInvPathInfo inv_path_vs0_data_external[3] = {
	{
		.drive        = inv_strings_vs0,
		.drive_length = 4,
		.path         = inv_strings_data_external,
		.path_length  = 0xD
	},
	{
		.drive        = inv_strings_vs0,
		.drive_length = 4,
		.path         = inv_strings_sys_external,
		.path_length  = 0xC
	},
	{
		.drive        = NULL,
		.drive_length = 0,
		.path         = NULL,
		.path_length  = 0
	}
};

const SceSblACMgrInvPathInfo inv_path_vs0_data_external_cert2[4] = {
	{
		.drive        = inv_strings_vs0,
		.drive_length = 4,
		.path         = inv_strings_data_external_cert,
		.path_length  = 0x13
	},
	{
		.drive        = inv_strings_vs0,
		.drive_length = 4,
		.path         = inv_strings_data_external,
		.path_length  = 0xD
	},
	{
		.drive        = inv_strings_vs0,
		.drive_length = 4,
		.path         = inv_strings_sys_external,
		.path_length  = 0xC
	},
	{
		.drive        = NULL,
		.drive_length = 0,
		.path         = NULL,
		.path_length  = 0
	}
};

typedef struct SceSblVs0UserDriveAllowProgram {
	SceUInt64 authId;
	const SceSblACMgrInvPathInfo *path;
	SceUInt32 padding;
} SceSblVs0UserDriveAllowProgram;

const SceSblVs0UserDriveAllowProgram vs0UserDriveAllowList[9] = {
	{
		.authId = 0x2800000000008003,
		.path = inv_path_vs0_data_external_cert,
		.padding = 0
	},
	{
		.authId = 0x2800000000008005,
		.path = inv_path_vs0_data_external_cert,
		.padding = 0
	},
	{
		.authId = 0x2800C0101CD2000B,
		.path = inv_path_vs0_data_external,
		.padding = 0
	},
	{
		.authId = 0x210000101CD20007,
		.path = inv_path_vs0_data_external,
		.padding = 0
	},
	{
		.authId = 0x210000101CD20008,
		.path = inv_path_vs0_data_external,
		.padding = 0
	},
	{
		.authId = 0x210000101CD2000A,
		.path = inv_path_vs0_data_external,
		.padding = 0
	},
	{
		.authId = 0x210000101CD20009,
		.path = inv_path_vs0_data_external,
		.padding = 0
	},
	{
		.authId = 0x220000101CD2000C,
		.path = inv_path_vs0_data_external_cert2,
		.padding = 0
	},
	{
		.authId = 0x220000101CD2000D,
		.path = inv_path_vs0_data_external_cert2,
		.padding = 0
	}
};

SceBool sceSblACMgrIsVs0UserDriveAccessibleProgram(SceUID pid, const SceChar8 *path){

	const SceChar8 *current_path;
	SceUInt64 authId = 0LL;

	if(SCE_FALSE == sceSblACMgrHasAttributes(pid, 0x81))
		return SCE_TRUE;

	if(0 != sceSblACMgrGetProcessProgramAuthId(pid, &authId))
		return SCE_FALSE;

	if(path == NULL)
        return SCE_FALSE;

	for(int i=0;i<9;i++){
		if(vs0UserDriveAllowList[i].authId == authId){

			const SceSblACMgrInvPathInfo *path_info = vs0UserDriveAllowList[i].path;

			while(path_info->drive != NULL){

				current_path = path;

				int is_valid = 1;

				for(int i=0;i<path_info->drive_length;i++){
					is_valid &= (path_info->drive[i] == (SceChar8)(~(current_path[i])));
				}

				if(is_valid != 0){
					current_path = &current_path[path_info->drive_length];
					if(current_path[0] == '/'){
						current_path = &current_path[1];
					}

					for(int i=0;i<path_info->path_length;i++){
						is_valid &= (path_info->path[i] == (SceChar8)(~(current_path[i])));
					}

					return (is_valid == 0) ? SCE_FALSE : SCE_TRUE;
				}

				path_info = &path_info[1];
			}
		}
	}

    return SCE_FALSE;
}

typedef struct SceSblMediaIoMode {
	const SceChar8 *path;
	SceSize path_length;
	SceUInt32 a3; // maybe SceIoAccessMode
} SceSblMediaIoMode;

const SceSblMediaIoMode mediaAccessibleListWithTAL[0x15] = {
	{
		.path = inv_strings_sd0,
		.path_length = 0x4,
		.a3 = 0x186
	},
	{
		.path = inv_strings_os0,
		.path_length = 0x4,
		.a3 = 0x106
	},
	{
		.path = inv_strings_vs0,
		.path_length = 0x4,
		.a3 = 0x106
	},
	{
		.path = inv_strings_vd0,
		.path_length = 0x4,
		.a3 = 0x106
	},
	{
		.path = inv_strings_tm0,
		.path_length = 0x4,
		.a3 = 0x106
	},
	{
		.path = inv_strings_ur0,
		.path_length = 0x4,
		.a3 = 0x106
	},
	{
		.path = inv_strings_ud0,
		.path_length = 0x4,
		.a3 = 0x106
	},
	{
		.path = inv_strings_ux0,
		.path_length = 0x4,
		.a3 = 0x106
	},
	{
		.path = inv_strings_gro0,
		.path_length = 0x5,
		.a3 = 0x106
	},
	{
		.path = inv_strings_grw0,
		.path_length = 0x5,
		.a3 = 0x106
	},
	{
		.path = inv_strings_sa0,
		.path_length = 0x4,
		.a3 = 0x106
	},
	{
		.path = inv_strings_mfa0,
		.path_length = 0x5,
		.a3 = 0x106
	},
	{
		.path = inv_strings_mfb0,
		.path_length = 0x5,
		.a3 = 0x106
	},
	{
		.path = inv_strings_lma0,
		.path_length = 0x5,
		.a3 = 0x1000000
	},
	{
		.path = inv_strings_lmb0,
		.path_length = 0x5,
		.a3 = 0x106
	},
	{
		.path = inv_strings_lmc0,
		.path_length = 0x5,
		.a3 = 0x106
	},
	{
		.path = inv_strings_lmd0,
		.path_length = 0x5,
		.a3 = 0x106
	},
	{
		.path = inv_strings_pd0,
		.path_length = 0x4,
		.a3 = 0x106
	},
	{
		.path = inv_strings_imc0,
		.path_length = 0x5,
		.a3 = 0x106
	},
	{
		.path = inv_strings_xmc0,
		.path_length = 0x5,
		.a3 = 0x106
	},
	{
		.path = inv_strings_uma0,
		.path_length = 0x5,
		.a3 = 0x106
	}
};

SceBool sceSblACMgrIsMediaAccessibleProgramWithThreadAccessLevel(SceUID pid, const SceChar8 *path){

	int threadAccessLevel, n;

	threadAccessLevel = ksceKernelSysrootGetThreadAccessLevel();

	SceUInt64 authId = 0LL;

	if(0 != sceSblACMgrGetProcessProgramAuthId(pid, &authId))
		return SCE_FALSE;

	if(path == NULL)
		return SCE_FALSE;

	for(int i=0;i<0x15;i++){
		int is_valid = 1;

		n = mediaAccessibleListWithTAL[i].path_length;
		for(int k=0;k<n;k++){
			is_valid &= (path[k] == (SceChar8)(~mediaAccessibleListWithTAL[i].path[k]));
		}

		if(is_valid != 0){
			if(SCE_FALSE != sceSblACMgrHasCapability(pid, 1))
				return SCE_TRUE;

			if(threadAccessLevel == 0x40 || threadAccessLevel == 0x80)
				return SCE_TRUE;

			if(SCE_FALSE != sceSblACMgrHasCapability(pid, 3) && 0 == memcmp(&inv_strings_ux0, mediaAccessibleListWithTAL[i].path, n))
				return SCE_TRUE;

			if((authId == 0x220000101CC73883 || authId == 0x220000101CC60019) && 0 == memcmp(inv_strings_lma0, mediaAccessibleListWithTAL[i].path, n))
				return SCE_TRUE;

			return SCE_FALSE;
		}
	}

	return SCE_FALSE;
}

SceInt32 sceSblACMgrGetMediaMode(SceUID pid, const SceChar8 *path, SceUInt32 *mode){

	if(mode == NULL || path == NULL)
		return 0x800F0916;

	for(int i=0;i<0x15;i++){

		int is_valid = 1;

		for(int k=0;k<mediaAccessibleListWithTAL[i].path_length;k++){
			is_valid &= (path[k] == (SceChar8)(~mediaAccessibleListWithTAL[i].path[k]));
		}

		if(is_valid != 0){

			*mode = mediaAccessibleListWithTAL[i].a3;
			return 0;
		}
	}

	return 0x800F0902;
}

typedef struct SceSblACMgrForKernel_7F294A09_t {
	const SceChar8 *path;
	SceSize path_length;
	SceBool a3[2];
} SceSblACMgrForKernel_7F294A09_t;

const SceSblACMgrForKernel_7F294A09_t SceSblACMgrForKernel_7F294A09_list_for_devmode[4] = {
	{
		.path = inv_strings_ux0,
		.path_length = 4,
		.a3 = {
			1, 1
		}
	},
	{
		.path = inv_strings_sd0,
		.path_length = 4,
		.a3 = {
			1, 1
		}
	},
	{
		.path = inv_strings_pd0,
		.path_length = 4,
		.a3 = {
			0, 0
		}
	},
	{
		.path = inv_strings_host0,
		.path_length = 6,
		.a3 = {
			1, 1
		}
	}
};

const SceSblACMgrForKernel_7F294A09_t SceSblACMgrForKernel_7F294A09_list[4] = {
	{
		.path = inv_strings_ux0,
		.path_length = 4,
		.a3 = {
			0, 0
		}
	},
	{
		.path = inv_strings_sd0,
		.path_length = 4,
		.a3 = {
			0, 0
		}
	},
	{
		.path = inv_strings_pd0,
		.path_length = 4,
		.a3 = {
			0, 0
		}
	},
	{
		.path = inv_strings_host0,
		.path_length = 6,
		.a3 = {
			0, 0
		}
	}
};

// Used by SceAppMgr
SceBool sceSblACMgrIsMediaAccessibleProgram(SceUID pid, const SceChar8 *path){

	SceUInt32 idx;
	const SceSblACMgrForKernel_7F294A09_t *pList;
	SceSelfAuthInfo auth_info;

	if(0 != ksceSysrootGetSelfAuthInfo(pid, &auth_info))
		return SCE_FALSE;

	if(path == NULL)
		return SCE_FALSE;

	SceUInt64 authId = auth_info.program_authority_id & 0xFFF0000000000000;

	if(authId == 0x2100000000000000 || authId == 0x2F00000000000000){

		idx = (auth_info.attribute[0x10] & 0x40) >> 6;
	}else{
		if((auth_info.attribute[0x10] & 0x40) == 0){
			return SCE_TRUE;
		}

		idx = 1;
	}

	if(0 != ksceKernelCheckDipsw(0x9F)){
		pList = SceSblACMgrForKernel_7F294A09_list_for_devmode;
	}else{
		pList = SceSblACMgrForKernel_7F294A09_list;
	}

	for(int i=0;i<4;i++){

		int is_valid = 1;

		for(int n=0;n<pList[i].path_length;n++){
			is_valid &= (path[n] == (SceChar8)(~pList[i].path[n]));
		}

		if(is_valid != 0){
			return pList[i].a3[idx];
		}
	}

	return SCE_FALSE;
}
