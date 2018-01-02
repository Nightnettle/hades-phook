#pragma once
#include "IDirect3DDevice9.h"
#include "Themes.h"
#include "Variables.h"

typedef void(*CL_FullUpdate_t) (void);
CL_FullUpdate_t CL_FullUpdate = nullptr;



void KnifeApplyCallbk()
{
	static auto CL_FullUpdate = reinterpret_cast<CL_FullUpdate_t>(U::FindPattern("engine.dll", reinterpret_cast<PBYTE>("\xA1\x00\x00\x00\x00\xB9\x00\x00\x00\x00\x56\xFF\x50\x14\x8B\x34\x85"), "x????x????xxxxxxx"));
	CL_FullUpdate();
}
void SetClanTag(const char* tag, const char* name)
{
	static auto ClanTagOffset = U::FindPattern("engine.dll", (PBYTE)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15", "xxxxxxxxx");
	if (ClanTagOffset)
	{
		if (strlen(name) > 0) {
			auto name_ = std::string(name);
			auto newline = name_.find("\\n");
			auto tab = name_.find("\\t");
			if (newline != std::string::npos) {
				name_.replace(newline, newline + 2, "\n");
			}
			if (tab != std::string::npos) {
				name_.replace(tab, tab + 2, "\t");
			}
		}
		auto tag_ = std::string(tag);
		if (strlen(tag) > 0) {
			auto newline = tag_.find("\\n");
			auto tab = tag_.find("\\t");
			if (newline != std::string::npos) {
				tag_.replace(newline, newline + 2, "\n");
			}
			if (tab != std::string::npos) {
				tag_.replace(tab, tab + 2, "\t");
			}
		}
		static auto SetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(ClanTagOffset);
		SetClanTag(tag_.data(), tag_.data());
	}
}


/*
void RenderMain()
{

	
	if (ImGui::CollapsingHeader("Legit"))
	{
		ImGui::Checkbox(("Active"), &g_Options.Legitbot.MainSwitch);
		ImGui::SameLine(150);
		ImGui::Checkbox(("Enable Aimbot"), &g_Options.Legitbot.Aimbot.Enabled);
		ImGui::Separator();
		ImGui::Checkbox(("Backtracking"), &g_Options.Legitbot.backtrack);
		ImGui::SameLine(150);
		ImGui::Checkbox(("Backtracking Dot"), &g_Options.Legitbot.memedot);
		ImGui::Separator();
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Memetrack them back to mexico.");
		if (ImGui::TreeNode("Pistols"))
		{
			ImGui::Hotkey("Key##1", &g_Options.Legitbot.PistolKey, ImVec2(150, 20));
			ImGui::SliderFloat("Smooth##1", &g_Options.Legitbot.PistolSmooth, 1.00f, 100.00f, "%.2f");
			ImGui::SliderFloat("FOV##1", &g_Options.Legitbot.Pistolfov, 0.00f, 30.00f, "%.2f");
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Rifles"))
		{
			ImGui::Hotkey("Key##0", &g_Options.Legitbot.MainKey, ImVec2(150, 20));
			ImGui::SliderFloat("Smooth##0", &g_Options.Legitbot.MainSmooth, 1.00f, 100.00f, "%.2f");
			ImGui::SliderFloat("FOV##0", &g_Options.Legitbot.Mainfov, 0.00f, 30.00f, "%.2f");
			ImGui::SliderFloat("Min RCS##0", &g_Options.Legitbot.main_recoil_min, 1.00f, 100.00f, "%.2f");
			ImGui::SliderFloat("Max RCS##0", &g_Options.Legitbot.main_recoil_max, 1.00f, 100.00f, "%.2f");
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Triggerbot"))
		{
			ImGui::Checkbox(("Enable Triggerbot"), &g_Options.Legitbot.Triggerbot.Enabled);
			ImGui::SliderFloat(("Delay"), &g_Options.Legitbot.Triggerbot.Delay, 1.f, 150.f, "%.0f");
			ImGui::Hotkey("Key##3", &g_Options.Legitbot.Triggerbot.Key, ImVec2(150, 20));
			if (ImGui::TreeNode("Filter"))
			{
				ImGui::Selectable((" Head"), &g_Options.Legitbot.Triggerbot.Filter.Head);
				ImGui::Selectable((" Arms"), &g_Options.Legitbot.Triggerbot.Filter.Arms);
				ImGui::Selectable((" Chest"), &g_Options.Legitbot.Triggerbot.Filter.Chest);
				ImGui::Selectable((" Stomach"), &g_Options.Legitbot.Triggerbot.Filter.Stomach);
				ImGui::Selectable((" Legs"), &g_Options.Legitbot.Triggerbot.Filter.Legs);
				
			}
			ImGui::TreePop();
		}


	}
	if (ImGui::CollapsingHeader("Visuals"))
	{
		ImGui::Checkbox((" Active"), &g_Options.Visuals.Enabled);
		ImGui::SameLine(150);
		ImGui::Checkbox((" Enemies Only"), &g_Options.Visuals.Filter.EnemyOnly);
		ImGui::Separator();
		ImGui::Checkbox(("Box"), &g_Options.Visuals.Box);
		ImGui::SameLine(150);
		ImGui::Checkbox(("IsVisible"), &g_Options.Visuals.IsVisibleBox);
		ImGui::Separator();
		ImGui::Checkbox(("Chams"), &g_Options.Visuals.Chams);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Wallhack Chams.");
		ImGui::SameLine(150);
		ImGui::Checkbox(("IsVisible "), &g_Options.Visuals.IsVisibleChams);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Visible Only Chams.");
		ImGui::Separator();
		ImGui::Checkbox(("HP"), &g_Options.Visuals.HP);
		ImGui::SameLine(150);
		ImGui::Checkbox(("Skelton"), &g_Options.Visuals.Skeleton);
		ImGui::Checkbox(("Name"), &g_Options.Visuals.Name);
		ImGui::SameLine(150);
		ImGui::Checkbox(("Weapon"), &g_Options.Visuals.Weapon);
		ImGui::Separator();
		ImGui::Checkbox(("DLight"), &g_Options.Visuals.DLight);
		ImGui::SameLine(150);
		ImGui::Checkbox(("GrenadeESP"), &g_Options.Visuals.GrenadeESP);
		ImGui::Checkbox(("C4"), &g_Options.Visuals.C4);
		ImGui::SameLine(150);
		ImGui::Checkbox("Glow", &g_Options.Visuals.Glow);
		ImGui::Checkbox(("C4 - World"), &g_Options.Visuals.C4World);
		ImGui::SameLine(150);
		ImGui::Checkbox(("Weapons - World"), &g_Options.Visuals.WeaponsWorld);

		if (ImGui::TreeNode("Misc Visuals"))
		{

			ImGui::Combo(("Hands"), &g_Options.Visuals.Hands, handd, ARRAYSIZE(handd));
			ImGui::SliderFloat(("FOV"), &g_Options.Visuals.FOVChanger, 0.f, 60.f, "%.0f");
			ImGui::SliderFloat(("V-FOV"), &g_Options.Visuals.viewmodelChanger, 68.f, 120.f, "%.0f");
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Viewmodel FOV.");
			ImGui::Separator();
			ImGui::Checkbox(("NoFlash"), &g_Options.Visuals.NoFlash);
			ImGui::SameLine(150);
			ImGui::Checkbox(("NoSmoke"), &g_Options.Visuals.NoSmoke);
			ImGui::Checkbox(("Time"), &g_Options.Visuals.Time);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Display the current system time.");
			ImGui::SameLine(150);
			ImGui::Checkbox(("NoScope Border"), &g_Options.Visuals.noscopeborder);
			ImGui::Checkbox(("Sniper Crosshair"), &g_Options.Visuals.crosshair);

			ImGui::TreePop();
		}
	}
	
	if (ImGui::CollapsingHeader("Misc"))
	{

			ImGui::Checkbox(("Bunny Hop"), &g_Options.Misc.Bhop);
			ImGui::SameLine(150);
			ImGui::Checkbox(("Reveal Ranks"), &g_Options.Misc.ServerRankRevealAll);
			ImGui::Checkbox(("NightMode"), &g_Options.Misc.nightMode);
			ImGui::SameLine(150);
			ImGui::Checkbox(("ChromeModels"), &g_Options.Misc.chrome);
			ImGui::Checkbox(("Grenade-Pred"), &g_Options.Visuals.GrenadePrediction);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Grenade Prediction.");
			ImGui::SameLine(150);
			ImGui::Checkbox(("ThirdPerson"), &g_Options.Visuals.ThirdPerson);
			ImGui::Checkbox(("SpecList"), &g_Options.Misc.SpecList);
			ImGui::SameLine(150);
			ImGui::Checkbox(("Auto-Accept"), &g_Options.Misc.AutoAccept);
			ImGui::Checkbox(("Draw Inaccuracy"), &g_Options.Misc.inaccuracy);
			ImGui::SameLine(150);
			ImGui::Checkbox(("Asus Walls"), &g_Options.Misc.asus);
			ImGui::PushItemWidth(50);
			if (ImGui::Button("Unhook"))
			{
				g_Engine->ClientCmd_Unrestricted("cl_mouseenable 1");
				unload = true;
			}
			ImGui::PopItemWidth();

	}
if (ImGui::CollapsingHeader("Skin Changer"))
{
	ImGui::Checkbox(("Enabled"), &g_Options.Skinchanger.Enabled);
	ImGui::SameLine(150);
	ImGui::PushItemWidth(150);
	if (ImGui::Button(("Force Update")))
	{
		KnifeApplyCallbk();
	}
	ImGui::PopItemWidth();
	if (ImGui::TreeNode(" Knife / Gloves"))
	{
		ImGui::Combo(("Knife Model"), &g_Options.Skinchanger.Knife, knives, ARRAYSIZE(knives));
		ImGui::Combo(("Knife Skin"), &g_Options.Skinchanger.KnifeSkin, knifeskins, ARRAYSIZE(knifeskins));
		ImGui::Combo(("Gloves"), &g_Options.Skinchanger.gloves, gloves, ARRAYSIZE(gloves));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode(" Pistols"))
	{
		ImGui::Combo(("Glock-18"), &g_Options.Skinchanger.GlockSkin, glock, ARRAYSIZE(glock));
		ImGui::Combo(("USP-S"), &g_Options.Skinchanger.USPSkin, usp, ARRAYSIZE(usp));
		ImGui::Combo(("Deagle"), &g_Options.Skinchanger.DeagleSkin, deagle, ARRAYSIZE(deagle));
		ImGui::Combo(("TEC-9"), &g_Options.Skinchanger.tec9Skin, tec9, ARRAYSIZE(tec9));
		ImGui::Combo(("P2000"), &g_Options.Skinchanger.P2000Skin, p2000, ARRAYSIZE(p2000));
		ImGui::Combo(("P250"), &g_Options.Skinchanger.P250Skin, p250, ARRAYSIZE(p250));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode(" SMGs"))
	{
		ImGui::Combo(("UMP45"), &g_Options.Skinchanger.UMP45Skin, ump45, ARRAYSIZE(ump45));
		ImGui::Combo(("P90"), &g_Options.Skinchanger.P90Skin, p90, ARRAYSIZE(p90));
		//ImGui::Combo(("MP9"), &g_Options.Skinchanger.MP9Skin, mp9, ARRAYSIZE(mp9));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode(" Rifles"))
	{
		ImGui::Combo(("AK-47"), &g_Options.Skinchanger.AK47Skin, ak47, ARRAYSIZE(ak47));
		ImGui::Combo(("M4A1-S"), &g_Options.Skinchanger.M4A1SSkin, m4a1s, ARRAYSIZE(m4a1s));
		ImGui::Combo(("M4A4"), &g_Options.Skinchanger.M4A4Skin, m4a4, ARRAYSIZE(m4a4));
		ImGui::Combo(("AUG"), &g_Options.Skinchanger.AUGSkin, aug, ARRAYSIZE(aug));
		ImGui::Combo(("FAMAS"), &g_Options.Skinchanger.FAMASSkin, famas, ARRAYSIZE(famas));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode(" Snipers"))
	{
		ImGui::Combo(("AWP"), &g_Options.Skinchanger.AWPSkin, awp, ARRAYSIZE(awp));
		ImGui::Combo(("SSG08"), &g_Options.Skinchanger.SSG08Skin, ssg08, ARRAYSIZE(ssg08));
		ImGui::Combo(("SCAR20"), &g_Options.Skinchanger.SCAR20Skin, scar20, ARRAYSIZE(scar20));
		ImGui::TreePop();
	}


}

if (ImGui::CollapsingHeader("Configuration"))
{

	static char buf2[128] = "";
	static char def[128] = "";
	ImGui::PushItemWidth(100);
	if (ImGui::TreeNode("Custom Clantag"))
	{
		ImGui::PushItemWidth(150);
		ImGui::InputText(("##Clantag"), buf2, 16, ImGuiInputTextFlags_AllowTabInput);
		ImGui::PopItemWidth();
		if (ImGui::Button(("Change Clantag")))
		{
			SetClanTag(buf2, buf2);
		}
		if (ImGui::Button(("Clear Clantag")))
			SetClanTag(def, def);
		ImGui::TreePop();

	}
	ImGui::PopItemWidth();


	if (ImGui::TreeNode("Config"))
	{
		if (ImGui::Button("Save Config")) Config->Save();
		ImGui::SameLine();
		if (ImGui::Button("Load Config")) Config->Load();
		ImGui::TreePop();
	}
}
if (ImGui::CollapsingHeader("Colors"))
{
	ImGui::Text("Visible Only Chams Color : ");
		ImGui::ColorEdit3(("Chams"), g_Options.Colors.chams_color_vis);
	ImGui::Text("Hand Chams Color : ");
		ImGui::ColorEdit3(("Hands"), g_Options.Colors.hands_color);
			ImGui::SliderFloat(("Hands Alpha"),	&g_Options.Colors.hands_alpha, 0.f, 1.f, "%.0f");

	ImGui::Text("ESP Box Color : ");
		ImGui::ColorEdit3(("ESP"), (float*)&g_Options.Colors.box_color_t);
	//ImGui::Text("Hitmarker Color : ");
		//ImGui::ColorEdit3(("Hit"), (float*)&g_Options.Colors.hit_color);

}



}
*/

void drawConfig()
{
	ImGui::PushItemWidth(10);
	ImGui::Combo("Config", &g_Options.Menu.ConfigFile, configFiles, ARRAYSIZE(configFiles));
	ImGui::PopItemWidth();
	if (ImGui::Button("Save")) Config->Save();
	ImGui::SameLine();
	if (ImGui::Button("Load")) Config->Load();
	ImGui::SameLine();
	if (ImGui::Button("Reset")) Config->Setup();
}
void RenderVisuals()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
		ImGui::BeginChild("COL1", ImVec2(305, 313), true);
		{
			ImGui::Text(XorStr("ESP"));
			ImGui::Separator();
				ImGui::Checkbox(XorStr("Enabled"), &g_Options.Visuals.Enabled);
				ImGui::SameLine(150);
				ImGui::Checkbox(XorStr("Enemies Only"), &g_Options.Visuals.Filter.EnemyOnly);
				ImGui::Separator();
				ImGui::Checkbox(XorStr("Box"), &g_Options.Visuals.Box);
				ImGui::SameLine(150);
				ImGui::Checkbox(XorStr("IsVisible"), &g_Options.Visuals.IsVisibleBox);
				ImGui::Separator();
				ImGui::Checkbox(XorStr("Chams"), &g_Options.Visuals.Chams);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Wallhack Chams.");
				ImGui::SameLine(150);
				ImGui::Checkbox(XorStr("IsVisible "), &g_Options.Visuals.IsVisibleChams);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Visible Only Chams.");
				
				ImGui::Separator();
				ImGui::Text(XorStr("Draw"));
				ImGui::Separator();
				ImGui::Checkbox(XorStr("Skelton"), &g_Options.Visuals.Skeleton);
				ImGui::SameLine(150);
				ImGui::Checkbox(XorStr("Name"), &g_Options.Visuals.Name);
				
				ImGui::Checkbox(XorStr("Weapon"), &g_Options.Visuals.Weapon);
				ImGui::SameLine(150);
				ImGui::Checkbox(XorStr("HP"), &g_Options.Visuals.HP);
				ImGui::Separator();
				ImGui::Checkbox(XorStr("DLight"), &g_Options.Visuals.DLight);
				ImGui::SameLine(150);
				ImGui::Checkbox(XorStr("GrenadeESP"), &g_Options.Visuals.GrenadeESP);
				ImGui::Separator();
					ImGui::Checkbox(XorStr("C4"), &g_Options.Visuals.C4);
					ImGui::SameLine(150);
					ImGui::Checkbox(XorStr("Glow"), &g_Options.Visuals.Glow);
					ImGui::Checkbox(XorStr("C4 - World"), &g_Options.Visuals.C4World);
					ImGui::SameLine(150);
					ImGui::Checkbox(XorStr("Weapons - World"), &g_Options.Visuals.WeaponsWorld);
					


				
			}ImGui::EndChild();

		
			ImGui::SameLine();

			ImGui::BeginChild("COL2", ImVec2(305, 313), true);
			{
				ImGui::Text("Misc Visuals");
				ImGui::Separator();
				ImGui::Combo(XorStr("Hands"), &g_Options.Visuals.Hands, handd, ARRAYSIZE(handd));
				ImGui::SliderFloat(XorStr("Hands Alpha"), &g_Options.Colors.hands_alpha, 0.f, 1.f);
				ImGui::SliderFloat(XorStr("FOV"), &g_Options.Visuals.FOVChanger, 0.f, 60.f, "%.0f");
				ImGui::SliderFloat(XorStr("V-FOV"), &g_Options.Visuals.viewmodelChanger, 60.f, 120.f, "%.0f",1.0f);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Viewmodel FOV.");
				ImGui::Checkbox(XorStr("NoFlash"), &g_Options.Visuals.NoFlash);
				ImGui::SameLine(150);
				ImGui::Checkbox(XorStr("NoSmoke"), &g_Options.Visuals.NoSmoke);
				ImGui::Checkbox(XorStr("Time"), &g_Options.Visuals.Time);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Display the current system time.");
				ImGui::SameLine(150);
				ImGui::Checkbox(XorStr("noScope Border"), &g_Options.Visuals.noscopeborder);
				ImGui::Separator();
				ImGui::Text("Checks");
				ImGui::Separator();
				ImGui::Checkbox(XorStr("IsScoped"), &g_Options.Visuals.IsScoped);

				if (ImGui::Button("Unhook"))
				{
					g_Engine->ClientCmd_Unrestricted("cl_mouseenable 1");
					unload = true;
				}
			}ImGui::EndChild();


		
	
}
void RenderSkinChanger()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	ImGui::BeginChild("skins", ImVec2(618, 313), true);
	{
	ImGui::Checkbox(("Enabled "), &g_Options.Skinchanger.Enabled);
	ImGui::SameLine(150);
	ImGui::PushItemWidth(150);
	if (ImGui::Button(("Force Update")))
	{
		KnifeApplyCallbk();
	}
	ImGui::Separator();
	ImGui::Columns(2, NULL, false);
	{
		ImGui::Combo(XorStr("Knife Model"), &g_Options.Skinchanger.Knife, knives, ARRAYSIZE(knives));
		ImGui::Combo(XorStr("Knife Skin"), &g_Options.Skinchanger.KnifeSkin, knifeskins, ARRAYSIZE(knifeskins));
		ImGui::Combo(XorStr("Gloves"), &g_Options.Skinchanger.gloves, gloves, ARRAYSIZE(gloves));
		ImGui::Combo(XorStr("UMP45"), &g_Options.Skinchanger.UMP45Skin, ump45, ARRAYSIZE(ump45));
		ImGui::Combo(XorStr("P90"), &g_Options.Skinchanger.P90Skin, p90, ARRAYSIZE(p90));
		ImGui::Combo(XorStr("AWP"), &g_Options.Skinchanger.AWPSkin, awp, ARRAYSIZE(awp));
		ImGui::Combo(XorStr("SSG08"), &g_Options.Skinchanger.SSG08Skin, ssg08, ARRAYSIZE(ssg08));
		ImGui::Combo(XorStr("SCAR20"), &g_Options.Skinchanger.SCAR20Skin, scar20, ARRAYSIZE(scar20));
		ImGui::Combo(XorStr("Glock-18"), &g_Options.Skinchanger.GlockSkin, glock, ARRAYSIZE(glock));
		ImGui::Combo(XorStr("USP-S"), &g_Options.Skinchanger.USPSkin, usp, ARRAYSIZE(usp));
		ImGui::Combo(XorStr("Deagle"), &g_Options.Skinchanger.DeagleSkin, deagle, ARRAYSIZE(deagle));

		ImGui::NextColumn();
		ImGui::Combo(XorStr("TEC-9"), &g_Options.Skinchanger.tec9Skin, tec9, ARRAYSIZE(tec9));
		ImGui::Combo(XorStr("P2000"), &g_Options.Skinchanger.P2000Skin, p2000, ARRAYSIZE(p2000));
		ImGui::Combo(XorStr("P250"), &g_Options.Skinchanger.P250Skin, p250, ARRAYSIZE(p250));
		ImGui::Combo(XorStr("AK-47"), &g_Options.Skinchanger.AK47Skin, ak47, ARRAYSIZE(ak47));
		ImGui::Combo(XorStr("M4A1-S"), &g_Options.Skinchanger.M4A1SSkin, m4a1s, ARRAYSIZE(m4a1s));
		ImGui::Combo(XorStr("M4A4"), &g_Options.Skinchanger.M4A4Skin, m4a4, ARRAYSIZE(m4a4));
		ImGui::Combo(XorStr("AUG"), &g_Options.Skinchanger.AUGSkin, aug, ARRAYSIZE(aug));
		ImGui::Combo(XorStr("FAMAS"), &g_Options.Skinchanger.FAMASSkin, famas, ARRAYSIZE(famas));

	}
	}ImGui::EndChild();
}
void RenderLegit()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	ImGui::Checkbox(XorStr("Enabled  "), &g_Options.Legitbot.MainSwitch);
	ImGui::SameLine(150);
	ImGui::Checkbox(XorStr("Enable Aimbot"), &g_Options.Legitbot.Aimbot.Enabled);
	ImGui::SameLine(300);
	ImGui::Checkbox(XorStr("Backtracking"), &g_Options.Legitbot.backtrack);
	ImGui::SameLine(450);
	ImGui::Checkbox(XorStr("Backtracking Dot"), &g_Options.Legitbot.memedot);
	ImGui::Separator();

	ImGui::BeginChild(" pistols", ImVec2(305, 154), true);
	{
		ImGui::Text("Pistols");
		ImGui::Separator();
		ImGui::Hotkey("Key##1", &g_Options.Legitbot.PistolKey, ImVec2(150, 20));
		ImGui::SliderFloat("Smooth##1", &g_Options.Legitbot.PistolSmooth, 1.00f, 100.00f, "%.2f");
		ImGui::SliderFloat("FOV##1", &g_Options.Legitbot.Pistolfov, 0.00f, 30.00f, "%.2f");

	}ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild(" rifles", ImVec2(305, 154), true);
	{
		ImGui::Text("Rifles");
		ImGui::Separator();
		ImGui::Hotkey("Key##0", &g_Options.Legitbot.MainKey, ImVec2(150, 20));
		ImGui::SliderFloat("Smooth##0", &g_Options.Legitbot.MainSmooth, 1.00f, 100.00f, "%.2f");
		ImGui::SliderFloat("FOV##0", &g_Options.Legitbot.Mainfov, 0.00f, 30.00f, "%.2f");
		ImGui::SliderFloat("Min RCS##0", &g_Options.Legitbot.main_recoil_min, 1.00f, 100.00f, "%.2f");
		ImGui::SliderFloat("Max RCS##0", &g_Options.Legitbot.main_recoil_max, 1.00f, 100.00f, "%.2f");
	}ImGui::EndChild();
	
	ImGui::BeginChild(" trigger", ImVec2(305, 129), true);
	{
		ImGui::Text("Triggerbot");
		ImGui::Separator();
		ImGui::Checkbox(XorStr("Enable Triggerbot"), &g_Options.Legitbot.Triggerbot.Enabled);
		ImGui::SliderFloat(XorStr("Delay"), &g_Options.Legitbot.Triggerbot.Delay, 1.f, 150.f, "%.0f");
		ImGui::Hotkey("Key##3", &g_Options.Legitbot.Triggerbot.Key, ImVec2(150, 20));
		if (ImGui::TreeNode("Filter"))
		{
			ImGui::Selectable(XorStr(" Head"), &g_Options.Legitbot.Triggerbot.Filter.Head);
			ImGui::Selectable(XorStr(" Arms"), &g_Options.Legitbot.Triggerbot.Filter.Arms);
			ImGui::Selectable(XorStr(" Chest"), &g_Options.Legitbot.Triggerbot.Filter.Chest);
			ImGui::Selectable(XorStr(" Stomach"), &g_Options.Legitbot.Triggerbot.Filter.Stomach);
			ImGui::Selectable(XorStr(" Legs"), &g_Options.Legitbot.Triggerbot.Filter.Legs);
		}ImGui::TreePop();
	}ImGui::EndChild();
}

