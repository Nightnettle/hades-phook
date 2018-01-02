#pragma once
#include "HookIncludes.h"
typedef void(__thiscall* dme_t)(void*, void*, void*, const ModelRenderInfo_t&, matrix3x4*);

float flColor[3] = { 0.f };

void __fastcall hkDrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld)
{
    static bool DontDraw = false;
    static auto hkDme = hooks::modelrender.get_original<dme_t>(21);
    if (g_Options.Visuals.Enabled && g_Options.Visuals.Chams)
    {
        static IMaterial* ignorez = CreateMaterial(true, true, false);
        static IMaterial* notignorez = CreateMaterial(false, true, false);
		static IMaterial* mat = CreateMaterial(false, false, false);
		static IMaterial* glow = g_MaterialSystem->FindMaterial("vgui/achievements/glow", "models/players");
		static IMaterial* gold = g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/gold", "models/players");
		static IMaterial* glass = g_MaterialSystem->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", "models/players");
        DontDraw = false;

        const char * ModelName = g_ModelInfo->GetModelName((model_t*)pInfo.pModel);
        C_BaseEntity* pModelEntity = (C_BaseEntity*)g_EntityList->GetClientEntity(pInfo.entity_index);
        C_BaseEntity* pLocal = (C_BaseEntity*)g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
		

        
        if (ModelName && g_Options.Visuals.Chams && strstr(ModelName, "models/player"))
        {
            if (pModelEntity && pLocal)
            {
                if (!g_Options.Visuals.Filter.EnemyOnly ||
                    pModelEntity->GetTeamNum() != pLocal->GetTeamNum())
                {
                    pModelEntity = g_EntityList->GetClientEntity(pInfo.entity_index);
                    if (pModelEntity)
                    {

                        if (!g_Options.Visuals.IsVisibleChams && pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0)
                        {

							float alpha = 1.f;

							if (pModelEntity->HasGunGameImmunity())
								alpha = 0.5f;
							
							
								g_RenderView->SetColorModulation(g_Options.Colors.chams_color_notvis);
								g_RenderView->SetBlend(alpha);
								g_ModelRender->ForcedMaterialOverride(ignorez);
								hkDme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
							
							
							

								g_RenderView->SetColorModulation(g_Options.Colors.chams_color_vis);
								g_RenderView->SetBlend(alpha);
								g_ModelRender->ForcedMaterialOverride(notignorez);
								hkDme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
							

                        }
						else if (g_Options.Visuals.IsVisibleChams && pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0)
						{
				
							
							float alpha = 1.0f;
							if (pModelEntity->HasGunGameImmunity())
								alpha = 0.5f;
							
							
								g_RenderView->SetColorModulation(g_Options.Colors.chams_color_vis);
								g_RenderView->SetBlend(alpha);
								g_ModelRender->ForcedMaterialOverride(notignorez);
								hkDme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
							
							
		

						}
						else
						{
							ForceMaterial(Color(255, 255, 255), ignorez);
						}
                    }
                }
            }
        }


        else if (g_Options.Visuals.Chams && g_Options.Visuals.WeaponsWorld && strstr(ModelName, "_dropped.mdl"))
        {
            ForceMaterial(Color(255, 255, 255), notignorez);
        }
		if (g_Options.Visuals.Hands && strstr(ModelName, "arms"))
		{
			if (g_Options.Visuals.Hands == 1)
			{
				DontDraw = true;
			}
			if (g_Options.Visuals.Hands == 2)
			{
					g_ModelRender->ForcedMaterialOverride(notignorez);
					g_RenderView->SetColorModulation(g_Options.Colors.hands_color);
					g_RenderView->SetBlend(g_Options.Colors.hands_alpha);

			}
			if (g_Options.Visuals.Hands == 3)
			{
				IMaterial* Hands = g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/gold", "Model textures");
					g_ModelRender->ForcedMaterialOverride(Hands);
					g_RenderView->SetBlend(g_Options.Colors.hands_alpha);
			}

		}



	}
    if (!DontDraw)
        hkDme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
    g_ModelRender->ForcedMaterialOverride(NULL);

}