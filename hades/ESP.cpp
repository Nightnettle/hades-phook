
#include "ESP.h"
#include "Interfaces.h"
#include "Render.h"
#include <ctime>
#include <iostream>
#include <algorithm>
#include "GrenadePrediction.h"
#include "LagComp.h"
#include "Autowall.h"


visuals::visuals()
{
	BombCarrier = nullptr;
}

int width = 0;
int height = 0;
bool done = false;
void visuals::OnPaintTraverse(C_BaseEntity* local)
{


    for (int i = 0; i < g_EntityList->GetHighestEntityIndex(); i++)
    {

        C_BaseEntity *entity = g_EntityList->GetClientEntity(i);
        player_info_t pinfo;
		
		
		if (entity == local && local->IsAlive() && g_Engine->GetPlayerInfo(g_Engine->GetLocalPlayer(), &pinfo))
        {
            
			
			if (g_Input->m_fCameraInThirdPerson && g_Options.Visuals.Enabled)
            {
                Vector max = entity->GetCollideable()->OBBMaxs();
                Vector pos, pos3D;
                Vector top, top3D;
                pos3D = entity->GetOrigin();
                top3D = pos3D + Vector(0, 0, max.z);

                if (!g_Render->WorldToScreen(pos3D, pos) || !g_Render->WorldToScreen(top3D, top))
                    return;

                float height = (pos.y - top.y);
                float width = height / 4.f;
				if (g_Options.Visuals.Box )
				{

					Color color;
					color = GetPlayerColor(entity, local);
					PlayerBox(top.x, top.y, width, height, color);

				}
				if (g_Options.Visuals.IsScoped && local->IsScoped())
					g_Render->DrawString2(g_Render->font.ESP, (int)top.x, (int)top.y -16, Color::Red(), FONT_CENTER, "*Scoped*");

                if (g_Options.Visuals.HP)
                    DrawHealth(pos, top, local->GetHealth());
				

                if (g_Options.Visuals.Name)
                    g_Render->DrawString2(g_Render->font.ESP, (int)top.x, (int)top.y - 6, Color::White(), FONT_CENTER, pinfo.name);
				
            }
        }
        if (entity && entity != local && !entity->IsDormant())
        {
			
			if (g_Engine->GetPlayerInfo(i, &pinfo) && entity->IsAlive())
            {
				if (g_Options.Legitbot.backtrack)
				{
					if (local->IsAlive())
					{
						for (int t = 0; t < 12; ++t)
						{
							Vector screenbacktrack[64][12];

							if (headPositions[i][t].simtime && headPositions[i][t].simtime + 1 > local->GetSimulationTime())
							{
								if (g_Render->WorldToScreen(headPositions[i][t].hitboxPos, screenbacktrack[i][t]))
								{

									g_Surface->DrawSetColor(Color::Green());
									g_Surface->DrawOutlinedRect(screenbacktrack[i][t].x, screenbacktrack[i][t].y, screenbacktrack[i][t].x + 2, screenbacktrack[i][t].y + 2);

								}
							}
						}
					}
					else
					{
						memset(&headPositions[0][0], 0, sizeof(headPositions));
					}
				}
				if (g_Options.Ragebot.PosAdj)
				{
					if (local->IsAlive())
					{
						for (int t = 0; t < 12; ++t)
						{
							Vector screenbacktrack[64];

							if (backtracking->records[i].tick_count + 12 > g_Globals->tickcount)
							{
								if (g_Render->WorldToScreen(backtracking->records[i].headPosition, screenbacktrack[i]))
								{

									g_Surface->DrawSetColor(Color::Blue());
									g_Surface->DrawOutlinedRect(screenbacktrack[i].x, screenbacktrack[i].y, screenbacktrack[i].x + 2, screenbacktrack[i].y + 2);

								}
							}
						}
					}
					else
					{
						memset(&headPositions[0][0], 0, sizeof(headPositions));
					}
				}
				if (g_Options.Ragebot.FakeLagFix)
				{
					if (local->IsAlive())
					{
						Vector screenbacktrack[64];

						if (backtracking->records[i].tick_count + 12 > g_Globals->tickcount)
						{
							if (g_Render->WorldToScreen(backtracking->records[i].headPosition, screenbacktrack[i]))
							{

								g_Surface->DrawSetColor(Color::Black());
								g_Surface->DrawLine(screenbacktrack[i].x, screenbacktrack[i].y, screenbacktrack[i].x + 2, screenbacktrack[i].y + 2);

							}
						}
					}
					else
					{
						memset(&backtracking->records[0], 0, sizeof(backtracking->records));
					}
				
				}

                if (g_Options.Visuals.Enabled )
                {
                    if (g_Options.Visuals.DLight)
                        DLight(local, entity);

                    DrawPlayer(entity, pinfo, local);

                }
				if (g_Options.Misc.AWalldmg && local->IsAlive())
					DrawAwall();


            }
            if (g_Options.Visuals.Enabled)
            {
                ClientClass* cClass = (ClientClass*)entity->GetClientClass();
                if (g_Options.Visuals.WeaponsWorld && cClass->m_ClassID != (int)ClassID::CBaseWeaponWorldModel && ((strstr(cClass->m_pNetworkName, "Weapon") || cClass->m_ClassID == (int)ClassID::CDEagle || cClass->m_ClassID == (int)ClassID::CAK47)))
                {
                    DrawDrop(entity);
                }
                if (g_Options.Visuals.C4World)
                {
                    if (cClass->m_ClassID == (int)ClassID::CPlantedC4)
                        DrawBombPlanted(entity, local);
                }

                if (cClass->m_ClassID == (int)ClassID::CC4)
                    DrawBomb(entity, cClass);
                if (g_Options.Visuals.GrenadeESP && strstr(cClass->m_pNetworkName, "Projectile"))
                {
                    DrawThrowable(entity);
                }
            }
        }
    }


    if (g_Options.Misc.SpecList) SpecList(local);
	if(g_Options.Misc.aalines) DrawAngles();
	
    NightMode();
	Crosshair();
	CInput::CUserCmd *pCmd;
	bool bSendPacket;
    grenade_prediction::instance().Paint();


}


