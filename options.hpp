#pragma once

#include <string>
#include "valve_sdk/Misc/Color.hpp"

#define OPTION(type, var, val) type var = val

enum MenuKnife
{
	Off,
	Bayonet,
	M9,
	Karam,
	Gut,
	Flip,
	Butter,
	Shadow,
	Flachion,
	Hunts,
	Bowei

};

class Config
{
public:


	int CurrentPlayer;
	char* PlayeName;



	OPTION(bool, misc_slidewalk, false );
	OPTION(bool, misc_crouchexploit, false);

    // 
    // ESP
    // 
    OPTION(bool, esp_enabled, false);
	OPTION(bool, esp_visible_only, false);
    OPTION(bool, esp_enemies_only, true);
    OPTION(int, esp_player_boxes, false);
	OPTION(bool, esp_player_boxes_outline, true);
    OPTION(int, esp_player_names, 0);
    OPTION(int, esp_player_health, false);
    OPTION(int, esp_player_weapons, 0);
    OPTION(bool, esp_player_snaplines, false);
    OPTION(bool, esp_dropped_weapons, false);
    OPTION(bool, esp_defuse_kit, false);
    OPTION(bool, esp_planted_c4, false);
	OPTION(bool, esp_nade_esp, false);
	OPTION(bool, esp_skeleton, false);



    //
    // CHAMS
    //
    OPTION(int, chams_player_enabled, 0);
    OPTION(bool, chams_player_ignorez, false);

    //
    // MISC
    //
    OPTION(bool, misc_bhop, false);
    OPTION(bool, misc_no_hands, false);
	OPTION(int, misc_clantag, 0);
	char tag[42];
	OPTION(bool, misc_noname, false);
	OPTION(bool, misc_radar, false);
	OPTION(bool, misc_speclist, false);
	OPTION(bool, misc_namesteal, false);
	OPTION(int, misc_chatspam, false);
	char spam[128];
	OPTION(float, misc_viewmodel_fov, 0);
	OPTION(bool, misc_rankreveal, false);
	OPTION(bool, misc_nightmode, false);
	OPTION(bool, misc_col_sky, false);
	OPTION(float, misc_fov, 0);
	OPTION(int, misc_arms_chams, 0);
	OPTION(int, misc_weapon_chams, 0);
	OPTION(Color, misc_arms_color, Color(22, 81, 211, 255));
	OPTION(Color, misc_weapon_color, Color(227, 38, 38, 255));
	OPTION(int, legit_backtrack, false);

	OPTION(Color, hand_color2, Color(211, 81, 22, 255));
	OPTION(Color, weap_color2, Color(38, 38, 227, 255));

	OPTION(bool, skins_enable, false);
	OPTION(int, skins_knife_model, 0);
	OPTION(int, skins_glove_model, 0);
	OPTION(int, skins_glove_skin, 0);

	int currentWeapon;
	struct
	{
		bool ChangerEnabled = false;
		int ChangerSkin = 0;
		char ChangerName[32] = "";
		bool stat;
		int statamount;
		bool customstat;
		int customstatamount;
		char Name;
	} W[519];

    // 
    // COLORS
    // 
    OPTION(Color, color_esp_ally_visible, Color(0, 128, 255));
    OPTION(Color, color_esp_enemy_visible, Color(111, 227, 40, 255));
    OPTION(Color, color_esp_ally_occluded, Color(0, 128, 255));
    OPTION(Color, color_esp_enemy_occluded, Color(180, 7, 233, 255));
    OPTION(Color, color_esp_weapons, Color(250, 250, 250));
    OPTION(Color, color_esp_defuse, Color(0, 128, 255));
    OPTION(Color, color_esp_c4, Color(255, 255, 0));



    OPTION(Color, color_chams_player_ally_visible, Color(0, 128, 255));
    OPTION(Color, color_chams_player_ally_occluded, Color(0, 255, 128));
    OPTION(Color, color_chams_player_enemy_visible, Color(2, 255, 243, 255));
    OPTION(Color, color_chams_player_enemy_occluded, Color(159, 212, 13, 255));
	OPTION(Color, color_sky, Color(170, 30, 130));
	OPTION(Color, color_nightmode, Color(50, 50, 50));

	bool update_world;

};

extern Config g_Options;
extern bool   g_Unload;