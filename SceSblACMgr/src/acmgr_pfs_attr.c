
#include "imports.h"
#include "acmgr_pfs_attr.h"

typedef struct ScePfsACFSAttr_t {
	SceUInt32 mode;
	union {
		SceUInt32 attr;
		SceUInt16 attr_as_16;
	};
} ScePfsACFSAttr_t;

typedef struct SceSblACMgrForKernel_75AAF981_type6_t {
	SceUInt16 attr;
	SceUInt16 padding;
	SceUInt32 a2[2];
} SceSblACMgrForKernel_75AAF981_type6_t;

SceInt32 sceSblACMgrGetProcessAuthInfo(SceUID pid, const SceSblACAttribute *attr, SceSelfAuthInfo *pAuthInfo){

	if(0 != ksceSysrootGetSelfAuthInfo(pid, pAuthInfo))
		return 0x800F0916;

	if(attr != NULL && attr->data != NULL){
		return 0;
	}

	return 0x800F0916;
}

SceUInt32 sce_acmgr_get_attr_index(const SceSblACAttribute *attr){

	if(attr->length == 1 || attr->length == 2){
		return *(attr->data) & 7;
	}

	return 0;
}

const SceUInt32 SceSblACMgrForKernel_11C9158B_basic_list[8] = {
	0x186,
	0x106,
	0x186,
	0x106,
	0x106,
	0x104,
	0x6,
	0x4,
};

const ScePfsACFSAttr_t SceSblACMgrForKernel_11C9158B_cmd6_list[6] = {
	{
		.mode = 0x186,
		.attr = 0
	},
	{
		.mode = 0x10106,
		.attr = 1
	},
	{
		.mode = 0x106,
		.attr = 4
	},
	{
		.mode = 0x104,
		.attr = 5
	},
	{
		.mode = 0x6,
		.attr = 6
	},
	{
		.mode = 0x4,
		.attr = 7
	}
};

int sce_acmgr_set_fsmode_by_attr_basic(const SceSblACAttribute *attr, SceUInt32 *mode){

	if((attr->cmd - 1) < 5){
		*mode = SceSblACMgrForKernel_11C9158B_basic_list[sce_acmgr_get_attr_index(attr) & 7];
		return 0;
	}

	return 0x800F0916;
}

int sce_acmgr_set_fsmode_by_attr_cmd6(const SceSblACAttribute *attr, SceUInt32 *mode){

	SceUInt32 value, raw_value;

	if(attr->length == 1){
		raw_value = attr->data[0];
		value = raw_value & 7;
	  }else if(attr->length == 2){
		raw_value = *(SceUInt16 *)(attr->data);
		value = raw_value & 7;
	}else{
		value = 0;
		raw_value = value;
	}

	for(int i=0;i<6;i++){
		if(SceSblACMgrForKernel_11C9158B_cmd6_list[i].attr_as_16 == value){

			value = SceSblACMgrForKernel_11C9158B_cmd6_list[i].mode & 0xfffeffff;

			if((raw_value & 0x4000) != 0){
				value = value | 0x100000;
			}

			if((raw_value & 0x2000) != 0){
				value = value | 0x200000;
			}

			if((raw_value & 0x8000) != 0){
				value = value | 0x1000;
			}else{
				value = value | 0x2000;
			}

			*mode = value;

			return 0;
		}
	}

	return 0x800F0916;
}

int sce_acmgr_set_fsmode_by_attr_cmd7(const SceSblACAttribute *attr, SceUInt32 *mode){
	return 0x800F0925;
}

int sce_acmgr_set_fsmode_by_attr_cmd8(const SceSblACAttribute *attr, SceUInt32 *mode){
	return 0x800F0925;
}

