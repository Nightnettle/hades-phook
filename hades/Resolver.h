#pragma once
#include "HookIncludes.h"

int GetEstimatedServerTickCount(float latency)
{
	return (int)floorf(float((float)((float)(latency) / (float)((uintptr_t)&g_Globals->interval_per_tick)) + 0.5) + 1 + (int)((uintptr_t)&g_Globals->tickcount));
}

float oldlowerbodyyaw = 0;

inline float ClampYaw(float yaw) {
	while (yaw > 180.f)
		yaw -= 360.f;
	while (yaw < -180.f)
		yaw += 360.f;
	return yaw;
}

namespace Globals
{
	int Shots;
	int missedshots;
	float RealAngle;
	float FakeAngle;
	Vector AimPoint;
	bool shouldflip;
	bool ySwitch;
	float NextTime;
	int resolvemode = 1;
	float fakeAngle;
	float OldSimulationTime[65];
	bool error;
}
//hitting p xd
void Resolver()
{
	if (g_Options.Ragebot.Resolver)
	{
		C_BaseEntity *pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
		{
			for (auto i = 0; i < g_EntityList->GetHighestEntityIndex(); i++)
			{
				C_BaseEntity* pEnt = g_EntityList->GetClientEntity(i);

				if (!pEnt) continue;

				if (pEnt == pLocal) continue;

				if (pEnt->IsDormant()) continue;

				player_info_t pTemp;

				if (!g_Engine->GetPlayerInfo(i, &pTemp))
					continue;

				if (pEnt->GetTeamNum() == pLocal->GetTeamNum()) continue;

				auto FYaw = pEnt->GetLowerBodyYaw();
				auto pitch = pEnt->GetEyeAngles()->x;

				pEnt->GetEyeAngles()->y = FYaw;
				float PlayerIsMoving = abs(pEnt->GetVelocity().Length2D());
				bool bLowerBodyUpdated = false;
				bool IsBreakingLBY = false;

				bool isFakeHeading = false;

				float oldLBY = pEnt->GetLowerBodyYaw();


				if (oldLBY != pEnt->GetLowerBodyYaw())
				{
					bLowerBodyUpdated = true;
				}
				else
				{
					bLowerBodyUpdated = false;
				}

				if (pEnt->GetEyeAngles()->y - pEnt->GetLowerBodyYaw() > 35)
				{
					isFakeHeading = true;
				}
				else
				{
					isFakeHeading = false;
				}

				float bodyeyedelta = fabs(pEnt->GetEyeAngles()->y - pEnt->GetLowerBodyYaw());
				if (PlayerIsMoving || bLowerBodyUpdated)// || LastUpdatedNetVars->eyeangles.x != CurrentNetVars->eyeangles.x || LastUpdatedNetVars->eyeyaw != CurrentNetVars->eyeangles.y)
				{
					Globals::resolvemode = 3;

					pEnt->GetEyeAngles()->y = FYaw;
					oldLBY = pEnt->GetEyeAngles()->y;

					IsBreakingLBY = false;
				}
				else
				{
					Globals::resolvemode = 1;

					if (bodyeyedelta == 0.f && pEnt->GetVelocity().Length2D() >= 0 && pEnt->GetVelocity().Length2D() < 36)
					{
						pEnt->GetEyeAngles()->y = oldLBY;
						IsBreakingLBY = true;
					}
					else
					{
						IsBreakingLBY = false;
					}
				}

				if (IsBreakingLBY)
				{
					Globals::resolvemode = 2;

					pEnt->GetEyeAngles()->y = oldLBY;

					switch (Globals::Shots % 3)
					{
					case 1: pEnt->GetEyeAngles()->y = 180; break;
					case 2: pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw() + rand() % 180;
					case 3: pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw() + pEnt->GetEyeAngles()->y + rand() % 35;
					}
				}
				else if (!IsBreakingLBY && !isFakeHeading)
				{
					Globals::resolvemode = 3;

					pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw();

					switch (Globals::Shots % 4)
					{
					case 1: pEnt->GetEyeAngles()->y = 45 + rand() % 180;
					case 2: pEnt->GetEyeAngles()->y = oldLBY + rand() % 90;
					case 3: pEnt->GetEyeAngles()->y = 180 + rand() % 90;
					case 4: pEnt->GetEyeAngles()->y = oldLBY + pEnt->GetEyeAngles()->y + rand() % 45;
					}
				}
				else if (isFakeHeading)
				{
					Globals::resolvemode = 2;

					pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw() - pEnt->GetEyeAngles()->y;

					switch (Globals::Shots % 2)
					{
					case 1: pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw() + rand() % 90; break;
					case 2: pEnt->GetEyeAngles()->y = pEnt->GetEyeAngles()->y + rand() % 360; break;
					}
				}
				else if (Globals::Shots >= g_Options.Ragebot.bruteAfterX && g_Options.Ragebot.bruteAfterX != 0)
				{
					Globals::resolvemode = 2;

					pEnt->GetEyeAngles()->y = 180;

					switch (Globals::Shots % 4)
					{
					case 1: pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw(); break;
					case 2: pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw() + rand() % 90; break;
					case 3: pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw() + rand() % 180; break;
					case 4: pEnt->GetEyeAngles()->y = oldLBY + rand() % 45; break;
					}
				}
				else
				{
					Globals::resolvemode = 1;

					pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw() + rand() % 180;

					switch (Globals::Shots % 13)
					{
					case 1: pEnt->GetEyeAngles()->y = 180; break;
					case 2: pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw() + rand() % 180;
					case 3: pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw() + pEnt->GetEyeAngles()->y + rand() % 35;
					case 4: pEnt->GetEyeAngles()->y = 45 + rand() % 180;
					case 5: pEnt->GetEyeAngles()->y = oldLBY + rand() % 90;
					case 6: pEnt->GetEyeAngles()->y = 180 + rand() % 90;
					case 7: pEnt->GetEyeAngles()->y = oldLBY + pEnt->GetEyeAngles()->y + rand() % 45;
					case 8: pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw(); break;
					case 9: pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw() + rand() % 90; break;
					case 10: pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw() + rand() % 180; break;
					case 11: pEnt->GetEyeAngles()->y = oldLBY + rand() % 45; break;
					case 12: pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw() + rand() % 90; break;
					case 13: pEnt->GetEyeAngles()->y = pEnt->GetEyeAngles()->y + rand() % 360; break;
					}
				}
			}
		}
	}

}