std::string CleanItemName(std::string name)
{
	std::string Name = name;
	// Tidy up the weapon Name
	if (Name[0] == 'C')
		Name.erase(Name.begin());

	// Remove the word Weapon
	auto startOfWeap = Name.find("Weapon");
	if (startOfWeap != std::string::npos)
		Name.erase(Name.begin() + startOfWeap, Name.begin() + startOfWeap + 6);

	return Name;
}

wchar_t* CharToWideChar(const char* text)
{
	size_t size = strlen(text) + 1;
	wchar_t* wa = new wchar_t[size];
	mbstowcs_s(NULL, wa, size / 4, text, size);
	return wa;
}


C_CSPlayerResource* playerresources;
void visuals::DrawPlayer(C_BaseEntity* entity, player_info_t pinfo, C_BaseEntity* local)
{

	Vector max = entity->GetCollideable()->OBBMaxs();
	Vector pos, pos3D;
	Vector top, top3D;
	pos3D = entity->GetOrigin();
	top3D = pos3D + Vector(0, 0, max.z);

	if (!g_Render->WorldToScreen(pos3D, pos) || !g_Render->WorldToScreen(top3D, top))
		return;

	float height = (pos.y - top.y);
	float width = height / 4.f;

	Color color;

	if (g_Options.Visuals.Filter.EnemyOnly && (entity->GetTeamNum() == local->GetTeamNum()))
		return;
	color = GetPlayerColor(entity, local);
	if (!g_Options.Visuals.IsVisibleBox && g_Options.Visuals.Box)
		PlayerBox(top.x, top.y, width , height , color);
	else if (g_Options.Visuals.Box && MiscFunctions::IsVisible(local, entity, 0) && g_Options.Visuals.IsVisibleBox)
		PlayerBox(top.x, top.y, width , height , color);


	if (g_Options.Visuals.HP)
		DrawHealth(pos, top, entity->GetHealth());
	if(g_Options.Visuals.IsScoped && entity->IsScoped())
		g_Render->DrawString2(g_Render->font.ESP, (int)top.x, (int)top.y - 20, Color::Red(), FONT_CENTER, "*Scoped*");
	if (g_Options.Misc.resolvemode)
	{
		if (Globals::resolvemode == 1)
		{
			g_Render->Text((int)top.x + 16, (int)top.y, Color(200, 200, 200, 255), g_Render->font.ESP, "Bruteforce");
		}
		else if (Globals::resolvemode == 2)
		{
			g_Render->Text((int)top.x + 16, (int)top.y , Color(200, 200, 200, 255), g_Render->font.ESP, "Predicting LBY");
		}
		else if (Globals::resolvemode == 3)
		{
			g_Render->Text((int)top.x + 16, (int)top.y , Color(200, 200, 200, 255), g_Render->font.ESP, "LBY Update");
		}
		else if (Globals::resolvemode == 4)
		{
			g_Render->Text((int)top.x + 16, (int)top.y , Color(200, 200, 200, 255), g_Render->font.ESP, "Legit");
		}
	}
	/*
	if (g_Options.Visuals.BulletTrace)
		BulletTrace(entity, color);
		*/
	if (g_Options.Visuals.Name)
		g_Render->DrawString2(g_Render->font.ESP, (int)top.x, (int)top.y - 6, Color::White(), FONT_CENTER, pinfo.name);

	int bottom = 0;


	std::vector<std::string> weapon;
	std::vector<std::string> bomb;
	std::vector<std::string> rank;
	std::vector<std::string> wins;

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(entity->GetActiveWeaponHandle());
    if (g_Options.Visuals.Weapon && pWeapon)
    {
        int weapon_id = pWeapon->m_AttributeManager()->m_Item()->GetItemDefinitionIndex();

        auto weapon_name = ItemDefinitionIndexToString(weapon_id);
        weapon.push_back(weapon_name);
    }

	if (g_Options.Visuals.Skeleton)
		DrawSkeleton(entity);


	if (g_Options.Visuals.C4 && entity == BombCarrier)
	{
		bomb.push_back("Bomb");
	}

	int i = 0;
	if (g_Options.Visuals.Weapon)
	{

		for (auto Text : weapon)
		{
			g_Render->DrawString2(g_Render->font.Guns, (int)top.x, int(top.y + height + 8 + (10 * bottom++)), Color::White(), FONT_CENTER, "%s", Text.c_str());
			i++;
		}
	}
	if (g_Options.Visuals.C4)
	{
		for (auto Text : bomb)
		{
			g_Render->DrawString2(g_Render->font.Guns, (int)top.x, int(top.y + height + 8 + (10 * bottom++)), Color::Red(), FONT_CENTER, Text.c_str());
			i++;
		}
	}

	/*if(menu.Visuals.money)
	{
		g_Render->Textf(int(top.x + width + 3), int(top.y + 12), Color(255, 255, 255, 255), g_Render->font.ESP, "%i", entity->GetMoney());
	}*/

	

}