SceInt32 sceSblACMgrSetFSModeByAttribute(const SceSblACAttribute *attr, SceUInt32 *mode){

	int res;

	if(attr == NULL || attr->data == NULL || mode == NULL)
		return 0x800F0916;

	switch(attr->cmd){
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		res = sce_acmgr_set_fsmode_by_attr_basic(attr, mode);
		break;
	case 6:
		res = sce_acmgr_set_fsmode_by_attr_cmd6(attr, mode);
		break;
	case 7:
		res = sce_acmgr_set_fsmode_by_attr_cmd7(attr, mode);
		break;
	case 8:
		res = sce_acmgr_set_fsmode_by_attr_cmd8(attr, mode);
		break;
	default:
		res = 0x800F0916;
		break;
	}

	return res;
}

SceInt32 SceSblACMgrForKernel_1B160234_basic(const SceSelfAuthInfo *auth_info, SceSblACAttribute *dst, const SceSblACAttribute *src, SceUInt32 a4){

	SceUInt16 v;

	if(src == NULL || src->data == NULL || (src->cmd - 1) < 5 || a4 != 0x10000)
		return 0x800F0916;

	if(src->length == 1){
		v = src->data[0];
	}else if(src->length == 2){
		v = *(SceUInt16 *)(src->data);
	}else{
		return 0x800F0916;
	}

	if(v != 0xF)
		return 0x800F0916;

	SceUInt64 authId = auth_info->program_authority_id;

	if(authId == 0x280000000000001E || authId == 0x220000101cc73883){
		v = 0;
	}else if(authId == 0x2800000000000010 || authId == 0x2800000000000031){
		if((auth_info->capability[0x10] & 0x20) != 0){
			v = 0;
		}else{
			v = 4;
		}
	}

	if (dst->length == 1) {
		dst->data[0] = v;
	}else if (dst->length == 2) {
		dst->data[0] = v;
		dst->data[1] = 0;
	}

	return 0;
}

SceInt32 SceSblACMgrForKernel_1B160234(SceUID pid, SceSblACAttribute *dst, SceSblACAttribute *src, SceUInt32 a4){

	SceInt32 res;
	SceSelfAuthInfo auth_info;

	res = sceSblACMgrGetProcessAuthInfo(pid, dst, &auth_info);
	if(0 != res)
		return res;

	if((src == NULL) || (src->data == NULL))
		return 0x800F0916;

	switch(dst->cmd){
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		res = SceSblACMgrForKernel_1B160234_basic(&auth_info, dst, src, a4);
		break;
	case 6:
	case 7:
	case 8:
		res = 0x800F0925;
		break;
	default:
		res = 0x800F0916;
		break;
	}

	return res;
}

const ScePfsACFSAttr_t fsattr_list_for_system[10] = {
	{
		.mode = 0x10186,
		.attr = 0
	},
	{
		.mode = 0x186,
		.attr = 0
	},
	{
		.mode = 0x10106,
		.attr = 1
	},
	{
		.mode = 0x106,
		.attr = 4
	},
	{
		.mode = 0x10104,
		.attr = 5
	},
	{
		.mode = 0x104,
		.attr = 5
	},
	{
		.mode = 0x10006,
		.attr = 6
	},
	{
		.mode = 0x6,
		.attr = 6
	},
	{
		.mode = 0x10004,
		.attr = 7
	},
	{
		.mode = 0x4,
		.attr = 7
	}
};

const ScePfsACFSAttr_t fsattr_list_for_user[2] = {
	{
		.mode = 0x180,
		.attr = 0
	},
	{
		.mode = 0x100,
		.attr = 1
	}
};

void sce_acmgr_write_raw_attr(SceSblACAttribute *attr, SceUInt32 a2){
	if(attr->length == 1){
		attr->data[0] = (a2 & 7) | (attr->data[0] & 0xf8);
	}else if(attr->length == 2){
		SceUInt16 v = *(SceUInt16 *)(attr->data);
		attr->data[0] = (a2 & 7) | (v & 0xf8);
		attr->data[1] = (SceUInt8)((v & 0xfffffff8) >> 8);
	}
}

