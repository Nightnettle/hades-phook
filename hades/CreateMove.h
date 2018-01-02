#pragma once
#include "HookIncludes.h"
#include "Misc.h"
#include "MovementRecorder.h"
#include "LegitBot.h"
#include "RageBot.h"
#include "GrenadePrediction.h"
#include "LagComp.h"
//#include "FakeLag.h"
//---------------------------
//float OldLBY;
//float LBYBreakerTimer;
//float LastLBYUpdateTime;
//---------------------------
using create_move_t = void(__thiscall *)(IBaseClientDLL *, int, float, bool);
QAngle qLastTickAngles = QAngle(0.0f, 0.0f, 0.0f);
void __stdcall CHLCreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket)
{
    static auto ofunc = hooks::client.get_original<create_move_t>(21);
    ofunc(g_CHLClient, sequence_number, input_sample_frametime, active);
	C_BaseEntity *local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
	CInput::CUserCmd *cmd = g_Input->GetUserCmd(0, sequence_number);
	if (!cmd)
		return;

	static float next_time = 0;
	if (!Globals::ySwitch && local->IsAlive() && local->GetVelocity().Length2D() == 0)
	{
		//---------------------------------------------------------------
		/*
		float TickStuff = TICKS_TO_TIME(local->GetTickBase());

		Globals::shouldflip = false;

		bool moveFlip;

		Globals::NextTime = next_time;

		if (next_time - TICKS_TO_TIME(local->GetTickBase()) > 1.1)
		{
			next_time = 0;
		}

		if (local->GetVelocity().Length2D() > 1)
		{
			moveFlip = true;
		}

		if (local->GetVelocity().Length2D() == 0 && !moveFlip)
		{
			if (TickStuff > next_time + 1.1f  - GetOutgoingLatency())
			{
				next_time = TickStuff;
				Globals::shouldflip = true;
			}
		}
		else if (moveFlip && local->GetVelocity().Length2D() == 0)
		{
			if (TickStuff > next_time + 0.9f - GetOutgoingLatency())
			{
				next_time = TickStuff;
				Globals::shouldflip = true;
				moveFlip = false;
			}
		}
		*/
		//---------------------------------------------------------------
		/*
		float flServerTime = (float)(local->GetTickBase()  * g_Globals->interval_per_tick);

		if (OldLBY != local->GetLowerBodyYaw())
		{
			LBYBreakerTimer++;
			OldLBY = local->GetLowerBodyYaw();
			//bSwitch = !bSwitch;
			LastLBYUpdateTime = flServerTime;
		}

		if (local->GetVelocity().Length2D() > 0.1)
		{
			LastLBYUpdateTime = flServerTime;
			Globals::shouldflip = false;
		}


		if ((LastLBYUpdateTime + 1.1 - GetOutgoingLatency() < flServerTime) && (local->GetFlags() & FL_ONGROUND))
		{
			if (LastLBYUpdateTime + 0.9 - GetOutgoingLatency() < flServerTime)
			{
				LastLBYUpdateTime = flServerTime;
			}
			Globals::shouldflip = true;
		}
		Globals::shouldflip = false;
		*/
		//---------------------------------------------------------------
	}

    if (!cmd->command_number)
        return;

	CInput::CVerifiedUserCmd *verifiedCommands = *(CInput::CVerifiedUserCmd **)(reinterpret_cast< uint32_t >(g_Input) + 0xF0);
	CInput::CVerifiedUserCmd *verified = &verifiedCommands[sequence_number % 150];
	if (!verified)
		return;

    if (!local)
        return;
    QAngle oldAngle = cmd->viewangles;
    float oldForward = cmd->forwardmove;
    float oldSideMove = cmd->sidemove;
	
    backtracking->legitBackTrack(cmd, local);
	backtracking->RageBackTrack(cmd, local);
	
	if(g_Options.Misc.ServerRankRevealAll)
    RankReveal(cmd);

	//Chrome();
	if(g_Options.Misc.autostrafe)
		misc::AutoStrafe(cmd, local, cmd->viewangles);

    misc::instance().OnCreateMove(cmd, local);
    legitbot::instance().OnCreateMove(cmd, local);
	ragebot::instance().OnCreateMove(cmd, bSendPacket);
	grenade_prediction::instance().Tick(cmd->buttons);
	

	if (!bSendPacket && g_Options.Ragebot.YawFake) qLastTickAngles = cmd->viewangles;
	else if (bSendPacket && !g_Options.Ragebot.YawFake) qLastTickAngles = cmd->viewangles;

        
    if (!sanitize_angles(cmd->viewangles))
        return;

    movementfix(oldAngle, cmd);
    clampMovement(cmd);

	verified->m_cmd = *cmd;
	verified->m_crc = cmd->GetChecksum();

	if (Globals::ySwitch) {
		Globals::FakeAngle = cmd->viewangles.y;
	}
	else {
		Globals::RealAngle = cmd->viewangles.y;
	}
}
#pragma warning(disable : 4409)
__declspec(naked) void __stdcall hkCreateMove(int sequence_number, float input_sample_frametime, bool active)
{
	__asm
	{
		PUSH	EBP
		MOV		EBP, ESP
		PUSH	EBX
		LEA		ECX, [ESP]
		PUSH	ECX
		PUSH	active
		PUSH	input_sample_frametime
		PUSH	sequence_number
		CALL	CHLCreateMove
		POP		EBX
		POP		EBP
		RETN	0xC
	}
}