void visuals::DrawAngles()
{
	C_BaseEntity *pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());

	Vector src3D, dst3D, forward, src, dst;
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	filter.pSkip = pLocal;

	AngleVectors(QAngle(0, pLocal->GetLowerBodyYaw(), 0), &forward);
	src3D = pLocal->GetOrigin();
	dst3D = src3D + (forward * 35.f); //replace 50 with the length you want the line to have

	ray.Init(src3D, dst3D);

	g_EngineTrace->TraceRay(ray, 0, &filter, &tr);

	if (!g_Render->WorldToScreen(src3D, src) || !g_Render->WorldToScreen(tr.endpos, dst))
	return;

	g_Render->Line(src.x, src.y, dst.x, dst.y, Color(0, 0, 255, 255));
	g_Render->Text(dst.x, dst.y, Color(0, 0, 255, 255), g_Render->font.ESP, "lby");

	AngleVectors(QAngle(0, Globals::RealAngle, 0), &forward);
	src3D = pLocal->GetOrigin();
	dst3D = src3D + (forward * 35.f);

	ray.Init(src3D, dst3D);

	g_EngineTrace->TraceRay(ray, 0, &filter, &tr);

	if (!g_Render->WorldToScreen(src3D, src) || !g_Render->WorldToScreen(tr.endpos, dst))
		return;

	g_Render->Line(src.x, src.y, dst.x, dst.y, Color(0, 255, 0, 255));
	g_Render->Text(dst.x, dst.y, Color(0, 255, 0, 255), g_Render->font.ESP, "REAL");

	AngleVectors(QAngle(0, Globals::FakeAngle, 0), &forward);
	dst3D = src3D + (forward * 35.f);

	ray.Init(src3D, dst3D);

	g_EngineTrace->TraceRay(ray, 0, &filter, &tr);

	if (!g_Render->WorldToScreen(src3D, src) || !g_Render->WorldToScreen(tr.endpos, dst))
		return;

	g_Render->Line(src.x, src.y, dst.x, dst.y, Color(255, 0, 0, 255));
	g_Render->Text(dst.x, dst.y, Color(255, 0, 0, 255), g_Render->font.ESP, "FAKE");
}

void visuals::PlayerBox(float x, float y, float w, float h, Color clr)
{
	g_Surface->DrawSetColor(clr);
	g_Surface->DrawOutlinedRect(int(x - w), int(y), int(x + w), int(y + h));
	g_Surface->DrawSetColor(Color::Black());
	g_Surface->DrawOutlinedRect(int(x - w - 1), int(y - 1), int(x + w + 1), int(y + h + 1));
	g_Surface->DrawOutlinedRect(int(x - w + 1), int(y + 1), int(x + w - 1), int(y + h - 1));
}

void visuals::DrawAwall()
{
	int MidX;
	int MidY;
	g_Engine->GetScreenSize(MidX, MidY);

	int damage;
	if (CanWallbang(damage))
	{
		g_Render->OutlineCircle(MidX / 2, MidY / 2, 10, 10, Color(0, 255, 0));
		g_Render->Textf(MidX / 2, MidY / 2 + 6, Color(255, 255, 255, 255), g_Render->font.ESP, "DMG: %1i", damage);
	}
	else
	{
		g_Render->OutlineCircle(MidX / 2, MidY / 2, 10, 10, Color(255, 0, 0));
		g_Render->Textf(MidX / 2, MidY / 2 + 6, Color(255, 255, 255, 255), g_Render->font.ESP, "DMG: 0");
	}
}

void visuals::Hitmarker()
{
	if (G::hitmarkeralpha < 0.f)
		G::hitmarkeralpha = 0.f;
	else if (G::hitmarkeralpha > 0.f)
		G::hitmarkeralpha -= 0.01f;

	int W, H;
	g_Engine->GetScreenSize(W, H);

	if (G::hitmarkeralpha > 0.f)
	{
		g_Render->Line(W / 2 - 10, H / 2 - 10, W / 2 - 5, H / 2 - 5, Color(240, 240, 240, (G::hitmarkeralpha * 255.f)));
		g_Render->Line(W / 2 - 10, H / 2 + 10, W / 2 - 5, H / 2 + 5, Color(240, 240, 240, (G::hitmarkeralpha * 255.f)));
		g_Render->Line(W / 2 + 10, H / 2 - 10, W / 2 + 5, H / 2 - 5, Color(240, 240, 240, (G::hitmarkeralpha * 255.f)));
		g_Render->Line(W / 2 + 10, H / 2 + 10, W / 2 + 5, H / 2 + 5, Color(240, 240, 240, (G::hitmarkeralpha * 255.f)));

	}
}