void RenderMisc()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	ImGui::BeginChild("msc", ImVec2(618, 313), true);
	{
		ImGui::BeginChild("msc1", ImVec2(196, 299), true);
		{
			ImGui::Text("w.i.p");
			ImGui::Separator();
			ImGui::Checkbox(XorStr("NightMode"), &g_Options.Misc.nightMode);
			ImGui::Checkbox(XorStr("ChromeModels"), &g_Options.Misc.chrome);
			ImGui::Checkbox(XorStr("ThirdPerson"), &g_Options.Visuals.ThirdPerson);
			ImGui::Hotkey(XorStr("Key##77"), &g_Options.Misc.thirdpersonkey, ImVec2(150, 20));
			//ImGui::Text("ThirdPerson Angles");
			//ImGui::Combo(("  "),&g_Options.Misc.thirdpersontype,thirdpersontype, ARRAYSIZE(thirdpersontype));
			//ImGui::Checkbox(("angles"), &g_Options.Ragebot.fakeanglechams);
			ImGui::Checkbox(XorStr("Grenade Prediction"), &g_Options.Visuals.GrenadePrediction);
			ImGui::Checkbox(XorStr("Hitmarker"), &g_Options.Misc.hitmarker);
			ImGui::Checkbox(XorStr("Hitsound"), &g_Options.Misc.hitsound);
			ImGui::Checkbox(XorStr("Draw AWall"), &g_Options.Misc.AWalldmg);
			ImGui::Checkbox(XorStr("LBY Indicator"), &g_Options.Visuals.LBYIndicator);
			//ImGui::Checkbox(XorStr("Event Log"), &g_Options.Misc.EventLog);
		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("msc2", ImVec2(196, 299), true);
		{
			ImGui::Text("w.i.p");
			ImGui::Separator();
			ImGui::Checkbox(XorStr("Auto-Accept"), &g_Options.Misc.AutoAccept);
			ImGui::Checkbox(XorStr("Reveal Ranks"), &g_Options.Misc.ServerRankRevealAll);
			ImGui::Checkbox(XorStr("Bunny Hop"), &g_Options.Misc.Bhop);
			ImGui::Checkbox(XorStr("Auto-Strafe"), &g_Options.Misc.autostrafe);
			ImGui::Checkbox(XorStr("SpecList"), &g_Options.Misc.SpecList);
			ImGui::Checkbox(XorStr("Draw Inaccuracy"), &g_Options.Misc.inaccuracy);
			ImGui::Checkbox(XorStr("Sniper Crosshair"), &g_Options.Visuals.crosshair);
			ImGui::Checkbox(XorStr("Resolve Mode"), &g_Options.Misc.resolvemode);
			ImGui::Checkbox(XorStr("AA Lines"), &g_Options.Misc.aalines);
			ImGui::Checkbox(XorStr("No Visual Recoil"), &g_Options.Visuals.NoVisualRecoil);
			//ImGui::Checkbox(XorStr("Fake Angle Chams"), &g_Options.Ragebot.fakeanglechams);
		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("msc3", ImVec2(196, 300), true);
		{
			ImGui::Text("w.i.p");
			ImGui::Separator();
			static char buf2[128] = "";
			static char def[128] = "";
			ImGui::InputText(XorStr("##Clantag"), buf2, 16, ImGuiInputTextFlags_AllowTabInput);
			if (ImGui::Button(XorStr(" Change Clantag ")))
			{
				SetClanTag(buf2, buf2);
			}
			if (ImGui::Button(XorStr(" Clear Clantag ")))
				SetClanTag(def, def);

			

	

		}ImGui::EndChild();
	}ImGui::EndChild();
}





void RenderColors()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	ImGui::BeginChild("colors", ImVec2(618, 313), true);
	{
		ImGui::ColorEdit3(XorStr("Visible Chams Color"), g_Options.Colors.chams_color_vis);
		ImGui::ColorEdit3(XorStr("Not Visible Chams Color"), g_Options.Colors.chams_color_notvis);
		ImGui::ColorEdit3(XorStr("Hands Chams Color"), g_Options.Colors.hands_color);
		ImGui::ColorEdit3(XorStr("ESP Box CT Color"), (float*)&g_Options.Colors.box_color_ct);
		ImGui::ColorEdit3(XorStr("ESP Box T Color"), (float*)&g_Options.Colors.box_color_t);
		ImGui::ColorEdit3(XorStr("Grenade Prediction Line Color"), (float*)&g_Options.Colors.tracer_color);
		ImGui::ColorEdit3(XorStr("DLight Color"), g_Options.Colors.dlight_color);
		ImGui::ColorEdit3(XorStr("Sniper Crosshair Color"), g_Options.Colors.crosshair_color);
	}ImGui::EndChild();

}

