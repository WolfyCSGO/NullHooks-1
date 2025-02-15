#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/features/visuals/skin_changer/skin_changer.hpp"

bool skins::apply_skin(DWORD weapon_handle) {
	// Get the weapon entity from the provided handle
	weapon_t* weapon = (weapon_t*)interfaces::entity_list->get_client_entity_handle(weapon_handle);
	if (!weapon) return false;

	int weapon_index = weapon->item_definition_index();										// Get the weapons item definition index
	if (skins::custom_skins.find(weapon_index) == skins::custom_skins.end()) return false;	// Check if the weapon is in map
	
	// Change knife index
	if (skins::custom_skins.at(weapon_index).item_definition_index != NULL) {
		weapon->item_definition_index() = skins::custom_skins.at(weapon_index).item_definition_index;
		weapon_index = skins::custom_skins.at(weapon_index).item_definition_index;
	}
	
	update_knife_model(weapon);     // Will update the weapon model index and the viewmodel if needed

	// Apply to fallback variables
	if (skins::custom_skins.at(weapon_index).paint_kit != NULL) weapon->fallback_paint_kit() = skins::custom_skins.at(weapon_index).paint_kit;
	if (skins::custom_skins.at(weapon_index).quality != NULL)   weapon->entity_quality()     = skins::custom_skins.at(weapon_index).quality;
	if (skins::custom_skins.at(weapon_index).seed != NULL)      weapon->fallback_seed()      = skins::custom_skins.at(weapon_index).seed;
	if (skins::custom_skins.at(weapon_index).stattrack != NULL) weapon->fallback_stattrack() = skins::custom_skins.at(weapon_index).stattrack;
	if (skins::custom_skins.at(weapon_index).wear != NULL)      weapon->fallback_wear()      = skins::custom_skins.at(weapon_index).wear;
	// TODO: Set account id to localplayer id for stattrack
	// TODO: Custom name 

	weapon->item_id_high() = -1;	// Edit "m_iItemIDHigh" so fallback values will be used

	return true;
}

void skins::update_knife_model(weapon_t* weapon) {
	const int weapon_idx = weapon->item_definition_index();
	if (!(skins::custom_models.find(weapon_idx) != skins::custom_models.end())) return;
	
	// Weapon models
	weapon->set_model_index(interfaces::model_info->get_model_index(skins::custom_models.at(weapon_idx).viewmodel));
	weapon->net_pre_data_update(0);

	// Viewmodel
	const auto viewmodel = (base_view_model_t*)interfaces::entity_list->get_client_entity_handle(csgo::local_player->view_model());
	if (!viewmodel) return;
	const auto viewmodel_weapon = (weapon_t*)interfaces::entity_list->get_client_entity_handle(viewmodel->weapon());
	if (viewmodel_weapon != weapon) return;
	viewmodel->set_model_index(interfaces::model_info->get_model_index(skins::custom_models.at(weapon_idx).viewmodel));

    // Worldmodel
    const auto worldmodel = (weapon_t*)interfaces::entity_list->get_client_entity_handle(viewmodel_weapon->weapon_worldmodel());
    if (!worldmodel) return;
    worldmodel->set_model_index(interfaces::model_info->get_model_index(skins::custom_models.at(weapon_idx).worldmodel));
}

// Used in FRAME_NET_UPDATE_POSTDATAUPDATE_START inside FrameStageNotify
void skins::change_skins(client_frame_stage_t stage) {
	if (!csgo::local_player) return;

	// Change every skin in the map
	auto weapons = csgo::local_player->get_weapons();
	if (!weapons) return;
	for (int n = 0; weapons[n]; n++) {
		apply_skin(weapons[n]);
	}
}