int sce_acmgr_write_attr_by_thread_access_level(SceSblACAttribute *attr, SceUInt32 a2){

	SceUInt32 v, tal;

	v = sce_acmgr_get_attr_index(attr);

	tal = ksceKernelSysrootGetThreadAccessLevel();

	if(((v | a2) & 4) != 0 && (tal != 0x80 && tal != 0x40)){
		return 0x8001000D;
	}

	sce_acmgr_write_raw_attr(attr, a2);

	return 0;
}

int sce_acmgr_write_attr_by_program_access_level(const SceSelfAuthInfo *auth_info, void **ppData, SceUInt32 *pLength, SceUInt32 a4){

	SceUInt32 access_level = *(SceUInt32 *)(&auth_info->capability[0]);

	if(((auth_info == (void *)-0x10) || (access_level != 0x40 && access_level != 0x80)) && (a4 & 4) != 0){
		return 0x8001000D;
	}

	if(*pLength == 1){
		*(SceChar8 *)(*ppData) = (SceChar8)a4;
	}else if(*pLength == 2){
		*(SceUInt16 *)(*ppData) = (SceUInt16)a4;
	}

	return 0;
}

int sce_acmgr_set_fsattr_by_mode_basic(const SceSelfAuthInfo *auth_info, SceSblACAttribute *dst, SceUInt32 mode){

	SceUInt32 tal, v, ctr;
	const ScePfsACFSAttr_t *pList;

	if(mode == 0x1000000){
		if(dst->length == 1){
			dst->data[0] = 0xF;
		}else if(dst->length == 2){
			dst->data[0] = 0xF;
			dst->data[1] = 0;
		}
		return 0;
	}

	tal = ksceKernelSysrootGetThreadAccessLevel();
	if(0x40 == tal || 0x80 == tal){
		v = mode & 0x10186;
		pList = fsattr_list_for_system;
		ctr = 10;
	}else if(0x10 == tal || 0x20 == tal){
		v = mode & 0x180;
		pList = fsattr_list_for_user;
		ctr = 2;
	}else{
		return 0x80010016;
	}

	for(int i=0;i<ctr;i++){
		if(pList[i].mode == v){
			return sce_acmgr_write_attr_by_thread_access_level(dst, pList[i].attr);
	    }
	}

	return 0x80010016;
}

const ScePfsACFSAttr_t sce_acmgr_set_fsattr_by_mode_cmd6_list_for_system[6] = {
	{
		.mode = 0x186,
		.attr = 0
	},
	{
		.mode = 0x10106,
		.attr = 1
	},
	{
		.mode = 0x106,
		.attr = 4
	},
	{
		.mode = 0x104,
		.attr = 5
	},
	{
		.mode = 0x6,
		.attr = 6
	},
	{
		.mode = 0x4,
		.attr = 7
	}
};

const ScePfsACFSAttr_t sce_acmgr_set_fsattr_by_mode_cmd6_list_for_user[2] = {
	{
		.mode = 0x180,
		.attr = 6
	},
	{
		.mode = 0x100,
		.attr = 1
	}
};

int sce_acmgr_set_fsattr_by_mode_cmd6(const SceSelfAuthInfo *auth_info, SceSblACAttribute *dst, SceUInt32 mode){

	const ScePfsACFSAttr_t *pList;
	SceUInt32 ctr, privilege, mask, v;

	if(auth_info == (void *)0xfffffff0){
		return 0x800f0928;
	}

	privilege = *(SceUInt32 *)(&auth_info->capability[0]);

	if(0x20 == privilege || 0x10 == privilege){
		mask = mode & 0xff0fffff;
	}else if(0x80 == privilege || 0x40 == privilege){
		mask = mode & 0xff3fffff;
	}else{
		return 0x800f0928;
	}

	if(mode != mask){
		return 0x80010016;
	}

	if(0x10 == privilege || 0x20 == privilege){
		mask = 0x180;
		pList = sce_acmgr_set_fsattr_by_mode_cmd6_list_for_user;
		ctr = 2;
	}else if(0x40 == privilege || 0x80 == privilege){
		mask = 0x10186;
		pList = sce_acmgr_set_fsattr_by_mode_cmd6_list_for_system;
		ctr = 6;
	}else{
		return 0x800F0928;
	}

	for(int i=0;i<ctr;i++){
		if(pList[i].mode == (mask & mode)){
			v = pList[i].attr_as_16;

			if((mode & 0x100000) != 0){
				v = v | 0x4000;
			}
			if((mode & 0x200000) != 0){
				v = v | 0x2000;
			}

			return sce_acmgr_write_attr_by_program_access_level(auth_info, (void **)&(dst->data), &(dst->length), v);
		}
	}

	return 0x800F0902;
}

