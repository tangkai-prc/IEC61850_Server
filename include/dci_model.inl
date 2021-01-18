#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dci_model.h"

void * DCI_GetOperInf(char * do_ref)
{
	char ref[256], *s, fc[8];
	DCI_SID sid;
	
	if(do_ref == NULL || do_ref[0] == '\0')
		return NULL;

	ref[sizeof(ref)-1] = '\0';
	strncpy(ref, do_ref, sizeof(ref)-1);
	if((s = strchr(ref, '[')) == NULL)
		return NULL;

	*s++ = '\0';
	fc[0] = '[';
	fc[1] = *s;
	fc[2] = *(s + 1);
	fc[3] = ']';
	fc[4] = '\0';
	strncat(ref, ".Oper.ctlVal", sizeof(ref)-1);
	strncat(ref, fc, sizeof(ref)-1);

	if((sid = DCI_GetSidByRef(ref)) == DCI_INVALID_SID)
		return NULL;

	return DCI_GetInfBySid(sid);
}

int DCI_GetAIConfig(char * ref, DCI_AI_CONFIG * config)
{
	DCI_SID sid;
	char * cdc_type;
	DCI_AI_CONFIG ai_config;
	DCI_CAI_CONFIG cai_config;
	int ret;
	DCI_DataAttr ai_attr[4] = {
		{ "sVC.scaleFactor", DCI_FLOAT32, sizeof(float), &ai_config.scaleFactor },
		{ "sVC.offset", DCI_FLOAT32, sizeof(float), &ai_config.offset },
		{ "units.multiplier", DCI_INT8, sizeof(char), &ai_config.multiplier },
		{ "units.SIUnit", DCI_INT8U, sizeof(unsigned char), &ai_config.SIUnit }
	};

	if(ref == NULL || config == NULL)
		return DCI_EINVAL;

	if((sid = DCI_GetSidByRef(ref)) == DCI_INVALID_SID)
		return NULL;

	cdc_type = DCI_GetCDCType(sid);

	if(strcmp(cdc_type, "MV") == 0){
		ai_config.scaleFactor = 1.0;
		ai_config.offset = 0;
		ai_config.multiplier = 0;
		ai_config.SIUnit = 0;

		if((ret = DCI_QueryDataAttrs(sid, 4, ai_attr)) <= 0)
			return ret;

		memcpy(config, &ai_config, sizeof(DCI_AI_CONFIG));
	}
	else if(strcmp(cdc_type, "CMV") == 0){
		if((ret = DCI_GetCAIConfig(ref, &cai_config)) != DCI_ENONE)
			return ret;

		memcpy(config, &cai_config.mag, sizeof(DCI_AI_CONFIG));
	}
	else
		return DCI_EDATA;

	return DCI_ENONE;
}

int DCI_GetCAIConfig(char * ref, DCI_CAI_CONFIG * config)
{
	DCI_SID sid;
	char * cdc_type;
	DCI_CAI_CONFIG cai_config;
	int ret;
	DCI_DataAttr cai_attr[6] = {
		{ "magSVC.scaleFactor", DCI_FLOAT32, sizeof(float), &cai_config.mag.scaleFactor },
		{ "magSVC.offset", DCI_FLOAT32, sizeof(float), &cai_config.mag.offset },
		{ "units.multiplier", DCI_INT8, sizeof(char), &cai_config.mag.multiplier },
		{ "units.SIUnit", DCI_INT8U, sizeof(unsigned char), &cai_config.mag.SIUnit },
		{ "angSVC.scaleFactor", DCI_FLOAT32, sizeof(float), &cai_config.ang.scaleFactor },
		{ "angSVC.offset", DCI_FLOAT32, sizeof(float), &cai_config.ang.offset },
	};
	
	if(ref == NULL || config == NULL)
		return DCI_EINVAL;

	if((sid = DCI_GetSidByRef(ref)) == DCI_INVALID_SID)
		return NULL;

	cdc_type = DCI_GetCDCType(sid);

	if(strcmp(cdc_type, "CMV") != 0)
		return DCI_EDATA;

	cai_config.mag.scaleFactor = 1.0;
	cai_config.mag.offset = 0;
	cai_config.mag.multiplier = 0;
	cai_config.mag.SIUnit = 0;
	cai_config.ang.scaleFactor = 1.0;
	cai_config.ang.offset = 0;

	if((ret = DCI_QueryDataAttrs(sid, 6, cai_attr)) <= 0)
		return ret;

	cai_config.ang.multiplier = cai_config.mag.multiplier;
	cai_config.ang.SIUnit = cai_config.mag.SIUnit;
	memcpy(config, &cai_config, sizeof(DCI_CAI_CONFIG));
	return DCI_ENONE;
}