void RenderRage()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	ImGui::Checkbox(XorStr("Active"), &g_Options.Ragebot.MainSwitch);
	ImGui::Separator();
	ImGui::BeginGroup();
	{
		ImGui::BeginChild("##General", ImVec2(200, 287), true);
		{
			//ImGui::Text("General");
			//ImGui::Separator();
			ImGui::Checkbox(XorStr("Enable Aimbot"), &g_Options.Ragebot.Enabled);
			ImGui::Checkbox(XorStr("Silent Aim"), &g_Options.Ragebot.Silent);
			ImGui::Checkbox(XorStr("Auto Fire"), &g_Options.Ragebot.AutoFire);
			ImGui::Checkbox(XorStr("Auto Pistol"), &g_Options.Ragebot.AutoPistol);
			ImGui::SliderFloat(XorStr("Aimbot FOV"), &g_Options.Ragebot.FOV, 1.f, 180.f, "%.0f");
			ImGui::Checkbox(XorStr("Backtracking"), &g_Options.Ragebot.PosAdj);
			ImGui::Checkbox(XorStr("Resolver"), &g_Options.Ragebot.Resolver);
			ImGui::Text(XorStr("Bruteforce after :"));
			ImGui::SliderFloat(XorStr("Shots: "), &g_Options.Ragebot.bruteAfterX, 0, 10, "%1.f");
			//ImGui::Checkbox(XorStr("FakeLag Fix"), &g_Options.Ragebot.FakeLagFix);
			//ImGui::Checkbox(XorStr("Enable FakeLag"), &g_Options.Ragebot.FakeLag);
			//ImGui::SliderInt(XorStr("Amount"), &g_Options.Ragebot.FakeLagAmt, 0, 15, "%1.f");
			ImGui::Checkbox(XorStr("Fake Walk"), &g_Options.Ragebot.fakewalk);
		}ImGui::EndChild();
	}ImGui::EndGroup();
	ImGui::SameLine();
	ImGui::BeginGroup();
	{
		ImGui::BeginChild("###AA", ImVec2(200, 269), true);
		{
			//ImGui::Text("Anti-Aim");
			//ImGui::Separator();
			ImGui::Checkbox(XorStr("Anti-Aim Enabled"), &g_Options.Ragebot.EnabledAntiAim);
			ImGui::Text(XorStr("Pitch :"));
			ImGui::Combo(XorStr("##Pitch"), &g_Options.Ragebot.Pitch, antiaimpitch, ARRAYSIZE(antiaimpitch));
			ImGui::Text(XorStr("Yaw :"));
			ImGui::Combo(XorStr("##Yaw"), &g_Options.Ragebot.YawTrue, antiaimyawtrue, ARRAYSIZE(antiaimyawtrue));
			ImGui::Text(XorStr("Fake Yaw :"));
			ImGui::Combo(XorStr("##Yaw-Fake"), &g_Options.Ragebot.YawFake, antiaimyawfake, ARRAYSIZE(antiaimyawfake));
			ImGui::Separator();
			ImGui::Text(XorStr("Anti-Aim Settings"));
			ImGui::Separator();
			ImGui::Checkbox(XorStr("At Players"), &g_Options.Ragebot.AtTarget);
			ImGui::Checkbox(XorStr("Edge"), &g_Options.Ragebot.Edge);

			

		}ImGui::EndChild();
	}ImGui::EndGroup();
	ImGui::SameLine();
	ImGui::BeginGroup();
	{
		ImGui::BeginChild("##Accuracy", ImVec2(200, 287), true);
		{
			//ImGui::Text("Accuracy");
			//ImGui::Separator();
			ImGui::Checkbox(XorStr("NoRecoil"), &g_Options.Ragebot.AntiRecoil);
			ImGui::Checkbox(XorStr("Auto Stop"), &g_Options.Ragebot.AutoStop);
			ImGui::Checkbox(XorStr("Auto Crouch"), &g_Options.Ragebot.AutoCrouch);
			ImGui::Checkbox(XorStr("Auto Scope"), &g_Options.Ragebot.AutoScope);
			//ImGui::Checkbox(XorStr("Friendly Fire"), &g_Options.Ragebot.FriendlyFire);
			ImGui::Checkbox(XorStr("Knife AA"), &g_Options.Ragebot.KnifeAA);
			ImGui::Combo(XorStr("Hitbox"), &g_Options.Ragebot.Hitbox, aimBones, ARRAYSIZE(aimBones));
			ImGui::Combo(XorStr("Hitscan"), &g_Options.Ragebot.Hitscan, hitscan, ARRAYSIZE(hitscan));
			ImGui::SliderFloat(XorStr("Minimum Damage"), &g_Options.Ragebot.MinimumDamage, 1.f, 100.f, "%.2f");
			ImGui::Checkbox(XorStr("Enable Hitchance"), &g_Options.Ragebot.Hitchance);
			ImGui::SliderFloat(XorStr("Hitchance Amount"), &g_Options.Ragebot.HitchanceAmount, 0.f, 100.f, "%.1f");
			ImGui::Hotkey(XorStr("Baim Key"), &g_Options.Ragebot.BAIMkey, ImVec2(150, 20));

		}ImGui::EndChild();
	}ImGui::EndGroup();
}