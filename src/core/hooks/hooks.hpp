#pragma once
#include <cstdint>
#include "../features/features.hpp"
#include "../../source-sdk/sdk.hpp"
#include "../helpers/misc_helpers.hpp"
#include "../menu/framework.hpp"

namespace hooks {
	bool initialize();
	void release();

	inline unsigned int get_virtual(void* _class, unsigned int index) { return static_cast<unsigned int>((*static_cast<int**>(_class))[index]); }

	namespace WndProc_hook {
		inline HWND csgo_window;
		inline WNDPROC original;
		long __stdcall WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	}

	namespace alloc_key_values_memory {
		using fn = void* (__thiscall*)(void*, const std::int32_t);
		inline fn original;
		void* __stdcall hook(const std::int32_t size);
	}

	namespace create_move {
		using fn = bool(__stdcall*)(float, c_usercmd*);
		inline fn original;
		bool __stdcall hook(float input_sample_frametime, c_usercmd* cmd);
	}

	namespace paint_traverse {
		using fn = void(__thiscall*)(i_panel*, unsigned int, bool, bool);
		inline fn original;
		void __stdcall hook(unsigned int panel, bool force_repaint, bool allow_force);
	}

	namespace findmdl {
		using fn = unsigned long(__thiscall*)(void*, const char*);
		inline fn original;
		unsigned long __stdcall hook(const char* path) noexcept;
	}
	
	namespace do_post_screen_space_effects {
		using fn = void(__thiscall*)(void*, const void*);
		inline fn original = nullptr;
		void __stdcall hook(const void* viewSetup);
	}

	namespace get_viewmodel_fov {
		using fn = float(__thiscall*)(void*);
		inline fn original;
		float __fastcall hook(uintptr_t, uintptr_t);
	}

	namespace override_view {
		using fn = void(__thiscall*)(void*, view_setup_t* setup);
		inline fn original;
		void __fastcall hook(uintptr_t, uintptr_t, view_setup_t* setup);
	}

	namespace draw_model_execute {
		using fn = void(__fastcall*)(void*, int, i_mat_render_context*, const draw_model_state_t&, const model_render_info_t&, matrix_t*);
		inline fn original = nullptr;
		void __fastcall hook(void* _this, int edx, i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& pInfo, matrix_t* pCustomBoneToWorld);
	}
}
