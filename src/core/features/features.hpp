#pragma once
#include "source-sdk/sdk.hpp"
#include "core/features/misc/backtrack.hpp"
#include "misc/backtrack.hpp"					// VIP so uses his own header file

namespace aim {
	void triggerbot(c_usercmd* cmd);
	void run_aimbot(c_usercmd* cmd);
	bool aimbot_weapon_check();
	void draw_fov();
}

namespace visuals {
	void playeresp();
	void grenade_projectile_esp();
	void noflash();
	void motion_blur(view_setup_t* setup) noexcept;
	void bullet_tracer(i_game_event *event);

	namespace glow {
		void draw_glow();
	}

	namespace chams {
		void draw_chams(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* matrix);
	}

	namespace custom_models {
		void replace_model(char* path);
	}

	namespace misc {
		void nade_predict() noexcept;
		void nosmoke(client_frame_stage_t frame_stage);
	}

	namespace entity_info {
		void bomb(entity_t* bomb_ent);
		void dropped_bomb(entity_t* bomb_ent);
		void weapon_name(entity_t* entity, const char* text, int y_offset);
	}

	namespace crosshair {
		void draw_custom_crosshair(int x, int y, int outline, color cross_color);
		void custom_crosshair();
		void recoil_crosshair();
	}
}

namespace misc {
	void spectator_list();
	void thirdperson();
	void reset_thirdperson();

	namespace movement {
		void infinite_duck(c_usercmd* cmd);
		
		void bunny_hop(c_usercmd* cmd);
		void edgejump(c_usercmd* cmd, int old_flags);
		void edgebug(c_usercmd* cmd, int old_flags);
		
		void pre_pred_jumpbug(c_usercmd* cmd, int old_flags);
		void post_pred_jumpbug(c_usercmd* cmd, int old_flags);

	}
	namespace speedgraph {
		void update();
		void draw();
	}
}

namespace prediction {
	void start(c_usercmd *cmd);
	void end();

	inline player_move_data data;
	inline float old_cur_time;
	inline float old_frame_time;
	inline int *prediction_random_seed;
};

namespace commands_features {
	void exec_autoexec();
}
