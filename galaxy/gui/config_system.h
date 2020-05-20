#pragma once
#include <filesystem>

class c_config final
{
public:
	void run( const char* name ) noexcept;
	void load( size_t ) noexcept;
	void save( size_t ) const noexcept;
	void add( const char* ) noexcept;
	void remove( size_t ) noexcept;
	void rename( size_t, const char* ) noexcept;
	void reset( ) noexcept;

	constexpr auto& get_configs( ) noexcept {
		return configs;
	}

	struct
	{
		int hitscan_type;

		bool Aimbot;
		int Hitchance;
		int HitchanceValue;
		int Mindmg;
		bool Resolver;
		int BodyScale;
		int HeadScale;
		bool MultiPoint;
		bool DelayShot;
		bool IgnoreLimbs;
		bool Autostop;
		bool FixShotPitch;
		bool PosBacktrack;
		bool ShotBacktrack;
		bool BaimLethal;
		bool BaimPitch;
		bool BaimInAir;

		bool fake_lag;
		int fakelag_type;
		int body_key;
		bool server_hitobx;

		//slowwalk
		int slowspeed;
		bool slowwalk;

		//antiaim`
		int yaw_desync;
		int desync_swap;
		int body_lean;
		int pos_x;
		int pos_y;
		bool specator_list;

		bool flags;
		bool top_bar_enable;
		bool local_trails; 

		int body_invert;

		int air_key;
		bool air_stuck;
		bool no_smoke;
		bool air_desync;

		// - hitbox - //
		bool head;
		bool body;
		bool neck;
	
		bool desync_chams;
		float desync_chams_overide[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float desync_chams_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		int desync_chams_material;
		int desync_chams_overide_material;
		bool deysnc_Overid;

		bool legs;
		bool arms;

		bool Antiaim;
		bool DesyncAngle;
		bool RandJitterInRange;
		int	JitterRange;
		int	Fakelag;
		bool FakeLagOnPeek;
		bool ChokeShotOnPeek;

		bool Esp;
		int Font;
		
		bool name;
		int HealthVal;
		bool weapon_name;
		bool ammo_bar;
		bool Box;
		
		bool HealthBar;
		bool Skeleton[2] = { false,false };
		
		bool Chams;
		
		bool NoZoom;
		bool NoScope;
		bool NoRecoil;
		int Fov;
		bool Crosshair;

		bool Bhop;
		bool AutoStrafe;
		bool LegitBacktrack;

		// - [ esp ] -
		int NightModeSlider;
		bool NightMode;
		float sky_box_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };


		// - [ chams ] -

		// - enemy -
		bool chams_enemy;
		bool enemy_invisible;
		bool enemy_overide_chams;
		int overide_enemy_material;
		int normal_enemy_material;
		float enemy_visible_colors[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float enemy_overide_colors[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

		// - local - 
		bool local_chams;
		bool local_chams_overide;
		int local_materials;
		int local_overide_materials;
		float local_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float local_overide_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

		// - hands - 
		bool hand_chams;
		bool hand_chams_overide;
		int hand_materials;
		int hand_materials_overide;
		float hand_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float hand_overide_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };


		// - [ colors ] -

		float box_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float name_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float ammo_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float menu_color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };

	} 
	cfg;



private:
	std::filesystem::path path;
	std::vector<std::string> configs;
};
extern c_config galaxy_vars;