#pragma region ANIMATIONS
int remap_knife_animation(int weaponID, const int sequence) noexcept {
    enum Sequence {
        SEQUENCE_DEFAULT_DRAW = 0,
        SEQUENCE_DEFAULT_IDLE1 = 1,
        SEQUENCE_DEFAULT_IDLE2 = 2,
        SEQUENCE_DEFAULT_LIGHT_MISS1 = 3,
        SEQUENCE_DEFAULT_LIGHT_MISS2 = 4,
        SEQUENCE_DEFAULT_HEAVY_MISS1 = 9,
        SEQUENCE_DEFAULT_HEAVY_HIT1 = 10,
        SEQUENCE_DEFAULT_HEAVY_BACKSTAB = 11,
        SEQUENCE_DEFAULT_LOOKAT01 = 12,

        SEQUENCE_BUTTERFLY_DRAW = 0,
        SEQUENCE_BUTTERFLY_DRAW2 = 1,
        SEQUENCE_BUTTERFLY_LOOKAT01 = 13,
        SEQUENCE_BUTTERFLY_LOOKAT03 = 15,

        SEQUENCE_FALCHION_IDLE1 = 1,
        SEQUENCE_FALCHION_HEAVY_MISS1 = 8,
        SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP = 9,
        SEQUENCE_FALCHION_LOOKAT01 = 12,
        SEQUENCE_FALCHION_LOOKAT02 = 13,

        SEQUENCE_DAGGERS_IDLE1 = 1,
        SEQUENCE_DAGGERS_LIGHT_MISS1 = 2,
        SEQUENCE_DAGGERS_LIGHT_MISS5 = 6,
        SEQUENCE_DAGGERS_HEAVY_MISS2 = 11,
        SEQUENCE_DAGGERS_HEAVY_MISS1 = 12,

        SEQUENCE_BOWIE_IDLE1 = 1,
    };

    auto random = [](int min, int max) {
            return std::rand() % (max + 1 - min) + min;
    };

    switch (weaponID) {
        case WEAPON_KNIFE_BUTTERFLY:
            switch (sequence) {
                case SEQUENCE_DEFAULT_DRAW:
                    return random(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
                case SEQUENCE_DEFAULT_LOOKAT01:
                    return random(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
                default:
                    return sequence + 1;
            }
        case WEAPON_KNIFE_FALCHION:
            switch (sequence) {
                case SEQUENCE_DEFAULT_DRAW:
                case SEQUENCE_DEFAULT_IDLE1:
                case SEQUENCE_DEFAULT_HEAVY_HIT1:
                case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
                    return sequence;
                case SEQUENCE_DEFAULT_HEAVY_MISS1:
                    return random(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
                case SEQUENCE_DEFAULT_LOOKAT01:
                    return random(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
                default:
                    return sequence - 1;
            }
        case WEAPON_KNIFE_PUSH:
            switch (sequence) {
                case SEQUENCE_DEFAULT_IDLE2:
                    return SEQUENCE_DAGGERS_IDLE1;
                case SEQUENCE_DEFAULT_LIGHT_MISS1:
                case SEQUENCE_DEFAULT_LIGHT_MISS2:
                    return random(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
                case SEQUENCE_DEFAULT_HEAVY_MISS1:
                    return random(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
                case SEQUENCE_DEFAULT_HEAVY_HIT1:
                case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
                case SEQUENCE_DEFAULT_LOOKAT01:
                    return sequence + 3;
                case SEQUENCE_DEFAULT_DRAW:
                case SEQUENCE_DEFAULT_IDLE1:
                    return sequence;
                default:
                    return sequence + 2;
            }
        case WEAPON_KNIFE_SURVIVAL_BOWIE:
            switch (sequence) {
                case SEQUENCE_DEFAULT_DRAW:
                case SEQUENCE_DEFAULT_IDLE1:
                    return sequence;
                case SEQUENCE_DEFAULT_IDLE2:
                    return SEQUENCE_BOWIE_IDLE1;
                default:
                    return sequence - 1;
            }
        case WEAPON_KNIFE_URSUS:
        case WEAPON_KNIFE_SKELETON:
        case WEAPON_KNIFE_NOMAD:
        case WEAPON_KNIFE_PARACORD:
        case WEAPON_KNIFE_SURVIVAL:
            switch (sequence) {
                case SEQUENCE_DEFAULT_DRAW:
                    return random(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
                case SEQUENCE_DEFAULT_LOOKAT01:
                    return random(SEQUENCE_BUTTERFLY_LOOKAT01, Sequence(14));
                default:
                    return sequence + 1;
            }
        case WEAPON_KNIFE_STILETTO:
            switch (sequence) {
                case SEQUENCE_DEFAULT_LOOKAT01:
                    return random(12, 13);
            }
        case WEAPON_KNIFE_WIDOWMAKER:
            switch (sequence) {
                case SEQUENCE_DEFAULT_LOOKAT01:
                    return random(14, 15);
            }
        default:
            return sequence;
    }
}

// Used in viewmodel_sequence hook
void skins::fix_knife_animation(weapon_t* viewmodel_weapon, long& sequence) {
    if (!csgo::local_player) return;
    if (!viewmodel_weapon->is_knife()) return;

    sequence = remap_knife_animation(viewmodel_weapon->item_definition_index(), sequence);
}
#pragma endregion