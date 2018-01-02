#pragma once
#include "HookIncludes.h"

typedef void(__thiscall* EndScreenFn)(void*);

//This is cancer please move on
void __fastcall scene_end(void* thisptr, void* edx) {

	static auto scene_end_o = hooks::modelrender.get_original< decltype(&scene_end) >(9);
	scene_end_o(thisptr, edx);

	C_BaseEntity* pLocal = (C_BaseEntity*)g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());

	if (pLocal)
	{
		IMaterial* mat = CreateMaterial(false);
		if (mat)
		{

			Vector* OriginAng;
			OriginAng = pLocal->GetEyeAngles();
			//pLocal->SetAngle2(Vector(0, lineFakeAngle, 0)); // paste here ur AA.y value or pLocal->GetLby() (for example)
			bool LbyColor = false; // u can make LBY INDICATOR. When LbyColor is true. Color will be Green , if false it will be White
			float NormalColor[3] = { 1, 1, 1 };
			float lbyUpdateColor[3] = { 0, 1, 0 };
			g_RenderView->SetColorModulation(LbyColor ? lbyUpdateColor : NormalColor);
			g_ModelRender->ForcedMaterialOverride(mat);
			pLocal->draw_model(1, 255);
			g_ModelRender->ForcedMaterialOverride(nullptr);
			//pLocal->SetAngle2(OriginAng);
		}
	}
}
