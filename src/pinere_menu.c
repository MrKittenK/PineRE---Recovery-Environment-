#include "pinere_menu.h"
#include <stdio.h>
#include <string.h>

static pinere_state_t g_state;

void render_pinere_menu(pinere_menu_t *menu) {
    if (!menu) return;
    lv_obj_clean(lv_scr_act());
    lv_obj_t *header = lv_label_create(lv_scr_act());
    lv_label_set_text(header, lookup_asset_text(menu->title));
    lv_obj_align(header, LV_ALIGN_TOP_MID, 0, 20);
    for (int i = 0; i < menu->item_count; i++) {
        lv_obj_t *btn = lv_label_create(lv_scr_act());
        lv_label_set_text(btn, lookup_asset_text(menu->items[i].label_key));
        lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 50, 100 + (i * 40));
        if (i == g_state.selected_index) {
            lv_obj_set_style_bg_color(btn, PINERE_COLOR_GREEN, 0);
            lv_obj_set_style_text_color(btn, PINERE_COLOR_BG, 0);
        } else {
            lv_obj_set_style_text_color(btn, PINERE_COLOR_TEXT, 0);
        }
    }
    lv_obj_t *footer = lv_label_create(lv_scr_act());
    lv_label_set_text(footer, lookup_asset_text("global.footer"));
    lv_obj_align(footer, LV_ALIGN_BOTTOM_MID, 0, -20);
}

void pinere_handle_input_press(uint32_t hardware_key, bool is_pressed) {
    if (!is_pressed || !g_state.current_active_menu) return;
    switch (hardware_key) {
        case PINERE_KEY_UP: if (g_state.selected_index > 0) g_state.selected_index--; break;
        case PINERE_KEY_DOWN: if (g_state.selected_index < g_state.current_active_menu->item_count - 1) g_state.selected_index++; break;
        case PINERE_KEY_A:
            {
                pinere_menu_item_t *selected = &g_state.current_active_menu->items[g_state.selected_index];
                if (selected->target_sub) { g_state.current_active_menu = selected->target_sub; g_state.selected_index = 0; }
                else if (selected->action_callback) { selected->action_callback(); }
                else if (selected->script_path) { /* TODO: Dispatch Work Layer action via the Dispatcher ↔ Work Layer contract. */ }
            }
            break;
        default: break;
    }
    render_pinere_menu(g_state.current_active_menu);
}