Color visuals::GetPlayerColor(C_BaseEntity* entity, C_BaseEntity* local)
{
	int TeamNum = entity->GetTeamNum();
	bool IsVis = MiscFunctions::IsVisible(local, entity, Head);

	Color color;
	static float rainbow;
	rainbow += 0.005f;
	if (rainbow > 1.f)
		rainbow = 0.f;
	if (TeamNum == TEAM_CS_T)
	{

		if (IsVis)
			color = Color (int(g_Options.Colors.box_color_t[0] * 255.f), int(g_Options.Colors.box_color_t[1] * 255.f), int(g_Options.Colors.box_color_t[2] * 255.f));
		else
			color = Color(235, 50, 0, 100);
	}
	else
	{
		if (IsVis)
			color = Color (int(g_Options.Colors.box_color_ct[0] * 255.f), int(g_Options.Colors.box_color_ct[1] * 255.f), int(g_Options.Colors.box_color_ct[2] * 255.f));
		else
			color = Color(235, 50, 0, 100);
	}


	return color;
}
void visuals::DrawHealth(C_BaseEntity* entity, visuals::ESPBox size)
{
	int health = entity->GetHealth();
	int HP = health;
	if (HP > 100)
		HP = 100;
	int hp = health;
	float r = float(255 - health * 2.55);
	float g = float(health * 2.55);
	hp = (size.h - ((size.h * hp) / 100));

	g_Render->Outline(int(size.x - 4), int(size.y + hp), (int)2, int(size.h - hp + 1), Color((int)r, (int)g, 0));
	g_Render->Outline(int(size.x - 5), int(size.y - 1), (int)3, int(size.h + 2), Color(0, 0, 0, 150));
}

void visuals::DrawHealth(Vector bot, Vector top, float health)
{
	float h = (bot.y - top.y);
	float offset = (h / 4.f) + 4;
	float w = h / 64.f;

	UINT hp = UINT(h - (UINT)((h * health) / 100)); // Percentage

	int Red = int(255 - (health*2.55));
	int Green = int(health*2.55);

	g_Render->DrawOutlinedRect(int((top.x - offset) - 1), int(top.y - 1), 3, int(h + 2), Color::Black());

	g_Render->Line(int((top.x - offset)), int(top.y + hp), int((top.x - offset)), int(top.y + h), Color(Red, Green, 0, 255));
}

void visuals::DrawDrop(C_BaseEntity* entity)
{
    if (entity)
    {
        CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)entity;

        auto owner = pWeapon->GetOwnerHandle();

        if (owner > -1)
            return;

        Vector pos3D = entity->GetAbsOrigin2();

        if (pos3D.x == 0.0f && pos3D.y == 0.0f && pos3D.z == 0.0f)
            return;

        Vector pos;

        if (!g_Render->WorldToScreen(pos3D, pos))
            return;

        int weaponID = pWeapon->m_AttributeManager()->m_Item()->GetItemDefinitionIndex();
        auto weaponName = ItemDefinitionIndexToString(weaponID);


        g_Render->Text(pos.x, pos.y, Color(255, 255, 255, 255), g_Render->font.DroppedGuns, weaponName);
    }
}
float damage;
char bombdamagestringdead[24];
char bombdamagestringalive[24];
void visuals::DrawBombPlanted(C_BaseEntity* entity, C_BaseEntity* local)
{
	BombCarrier = nullptr;

	Vector vOrig; Vector vScreen;
	vOrig = entity->GetOrigin();
	CCSBomb* Bomb = (CCSBomb*)entity;
	float flBlow = Bomb->GetC4BlowTime();
	float lifetime = flBlow - (g_Globals->interval_per_tick * local->GetTickBase());
	if (g_Render->WorldToScreen(vOrig, vScreen))
	{
		if (local->IsAlive())
		{
			float flDistance = local->GetEyePosition().DistTo(entity->GetEyePosition());
			float a = 450.7f;
			float b = 75.68f;
			float c = 789.2f;
			float d = ((flDistance - b) / c);
			float flDamage = a*exp(-d * d);

			damage = float((std::max)((int)ceilf(CSGO_Armor(flDamage, local->ArmorValue())), 0));

			sprintf_s(bombdamagestringdead, sizeof(bombdamagestringdead) - 1, "You're ded.");
			sprintf_s(bombdamagestringalive, sizeof(bombdamagestringalive) - 1, "Health left: %.0f", local->GetHealth() - damage);
            if (lifetime > -2.f)
            {
                if (damage >= local->GetHealth())
                {
                    g_Render->Text((int)vScreen.x, int(vScreen.y + 9), Color::Red(), g_Render->font.Defuse, bombdamagestringdead);
                }
                else if (local->GetHealth() > damage)
                {
                    g_Render->Text((int)vScreen.x, int(vScreen.y + 9), Color::Lime(), g_Render->font.Defuse, bombdamagestringalive);
                }
            }
		}
		char buffer[64];
		if (lifetime > 0.01f && !Bomb->IsBombDefused())
		{
			sprintf_s(buffer, "Bomb: %.1f", lifetime);
			g_Render->Text((int)vScreen.x, (int)vScreen.y, Color(250, 42, 42, 255), g_Render->font.ESP, buffer);
		}

	}

	g_Engine->GetScreenSize(width, height);
	int halfX = width / 2;
	int halfY = height / 2;


	if (Bomb->GetBombDefuser() > 0)
	{
		float countdown = Bomb->GetC4DefuseCountDown() - (local->GetTickBase() * g_Globals->interval_per_tick);
		if (countdown > 0.01f)
		{
			if (lifetime > countdown)
			{
				char defuseTimeString[24];
				sprintf_s(defuseTimeString, sizeof(defuseTimeString) - 1, "Defusing: %.1f", countdown);
				g_Render->Text(halfX - 50, halfY + 200, Color(0, 255, 0, 255), g_Render->font.Defuse, defuseTimeString);
			}
			else
			{
				g_Render->Text(halfX - 50, halfY + 200, Color(255, 0, 0, 255), g_Render->font.Defuse, "No time, run!");
			}
		}
	}
}