int sce_acmgr_set_fsattr_by_mode_cmd7(SceSelfAuthInfo *auth_info, SceSblACAttribute *dst, SceUInt32 mode){
	return 0x800f0925;
}

int sce_acmgr_set_fsattr_by_mode_cmd8(SceSelfAuthInfo *auth_info, SceSblACAttribute *dst, SceUInt32 mode){
	return 0x800f0925;
}

SceInt32 sceSblACMgrSetFSAttributeByMode(SceUID pid, SceSblACAttribute *dst, SceUInt32 mode){

	int res;
	SceSelfAuthInfo auth_info;

	res = sceSblACMgrGetProcessAuthInfo(pid, dst, &auth_info);
	if(0 != res)
		return res;

	switch(dst->cmd){
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		res = sce_acmgr_set_fsattr_by_mode_basic(&auth_info, dst, mode);
		break;
    case 6:
		res = sce_acmgr_set_fsattr_by_mode_cmd6(&auth_info, dst, mode);
		break;
    case 7:
		res = sce_acmgr_set_fsattr_by_mode_cmd7(&auth_info, dst, mode);
		break;
	case 8:
		res = sce_acmgr_set_fsattr_by_mode_cmd8(&auth_info, dst, mode);
		break;
	default:
		res = 0x800F0916;
		break;
	}

	return res;
}

int sce_acmgr_get_fsattr_by_tal_basic(SceUInt32 mode, int threadAccessLevel, SceSblACAttribute *dst){

	SceUInt32 ctr;
	const ScePfsACFSAttr_t *pList;

	if(0x40 == threadAccessLevel || 0x80 == threadAccessLevel){
		ctr = 10;
		pList = fsattr_list_for_system;
	}else if(0x10 == threadAccessLevel || 0x20 == threadAccessLevel){
		ctr = 2;
		pList = fsattr_list_for_user;
	}else{
		return 0x800F0916;
	}

	for(int i=0;i<ctr;i++){
		if(pList[i].mode == mode){
			sce_acmgr_write_raw_attr(dst, pList[i].attr);
			return 0;
		}
	}

	return 0x80010002;
}

SceInt32 sceSblACMgrGetFSAttributeByThreadAccessLevel(SceUInt32 mode, int threadAccessLevel, SceSblACAttribute *dst){

	if(dst == NULL || dst->data == NULL)
		return 0x800F0916;

	switch(dst->cmd){
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		return sce_acmgr_get_fsattr_by_tal_basic(mode, threadAccessLevel, dst);
	case 6:
	case 7:
	case 8:
		return 0x800F0925;
	default:
		return 0x800F0916;
	}
}

const SceUInt32 SceSblACMgrForKernel_75AAF981_basic_list_for_user[8] = {
	0xB,
	0x9,
	0xB,
	0x9,
	0x1,
	0x1,
	0x0,
	0x0
};

const SceUInt32 SceSblACMgrForKernel_75AAF981_basic_list_for_system[8] = {
	0xB,
	0xB,
	0xB,
	0xB,
	0xB,
	0x9,
	0xB,
	0x9
};