float Bolbilize(float Yaw)
{
	if (Yaw > 180)
	{
		Yaw -= (round(Yaw / 360) * 360.f);
	}
	else if (Yaw < -180)
	{
		Yaw += (round(Yaw / 360) * -360.f);
	}
	return Yaw;
}

//You can give it a go ;)
void Resolver1()
{
	
	if (g_Options.Ragebot.Resolver)
	{
		C_BaseEntity *pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
		for (int i = 1; i < 65; i++)
		{
			if (pLocal->IsAlive())
			{
				C_BaseEntity* pEnt = g_EntityList->GetClientEntity(i);
				if (!pEnt) continue;
				if (pEnt->IsDormant()) continue;
				if (pEnt->GetHealth() < 1) continue;
				if (pEnt->GetLifeState() != 0) continue;

				static bool isMoving;
				float PlayerIsMoving = abs(pEnt->GetVelocity().Length());
				if (PlayerIsMoving > 36) isMoving = true;
				else if (PlayerIsMoving <= 36) isMoving = false;

				bool MeetsLBYReq;
				if (pEnt->GetFlags() & FL_ONGROUND)
					MeetsLBYReq = true;
				else
					MeetsLBYReq = false;

				float newsimtime;
				float storedsimtime;
				bool lbyupdated;
				float storedlbyFGE;
				float storedanglesFGE;
				float storedsimtimeFGE;
				bool didhitHS;
				float newlby;
				float newdelta;
				float newlbydelta;
				float finaldelta;
				float finallbydelta;
				float storedlby[64];
				float storeddelta[64];
				float storedlbydelta[64];
				float StoredAngles[64];
				float StoredLBY[65];
				
				newlby = pEnt->GetLowerBodyYaw();
				newsimtime = pEnt->GetSimulationTime();
				newdelta = *(float*)((DWORD)pEnt + pEnt->GetEyeAngles());
				newlbydelta = pEnt->GetLowerBodyYaw();
				finaldelta = newdelta - storeddelta[pEnt->GetIndex()];
				finallbydelta = newlbydelta - storedlbydelta[pEnt->GetIndex()];
				StoredAngles[pEnt->GetIndex()] = *(float*)((DWORD)pEnt + pEnt->GetEyeAngles());
				storedlby[pEnt->GetIndex()] = pEnt->GetLowerBodyYaw();
				storedlbydelta[pEnt->GetIndex()] = pEnt->GetLowerBodyYaw();
				storedsimtime = pEnt->GetSimulationTime();
				storeddelta[pEnt->GetIndex()] = *(float*)((DWORD)pEnt + pEnt->GetEyeAngles());
				storedanglesFGE = *(float*)((DWORD)pEnt + pEnt->GetEyeAngles());
				storedlbyFGE = pEnt->GetLowerBodyYaw();
				storedsimtimeFGE = pEnt->GetSimulationTime();

				if (newlby == *storedlby)
					lbyupdated = false;
				else
					lbyupdated = true;


				static float time_at_update[65];
				int index = pEnt->GetIndex();
				float simTime_since_lby_update = pEnt->GetSimulationTime() - time_at_update[index];

				Vector reset = Vector(0, 0, 0); reset.y = pEnt->GetEyeAngles()->y;
				static float LatestLowerBodyYawUpdateTime[55];
				float lby = pEnt->GetLowerBodyYaw();
				static bool bLowerBodyIsUpdated;
				if (pEnt->GetLowerBodyYaw() != StoredLBY[pEnt->GetIndex()]) bLowerBodyIsUpdated = true;
				else bLowerBodyIsUpdated = false;
				float bodyeyedelta = *(float*)((DWORD)pEnt + pEnt->GetEyeAngles()) - pEnt->GetLowerBodyYaw();
				float curTime_since_lby_update = LatestLowerBodyYawUpdateTime[pEnt->GetIndex()] - g_Globals->curtime;
				float LastUpdatedLBY = 0.f;
				bool bClientMoving = false;
				if (pEnt->GetVelocity().Length2D() > 0.1f) bClientMoving = true;
				else bClientMoving = false;
				float flLastUpdateTime = LatestLowerBodyYawUpdateTime[pEnt->GetIndex()] - g_Globals->curtime;

				if (bLowerBodyIsUpdated || isMoving || fabsf(bodyeyedelta) >= 35.0f)
				{
					*(float*)((DWORD)pEnt + pEnt->GetEyeAngles()) = pEnt->GetLowerBodyYaw();
					LatestLowerBodyYawUpdateTime[pEnt->GetIndex()] = g_Globals->curtime;
					StoredLBY[pEnt->GetIndex()] = pEnt->GetLowerBodyYaw();
				}
				else if (Globals::missedshots >= 2)
				{
					*(float*)((DWORD)pEnt + pEnt->GetEyeAngles()) = lby + 180;
				}
				else if ((curTime_since_lby_update) >= 1.1 || simTime_since_lby_update >= 1.1f || (1.1 - flLastUpdateTime) < 0.2)
				{
					*(float*)((DWORD)pEnt + pEnt->GetEyeAngles()) = lby + 180;
				}
				else if (simTime_since_lby_update <= 0.57f)
				{
					*(float*)((DWORD)pEnt + pEnt->GetEyeAngles()) = lby + bodyeyedelta; 
				}
				else
				{
					switch (Globals::Shots % 3)
					{
					case 0: *(float*)((DWORD)pEnt + pEnt->GetEyeAngles()) = Bolbilize(CalcAngle(pEnt->GetOrigin(), pLocal->GetOrigin()).y + 70); break;
					case 1: *(float*)((DWORD)pEnt + pEnt->GetEyeAngles()) = Bolbilize(CalcAngle(pEnt->GetOrigin(), pLocal->GetOrigin()).y + 180); break;
					case 2: *(float*)((DWORD)pEnt + pEnt->GetEyeAngles()) = Bolbilize(CalcAngle(pEnt->GetOrigin(), pLocal->GetOrigin()).y - 70); break;
					}
				}
				
			}
		}
	}
}