void visuals::Crosshair()
{
	C_BaseEntity* local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(local->GetActiveWeaponHandle());
	if (g_Options.Visuals.crosshair && MiscFunctions::IsSniper(pWeapon))
	{
		// Crosshair
		RECT View = g_Render->GetViewport();
		int MidX = View.right / 2;
		int MidY = View.bottom / 2;
		g_Render->Line(MidX - 7, MidY - 7, MidX + 7, MidY + 7, Color(int(g_Options.Colors.crosshair_color[0]*255), int(g_Options.Colors.crosshair_color[1] * 255), int(g_Options.Colors.crosshair_color[2] * 255), 255));
		g_Render->Line(MidX + 7, MidY - 7, MidX - 7, MidY + 7, Color(int(g_Options.Colors.crosshair_color[0] * 255), int(g_Options.Colors.crosshair_color[1] * 255), int(g_Options.Colors.crosshair_color[2] * 255), 255));
	}

}

void visuals::DrawBomb(C_BaseEntity* entity, ClientClass* cClass)
{
	// Null it out incase bomb has been dropped or planted
	BombCarrier = nullptr;
	CBaseCombatWeapon *BombWeapon = (CBaseCombatWeapon *)entity;
	Vector vOrig; Vector vScreen;
	vOrig = entity->GetOrigin();
	bool adopted = true;
	auto parent = BombWeapon->GetOwnerHandle();
	if (parent || (vOrig.x == 0 && vOrig.y == 0 && vOrig.z == 0))
	{
		C_BaseEntity* pParentEnt = (g_EntityList->GetClientEntityFromHandle(parent));
		if (pParentEnt && pParentEnt->IsAlive())
		{
			BombCarrier = pParentEnt;
			adopted = false;
		}
	}
	if (g_Options.Visuals.C4World)
	{
		if (adopted)
		{
			if (g_Render->WorldToScreen(vOrig, vScreen))
			{
				g_Render->Text((int)vScreen.x, (int)vScreen.y, Color(112, 20, 20, 255), g_Render->font.ESP, "Bomb");
			}
		}
	}
}
void visuals::DrawSkeleton(C_BaseEntity* entity)
{
	studiohdr_t* pStudioHdr = g_ModelInfo->GetStudiomodel(entity->GetModel());

	if (!pStudioHdr)
		return;

	Vector vParent, vChild, sParent, sChild;

	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		mstudiobone_t* pBone = pStudioHdr->GetBone(j);

		if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1))
		{
			vChild = entity->GetBonePos(j);
			vParent = entity->GetBonePos(pBone->parent);

			
			if (g_Render->WorldToScreen(vParent, sParent) && g_Render->WorldToScreen(vChild, sChild))
			{
				g_Render->Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(255, 50, 50, 255));
			}
		}
	}
}

void visuals::DrawBox(visuals::ESPBox size, Color color)
{
	Color esp_box(int(g_Options.Colors.box_color_t[0] * 255.f), int(g_Options.Colors.box_color_t[1] * 255.f), int(g_Options.Colors.box_color_t[2] * 255.f));
	g_Render->Outline(size.x, size.y, size.w, size.h, esp_box);//color);
	g_Render->Outline(size.x - 1, size.y - 1, size.w + 2, size.h + 2, Color(0, 0, 0, 255));
	g_Render->Outline(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(0, 0, 0, 255));
}
bool visuals::GetBox(C_BaseEntity* entity, visuals::ESPBox &result)
{
	// Variables
	Vector  vOrigin, min, max, sMin, sMax, sOrigin,
		flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	// Get the locations
	vOrigin = entity->GetOrigin();
	min = entity->collisionProperty()->GetMins() + vOrigin;
	max = entity->collisionProperty()->GetMaxs() + vOrigin;

	// Points of a 3d bounding box
	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };

	// Get screen positions
	if (!g_Render->WorldToScreen(points[3], flb) || !g_Render->WorldToScreen(points[5], brt)
		|| !g_Render->WorldToScreen(points[0], blb) || !g_Render->WorldToScreen(points[4], frt)
		|| !g_Render->WorldToScreen(points[2], frb) || !g_Render->WorldToScreen(points[1], brb)
		|| !g_Render->WorldToScreen(points[6], blt) || !g_Render->WorldToScreen(points[7], flt))
		return false;

	// Put them in an array (maybe start them off in one later for speed?)
	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	// Init this shit
	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	// Find the bounding corners for our box
	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	// Width / height
	result.x = (int)left;
	result.y = (int)top;
	result.w = int(right - left);
	result.h = int(bottom - top);

	return true;
}
void visuals::BoxAndText(C_BaseEntity* entity, std::string text)
{
	ESPBox Box;
	std::vector<std::string> Info;
	if (GetBox(entity, Box))
	{
		Info.push_back(text);
		if (g_Options.Visuals.GrenadeESP)
		{
			DrawBox(Box, Color(255, 0, 0, 255));
			int i = 0;
			for (auto kek : Info)
			{
				g_Render->Text(Box.x + 1, Box.y + 1, Color(255, 0, 0, 255), g_Render->font.ESP, kek.c_str());
				i++;
			}
		}
	}
}
void visuals::DrawThrowable(C_BaseEntity* throwable)
{
	model_t* nadeModel = (model_t*)throwable->GetModel();

	if (!nadeModel)
		return;

	studiohdr_t* hdr = g_ModelInfo->GetStudiomodel(nadeModel);

	if (!hdr)
		return;

	if (!strstr(hdr->name, "thrown") && !strstr(hdr->name, "dropped"))
		return;

	std::string nadeName = "Unknown Grenade";

	IMaterial* mats[32];
	g_ModelInfo->GetModelMaterials(nadeModel, hdr->numtextures, mats);

	for (int i = 0; i < hdr->numtextures; i++)
	{
		IMaterial* mat = mats[i];
		if (!mat)
			continue;

		if (strstr(mat->GetName(), "flashbang"))
		{
			nadeName = "Flashbang";
			break;
		}
		else if (strstr(mat->GetName(), "m67_grenade") || strstr(mat->GetName(), "hegrenade"))
		{
			nadeName = "HE";
			break;
		}
		else if (strstr(mat->GetName(), "smoke"))
		{
			nadeName = "Smoke";
			break;
		}
		else if (strstr(mat->GetName(), "decoy"))
		{
			nadeName = "Decoy";
			break;
		}
		else if (strstr(mat->GetName(), "incendiary") || strstr(mat->GetName(), "molotov"))
		{
			nadeName = "Molotov";
			break;
		}
	}

	BoxAndText(throwable, nadeName);
}

