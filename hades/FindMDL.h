#pragma once
#include "HookIncludes.h"

#define MP5_WEAPON_MODEL_PATH "models/weapons/v_smg_mp5.mdl"

typedef MDLHandle_t(__thiscall* iFindMdl)(void*, char*);
iFindMdl oFindMDL;
MDLHandle_t __fastcall hkFindMDL(void*, void*, char*);


MDLHandle_t __fastcall hkFindMDL(void* ecx, void* edx, char* FilePath)
{
	if (strstr(FilePath, "models/player")
		&& strstr(FilePath, "w_"))
		//return oFindMDL(ecx, "models/player/custom_player/xnet/girl/tokitsukaze/tokitsukaze.mdl");// https://i.imgur.com/EiEMUoV.png
		//return oFindMDL(ecx, "models/player/custom_player/kuristaja/trump/trump.mdl");// trump xd
		//return oFindMDL(ecx, "models/player/custom_player/xnet/girl/noire_swimwear/noire.mdl");// https://i.imgur.com/LyDYMbz.png
		return oFindMDL(ecx, "models/player/custom_player/kuristaja/banana_joe/banana_joe.mdl");// banana xd
	
	//Minecraft
	if (strstr(FilePath, "models/player") && strstr(FilePath, "knife_default_ct.mdl") || strstr(FilePath, "knife_default_t.mdl"))
		return oFindMDL(ecx, "models/player/minecaft/v_minecraft_pickaxe.mdl");
	

	/*if (strstr(pMDLRelativePath, "models/weapons/v_rif_ak47.mdl"))
		return oFindMDL(ecx, "models/weapons/v_rif_ak41.mdl");//cs:co ak47

	if (strstr(pMDLRelativePath, "models/weapons/v_rif_m4a1_s.mdl"))
		return oFindMDL(ecx, "models/weapons/v_rif_m4a2_s.mdl");//cs:co m4*/

	//if (strstr(FilePath, "models/weapons/v_smg_mp7.mdl"))
		//return oFindMDL(ecx, MP5_WEAPON_MODEL_PATH);

	//if (strstr(FilePath, "models/weapons/v_models/arms/glove_fingerless/v_glove_fingerless.mdl"))
		//return oFindMDL(ecx, "models/nip/t_base_fingerless_glove_color.vtf");


    return oFindMDL(ecx, FilePath);
}