int SceSblACMgrForKernel_75AAF981_basic(const SceSelfAuthInfo *auth_info, const SceSblACAttribute *src, int a3){

	SceUInt32 tal;
	const SceUInt32 *pList;

	tal = ksceKernelSysrootGetThreadAccessLevel();
	if(0x10 == tal || 0x20 == tal){
		pList = SceSblACMgrForKernel_75AAF981_basic_list_for_user;
	}else if(0x40 == tal || 0x80 == tal){
		pList = SceSblACMgrForKernel_75AAF981_basic_list_for_system;
	}else{
		return 0x8001000D;
	}

	switch(src->cmd){
	case 1:
	case 2:
	case 3:
		if(src->length == 1){
			return ((pList[src->data[0] & 7] & a3) == a3) ? 0 : 0x8001000D;
		}
		break;
	case 4:
	case 5:
		if(src->length == 2){
			return ((pList[*(SceUInt16 *)(src->data) & 7] & a3) == a3) ? 0 : 0x8001000D;
		}
		break;
	default:
		break;
	}

	return 0x800F0916;
}

const SceSblACMgrForKernel_75AAF981_type6_t SceSblACMgrForKernel_75AAF981_cmd6_list[6] = {
	{
		.attr = 0,
		.padding = 0,
		.a2 = {
			0xB, 0xB
		}
	},
	{
		.attr = 1,
		.padding = 0,
		.a2 = {
			0xB, 0x9
		}
	},
	{
		.attr = 4,
		.padding = 0,
		.a2 = {
			0xB, 0x1
		}
	},
	{
		.attr = 5,
		.padding = 0,
		.a2 = {
			0x9, 0x1
		}
	},
	{
		.attr = 6,
		.padding = 0,
		.a2 = {
			0xB, 0x0
		}
	},
	{
		.attr = 7,
		.padding = 0,
		.a2 = {
			0x9, 0x0
		}
	}
};

int SceSblACMgrForKernel_75AAF981_cmd6(const SceSelfAuthInfo *auth_info, const SceSblACAttribute *src, int a3){

	SceUInt32 v, tal, idx;

	tal = ksceKernelSysrootGetThreadAccessLevel();

	if(0x40 == tal || 0x80 == tal){
		idx = 1;
	}else if(0x20 == tal || 0x10 == tal){
		idx = 0;
	}else{
		return 0x800F0928;
	}

	if(src->length == 1){
		v = src->data[0] & 7;
	}else if(src->length == 2){
		v = *(SceUInt16 *)(src->data) & 7;
	}else {
		v = 0;
	}

	for(int i=0;i<6;i++){
		if(SceSblACMgrForKernel_75AAF981_cmd6_list[i].attr == v){
			return ((SceSblACMgrForKernel_75AAF981_cmd6_list[i].a2[idx] & a3) == a3) ? 0 : 0x8001000D;
		}
	}

	return 0x8001000D;
}

int SceSblACMgrForKernel_75AAF981_cmd7(const SceSelfAuthInfo *auth_info, const SceSblACAttribute *src, SceUInt32 a3){
	return 0x800F0925;
}

int SceSblACMgrForKernel_75AAF981_cmd8(const SceSelfAuthInfo *auth_info, const SceSblACAttribute *src, int a3){

	SceUInt32 mask, v;

	if(src == NULL || auth_info == NULL)
		return 0x800F0916;

	switch(a3){
	case 1:
		mask = 1;
		break;
	case 2:
		mask = 2;
		break;
	case 3:
		mask = 3;
		break;
	case 4:
		mask = 4;
		break;
	default:
		return 0x800F0916;
	}

	if(src->length == 6){

		SceUInt64 authId = auth_info->program_authority_id;

		if((0x80 == ksceKernelSysrootGetThreadAccessLevel()) || ((authId & 0xFFF8000000000000) == 0x2808000000000000)){
			v = 0xF;
		}else if((authId & 0xFFFFFFFFFFFFFFF0) == 0x2808000000000000 || authId == 0x2800000000007001 || authId == 0x2800000000000015){
			v = src->data[0] & 0xF;
		}else if((authId & 0xFFFF000000000000) == 0x2801000000000000 || authId == 0x2800800000000022 || authId == 0x2800800000000018){
			v = src->data[1] & 0xF;
		}else if((authId & 0xFFFFFFFFFFFF0000) == 0x2800000000000000){
			v = (src->data[0] >> 4) & 0xF;
		}else{
			v = 0;
		}

		if((v & mask) == mask){
			return 0;
		}
	}

	return 0x800F090D;
}