void visuals::BulletTrace(C_BaseEntity* pEntity, Color color)
{
	Vector src3D, dst3D, forward, src, dst;
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;
	Vector eyes = *pEntity->GetEyeAngles();

	AngleVectors(eyes, &forward);
	filter.pSkip = pEntity;
	src3D = pEntity->GetBonePos(6) - Vector(0, 0, 0);
	dst3D = src3D + (forward * 1);

	ray.Init(src3D, dst3D);

	g_EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	if (!g_Render->WorldToScreen(src3D, src) || !g_Render->WorldToScreen(tr.endpos, dst))
		return;


	g_Render->Line(src.x, src.y, dst.x, dst.y, color);
	g_Render->DrawOutlinedRect(dst.x - 3, dst.y - 3, 6, 6, color);
};

void visuals::DLight(C_BaseEntity *local, C_BaseEntity* entity)
{
    player_info_t pinfo;
    if (local && entity && entity != local)
    {
        if (g_Engine->GetPlayerInfo(entity->GetIndex(), &pinfo) && entity->IsAlive() && !entity->IsDormant())
        {
            if (local->GetTeamNum() != entity->GetTeamNum())
            {
                dlight_t* pElight = g_Dlight->CL_AllocElight(entity->GetIndex());
                pElight->origin = entity->GetOrigin() + Vector(0.0f, 0.0f, 35.0f);
                pElight->radius = 400.0f;
                pElight->color.b = int(g_Options.Colors.dlight_color[0] * 255);
                pElight->color.g = int(g_Options.Colors.dlight_color[1] * 255);
                pElight->color.r = int(g_Options.Colors.dlight_color[2] * 255);
                pElight->die = g_Globals->curtime + 0.05f;
				pElight->decay = pElight->radius; // / 5.0f;
                pElight->key = entity->GetIndex();

                dlight_t* pDlight = g_Dlight->CL_AllocDlight(entity->GetIndex());
                pDlight->origin = entity->GetOrigin();
                pDlight->radius = 400.0f;
                pDlight->color.b = int(g_Options.Colors.dlight_color[0] * 255);
                pDlight->color.g = int(g_Options.Colors.dlight_color[1] * 255);
                pDlight->color.r = int(g_Options.Colors.dlight_color[2] * 255);
                pDlight->die = g_Globals->curtime + 0.05f;
                pDlight->decay = pDlight->radius; // / 5.0f;
                pDlight->key = entity->GetIndex();
            }
        }
    }
}

void visuals::NightMode()
{
    if (g_Options.Misc.nightMode)
    {
        if (!done)
        {


            static auto sv_skyname = g_CVar->FindVar("sv_skyname");
            static auto r_DrawSpecificStaticProp = g_CVar->FindVar("r_DrawSpecificStaticProp");
            r_DrawSpecificStaticProp->SetValue(1);
            sv_skyname->SetValue("sky_csgo_night02");

            for (MaterialHandle_t i = g_MaterialSystem->FirstMaterial(); i != g_MaterialSystem->InvalidMaterial(); i = g_MaterialSystem->NextMaterial(i))
            {
                IMaterial *pMaterial = g_MaterialSystem->GetMaterial(i);

                if (!pMaterial)
                    continue;

                const char* group = pMaterial->GetTextureGroupName();
                const char* name = pMaterial->GetName();

                if (strstr(group, "World textures"))
                {
                    pMaterial->ColorModulate(0.10, 0.10, 0.10);
                }
                if (strstr(group, "StaticProp"))
                {
                    pMaterial->ColorModulate(0.30, 0.30, 0.30);
                }
                if (strstr(name, "models/props/de_dust/palace_bigdome"))
                {
                    pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
                }
                if (strstr(name, "models/props/de_dust/palace_pillars"))
                {
                    pMaterial->ColorModulate(0.30, 0.30, 0.30);
                }

                if (strstr(group, "Particle textures"))
                {
                    pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
                }
                done = true;
            }

        }
    }
    else
    {
        if (done)
        {
			static auto sv_skyname = g_CVar->FindVar("sv_skyname");
			sv_skyname->SetValue("vertigoblue_hdr");
            for (MaterialHandle_t i = g_MaterialSystem->FirstMaterial(); i != g_MaterialSystem->InvalidMaterial(); i = g_MaterialSystem->NextMaterial(i))
            {
                IMaterial *pMaterial = g_MaterialSystem->GetMaterial(i);

                if (!pMaterial)
                    continue;

                const char* group = pMaterial->GetTextureGroupName();
                const char* name = pMaterial->GetName();

                if (strstr(group, "World textures"))
                {
					
                    pMaterial->ColorModulate(1, 1, 1);
                }
                if (strstr(group, "StaticProp"))
                {

                    pMaterial->ColorModulate(1, 1, 1);
                }
                if (strstr(name, "models/props/de_dust/palace_bigdome"))
                {
                    pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
                }
                if (strstr(name, "models/props/de_dust/palace_pillars"))
                {

                    pMaterial->ColorModulate(1, 1, 1);
                }
                if (strstr(group, "Particle textures"))
                {
                    pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
                }

            }
            done = false;
        }
    }
	
}



