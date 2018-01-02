#pragma once
#include <Windows.h>
#pragma comment(lib, "Winmm.lib") 
#include "Listener.h"
#include "hitsounds.h"


class hitmarker
{
	class player_hurt_listener : public IGameEventListener2
	{
	public:
		void start()
		{
			if (!g_EventManager->AddListener(this, "player_hurt", false)) {
				throw std::exception("Failed to register the event");
			}
		}
		void stop()
		{
			g_EventManager->RemoveListener(this);
		}
		void FireGameEvent(IGameEvent *event) override
		{
			hitmarker::singleton()->on_fire_event(event);
		}
		int GetEventDebugID() override
		{
			return 0x2A;
		}
	};
public:
	static hitmarker* singleton()
	{
		static hitmarker* instance = new hitmarker;
		return instance;
	}

	void initialize()
	{
		_listener.start();
	}
	void on_fire_event(IGameEvent* event)
	{
		if (!strcmp(event->GetName(), "player_hurt")) {
			int attacker = event->GetInt("attacker");
			if (g_Engine->GetPlayerForUserID(attacker) == g_Engine->GetLocalPlayer() && g_Options.Misc.hitmarker)
			{
						if (g_Engine->GetPlayerForUserID(attacker) == g_Engine->GetLocalPlayer() && g_Options.Misc.hitmarker)
			{
				_flHurtTime = g_Globals->curtime;
			}
			if (g_Engine->GetPlayerForUserID(attacker) == g_Engine->GetLocalPlayer() && g_Options.Misc.hitsound)
			{
				PlaySoundA(skeet, NULL, SND_ASYNC | SND_MEMORY);
			}

			}

		}
	}

	void on_paint()
	{
		auto curtime = g_Globals->curtime;
		auto lineSize = 6;
		if (_flHurtTime + 0.25f >= curtime)
		{
			int screenSizeX, screenCenterX;
			int screenSizeY, screenCenterY;
			g_Engine->GetScreenSize(screenSizeX, screenSizeY);

			screenCenterX = screenSizeX / 2;
			screenCenterY = screenSizeY / 2;

			g_Surface->DrawSetColor(int(g_Options.Colors.hit_color[0] * 255.f), int(g_Options.Colors.hit_color[1] * 255.f), int(g_Options.Colors.hit_color[2] * 255.f),255);
			g_Surface->DrawLine(screenCenterX - lineSize, screenCenterY - lineSize, screenCenterX - (lineSize / 4), screenCenterY - (lineSize / 4));
			g_Surface->DrawLine(screenCenterX - lineSize, screenCenterY + lineSize, screenCenterX - (lineSize / 4), screenCenterY + (lineSize / 4));
			g_Surface->DrawLine(screenCenterX + lineSize, screenCenterY + lineSize, screenCenterX + (lineSize / 4), screenCenterY + (lineSize / 4));
			g_Surface->DrawLine(screenCenterX + lineSize, screenCenterY - lineSize, screenCenterX + (lineSize / 4), screenCenterY - (lineSize / 4));
		}
	}

private:
	player_hurt_listener    _listener;
	float                   _flHurtTime;
};