SceInt32 SceSblACMgrForKernel_75AAF981(SceUID pid, const SceSblACAttribute *src, SceUInt32 a3){

	int res;
	SceSelfAuthInfo auth_info;

	res = sceSblACMgrGetProcessAuthInfo(pid, src, &auth_info);
	if(res != 0)
		return res;

	switch(src->cmd){
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		res = SceSblACMgrForKernel_75AAF981_basic(&auth_info, src, a3);
		break;
	case 6:
		res = SceSblACMgrForKernel_75AAF981_cmd6(&auth_info, src, a3);
		break;
	case 7:
		res = SceSblACMgrForKernel_75AAF981_cmd7(&auth_info, src, a3);
		break;
	case 8:
		res = SceSblACMgrForKernel_75AAF981_cmd8(&auth_info, src, a3);
		break;
	default:
		res = 0x800F0916;
		break;
	}

	return res;
}

int SceSblACMgrForKernel_CCDBB74D_basic(const SceSelfAuthInfo *auth_info, SceSblACAttribute *dst, const SceSblACAttribute *src){
  
	if(src != NULL && src->data != NULL && (src->cmd - 1) < 5){
		return sce_acmgr_write_attr_by_thread_access_level(dst, sce_acmgr_get_attr_index(src));
	}

	return 0x800F0916;
}

int SceSblACMgrForKernel_CCDBB74D_cmd6(const SceSelfAuthInfo *auth_info, SceSblACAttribute *dst, const SceSblACAttribute *src){

	SceUInt32 v;

	if(src->length == 1){
		v = (SceUInt32)(*(src->data));
	}else if(src->length == 2){
		v = (SceUInt32)(*(SceUInt16 *)(src->data));
	}else{
		v = 0;
	}

	return sce_acmgr_write_attr_by_program_access_level(auth_info, (void **)&dst->data, &dst->length, v);
}

int SceSblACMgrForKernel_CCDBB74D_cmd7(const SceSelfAuthInfo *auth_info, SceSblACAttribute *dst, const SceSblACAttribute *src){
	return 0x800F0925;
}

int SceSblACMgrForKernel_CCDBB74D_cmd8(const SceSelfAuthInfo *auth_info, SceSblACAttribute *dst, const SceSblACAttribute *src){
	return 0x800F0925;
}

SceInt32 SceSblACMgrForKernel_CCDBB74D(SceUID pid, SceSblACAttribute *dst, const SceSblACAttribute *src){

	SceInt32 res;
	SceSelfAuthInfo auth_info;

	res = sceSblACMgrGetProcessAuthInfo(pid, dst, &auth_info);
	if(0 != res)
		return res;

	if(src == NULL || src->data == NULL)
		return 0x800F0916;

	switch(dst->cmd){
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		res = SceSblACMgrForKernel_CCDBB74D_basic(&auth_info, dst, src);
		break;
	case 6:
		res = SceSblACMgrForKernel_CCDBB74D_cmd6(&auth_info, dst, src);
		break;
	case 7:
		res = SceSblACMgrForKernel_CCDBB74D_cmd7(&auth_info, dst, src);
		break;
	case 8:
		res = SceSblACMgrForKernel_CCDBB74D_cmd8(&auth_info, dst, src);
		break;
	default:
		res = 0x800F0916;
		break;
	}

	return res;
}