void visuals::SpecList(C_BaseEntity *local)
{

    RECT scrn = g_Render->GetViewport();
    int kapi = 0;

    if (local)
    {
        for (int i = 0; i < g_EntityList->GetHighestEntityIndex(); i++)
        {
            // Get the entity
            C_BaseEntity *pEntity = g_EntityList->GetClientEntity(i);
            player_info_t pinfo;
            if (pEntity && pEntity != local)
            {
                if (g_Engine->GetPlayerInfo(i, &pinfo) && !pEntity->IsAlive() && !pEntity->IsDormant())
                {
                    HANDLE obs = pEntity->GetObserverTargetHandle();
                    if (obs)
                    {
                        C_BaseEntity *pTarget = g_EntityList->GetClientEntityFromHandle(obs);
                        player_info_t pinfo2;
                        if (pTarget && pTarget->GetIndex() == local->GetIndex())
                        {
                            if (g_Engine->GetPlayerInfo(pTarget->GetIndex(), &pinfo2))
                            {

                                g_Render->DrawString2(g_Render->font.watermark, scrn.right -300, (scrn.top) + (14 * kapi) + 18, Color(255, 0, 0, 255), FONT_LEFT, "%s", pinfo.name);
                                kapi++;
                            }
                        }
                    }
                }
            }
        }
    }
    g_Render->DrawString2(g_Render->font.watermark, scrn.right - 300, (scrn.top) + 8, Color(255, 255, 255, 255), FONT_LEFT, "Spectators :");
}





