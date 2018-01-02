#pragma once
#include "singleton.hpp"
#include "MiscClasses.h"
#include "Interfaces.h"
//#include "hitsounds.h"
#include <Windows.h>
#pragma comment(lib, "Winmm.lib") 

class item_purchase
    : public singleton<item_purchase>
{
    class item_purchase_listener
        : public IGameEventListener2
    {
    public:
        void start()
        {
            g_EventManager->AddListener(this, "item_purchase", false);
			g_EventManager->AddListener(this, "player_hurt", false);
        }
        void stop()
        {
            g_EventManager->RemoveListener(this);
        }
        void FireGameEvent(IGameEvent *event) override
        {
            singleton()->on_fire_event(event);
        }
        int GetEventDebugID(void) override
        {
            return 42 /*0x2A*/;
        }
    };
public:
    static item_purchase* singleton()
    {
        static item_purchase* instance = new item_purchase;
        return instance;
    }

    void initialize()
    {
        listener.start();
    }

    void remove()
    {
        listener.stop();
    }

    void on_fire_event(IGameEvent* event)
    {
        C_BaseEntity* local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
        if (!strcmp(event->GetName(), "item_purchase"))
        {
            auto buyer = event->GetInt("userid");
            std::string gun = event->GetString("weapon");

            if (strstr(gun.c_str(), "molotov")
                || strstr(gun.c_str(), "nade")
                || strstr(gun.c_str(), "kevlar")
                || strstr(gun.c_str(), "decoy")
                || strstr(gun.c_str(), "suit")
                || strstr(gun.c_str(), "flash")
                || strstr(gun.c_str(), "vest")
                || strstr(gun.c_str(), "cutter")
                || strstr(gun.c_str(), "defuse")
                )  return;
            auto player_index = g_Engine->GetPlayerForUserID(buyer);
            C_BaseEntity* player = (C_BaseEntity*)g_EntityList->GetClientEntity(player_index);
            player_info_t pinfo;
            
            if (player && local &&  player->GetTeamNum() != local->GetTeamNum() && g_Engine->GetPlayerInfo(player_index, &pinfo))
            {

                    gun.erase(gun.find("weapon_"), 7);
					g_Render->Text(10, 6, Color(255, 255, 255, 255), g_Render->font.Time, " ""\x04""%s bought %s\n", pinfo.name, gun.c_str());

            }
        }
		if (!strcmp(event->GetName(), "player_hurt"))
		{
			int deadfag = event->GetInt("userid");
			int attackingfag = event->GetInt("attacker");
			C_BaseEntity* pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
			if (g_Engine->GetPlayerForUserID(deadfag) != g_Engine->GetLocalPlayer() && g_Engine->GetPlayerForUserID(attackingfag) == g_Engine->GetLocalPlayer())
			{
				C_BaseEntity* hittedplayer = (C_BaseEntity*)(g_Engine->GetPlayerForUserID(deadfag));
				int hit = event->GetInt("hitgroup");
				if (hit >= 0 && hittedplayer && deadfag && attackingfag)
				{
					Globals::missedshots++;
				}
				else
				{
					Globals::missedshots++;
				}
			}
		}
	
    }


private:
    item_purchase_listener  listener;

};