/*
void visuals::masterlooser()
{
RECT scrn = g_Render->GetViewport();
#define charenc( s ) ( s )
//Backtrack Start
g_Render->DrawString2(g_Render->font.ESP, scrn.left + 200, (scrn.top) + 20, Color::White(), FONT_LEFT, "%s", charenc("Backtracking: "));
if(g_Options.Legitbot.backtrack)
g_Render->DrawString2(g_Render->font.ESP, scrn.left + 275, (scrn.top) + 20, Color::Green(), FONT_LEFT, "%s", charenc("On"));
else
g_Render->DrawString2(g_Render->font.ESP, scrn.left + 275, (scrn.top) + 20, Color::Red(), FONT_LEFT, "%s", charenc("Off"));
//Backtrack End
//Visuals Start
g_Render->DrawString2(g_Render->font.ESP, scrn.left + 200, (scrn.top) + 35, Color::White(), FONT_LEFT, "%s", charenc("Visuals: "));
if (g_Options.Visuals.Enabled)
{
g_Render->DrawString2(g_Render->font.ESP, scrn.left + 275, (scrn.top) + 35, Color::Green(), FONT_LEFT, "%s", charenc("On"));
g_Render->DrawString2(g_Render->font.ESP, scrn.left + 200, (scrn.top) + 50, Color::White(), FONT_LEFT, "%s", charenc("ESP Box: "));
g_Render->DrawString2(g_Render->font.ESP, scrn.left + 200, (scrn.top) + 60, Color::White(), FONT_LEFT, "%s", charenc("Chams: "));
if (g_Options.Visuals.Enabled && g_Options.Visuals.Filter.EnemyOnly)
{
g_Render->DrawString2(g_Render->font.ESP, scrn.left + 275, (scrn.top) + 35, Color(0, 191, 255), FONT_LEFT, "%s", charenc("On - Enemies Only"));
}
//Box
if (g_Options.Visuals.Box)
{
g_Render->DrawString2(g_Render->font.ESP, scrn.left + 275, (scrn.top) + 50, Color::Green(), FONT_LEFT, "%s", charenc("On"));
}
else
{
g_Render->DrawString2(g_Render->font.ESP, scrn.left + 275, (scrn.top) + 50, Color::Red(), FONT_LEFT, "%s", charenc("Off"));
}

//Chams
if (g_Options.Visuals.Chams)
{

g_Render->DrawString2(g_Render->font.ESP, scrn.left + 275, (scrn.top) + 65, Color::Green(), FONT_LEFT, "%s", charenc("On"));
if (g_Options.Visuals.Chams && g_Options.Visuals.IsVisibleChams)
{
g_Render->DrawString2(g_Render->font.ESP, scrn.left + 275, (scrn.top) + 65, Color(0, 191, 255), FONT_LEFT, "%s", charenc("On - Visible Only"));
}
}
else
{
g_Render->DrawString2(g_Render->font.ESP, scrn.left + 275, (scrn.top) + 65, Color::Red(), FONT_LEFT, "%s", charenc("Off"));
}

}
else
g_Render->DrawString2(g_Render->font.ESP, scrn.left + 275, (scrn.top) + 35, Color::Red(), FONT_LEFT, "%s", charenc("Off"));
//Visuals End
//Misc Start
if (!g_Options.Visuals.Enabled)
{
g_Render->DrawString2(g_Render->font.ESP, scrn.left + 200, (scrn.top) + 50, Color::White(), FONT_LEFT, "%s", charenc("Bhop: "));
g_Render->DrawString2(g_Render->font.ESP, scrn.left + 200, (scrn.top) + 65, Color::White(), FONT_LEFT, "%s", charenc("Hands: "));
if (g_Options.Misc.Bhop)
g_Render->DrawString2(g_Render->font.ESP, scrn.left + 275, (scrn.top) + 50, Color::Green(), FONT_LEFT, "%s", charenc("On"));
else
g_Render->DrawString2(g_Render->font.ESP, scrn.left + 275, (scrn.top) + 50, Color::Red(), FONT_LEFT, "%s", charenc("Off"));
if (g_Options.Visuals.Hands)
{
if(g_Options.Visuals.Hands == 1)
g_Render->DrawString2(g_Render->font.ESP, scrn.left + 275, (scrn.top) + 65, Color(0, 191, 255), FONT_LEFT, "%s", charenc("NoHands"));
}
else
g_Render->DrawString2(g_Render->font.ESP, scrn.left + 275, (scrn.top) + 65, Color::Red(), FONT_LEFT, "%s", charenc("Off"));
}

if (g_Options.Visuals.Enabled)
{
g_Render->DrawString2(g_Render->font.ESP, scrn.left + 200, (scrn.top) + 80, Color::White(), FONT_LEFT, "%s", charenc("Bhop: "));
g_Render->DrawString2(g_Render->font.ESP, scrn.left + 200, (scrn.top) + 95, Color::White(), FONT_LEFT, "%s", charenc("Hands: "));

if (g_Options.Misc.Bhop)
g_Render->DrawString2(g_Render->font.ESP, scrn.left + 275, (scrn.top) + 80, Color::Green(), FONT_LEFT, "%s", charenc("On"));
else
g_Render->DrawString2(g_Render->font.ESP, scrn.left + 275, (scrn.top) + 80, Color::Red(), FONT_LEFT, "%s", charenc("Off"));
if (g_Options.Visuals.Hands)
{
if (g_Options.Visuals.Hands == 1)
g_Render->DrawString2(g_Render->font.ESP, scrn.left + 275, (scrn.top) + 95, Color(0, 191, 255), FONT_LEFT, "%s", charenc("NoHands"));
}
else
g_Render->DrawString2(g_Render->font.ESP, scrn.left + 275, (scrn.top) + 95, Color::Red(), FONT_LEFT, "%s", charenc("Off"));
}




}
*/


/*
CBaseEntity *pLocal = g_pEntitylist->GetClientEntity(g_pEngine->GetLocalPlayer());

Vector src3D, dst3D, forward, src, dst;
trace_t tr;
Ray_t ray;
CTraceFilter filter;

filter.pSkip = pLocal;

Math::AngleVectors(QAngle(0, pLocal->LowerBodyYaw(), 0), &forward);
src3D = pLocal->GetOrigin();
dst3D = src3D + (forward * 35.f); //replace 50 with the length you want the line to have

ray.Init(src3D, dst3D);

g_pEngineTrace->TraceRay(ray, 0, &filter, &tr);
if (!GameUtils::WorldToScreen(src3D, src) || !GameUtils::WorldToScreen(tr.endpos, dst))
return;

g_pRender->Line(src.x, src.y, dst.x, dst.y, BLUE(255));
g_pRender->Text("lby", dst.x, dst.y, righted, g_pRender->Fonts.esp, true, BLUE(255),BLACK(255));

Math::AngleVectors(QAngle(0, G::RealAngle.y, 0), &forward);
src3D = pLocal->GetOrigin();
dst3D = src3D + (forward * 35.f);

ray.Init(src3D, dst3D);

g_pEngineTrace->TraceRay(ray, 0, &filter, &tr);

if (!GameUtils::WorldToScreen(src3D, src) || !GameUtils::WorldToScreen(tr.endpos, dst))
return;

g_pRender->Line(src.x, src.y, dst.x, dst.y, GREEN(255));
g_pRender->Text("real", dst.x, dst.y, righted, g_pRender->Fonts.esp, true, GREEN(255), BLACK(255));

Math::AngleVectors(QAngle(0, G::FakeAngle.y, 0), &forward);
dst3D = src3D + (forward * 35.f);

ray.Init(src3D, dst3D);

g_pEngineTrace->TraceRay(ray, 0, &filter, &tr);

if (!GameUtils::WorldToScreen(src3D, src) || !GameUtils::WorldToScreen(tr.endpos, dst))
return;

g_pRender->Line(src.x, src.y, dst.x, dst.y, RED(255));
g_pRender->Text("fake", dst.x, dst.y, righted, g_pRender->Fonts.esp, true, RED(255), BLACK(255));
*/