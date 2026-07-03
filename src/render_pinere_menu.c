#include "pinere_menu.h"
#include "lvgl/lvgl.h"

/*
 * render_pinere_menu()
 * Deterministic rendering: Maps the current state to the LVGL object tree.
 */
void render_pinere_menu(pinere_menu_t *menu) {
    if (!menu) return;

    /* 1. Clear active container (assuming a persistent screen container) */
    lv_obj_clean(lv_scr_act());

    /* 2. Render Header (Title) */
    lv_obj_t *title = lv_label_create(lv_scr_act());
    lv_label_set_text(title, lookup_asset_text(menu->title));
    lv_obj_set_style_text_color(title, PINERE_COLOR_TEXT, 0);

    /* 3. Render Menu Items */
    for (int i = 0; i < menu->item_count; i++) {
        lv_obj_t *btn = lv_label_create(lv_scr_act());

        /* Resolve label from registry */
        lv_label_set_text(btn, lookup_asset_text(menu->items[i].label_key));

        /* Highlight selected item */
        if (i == g_state.selected_index) {
            lv_obj_set_style_bg_color(btn, PINERE_COLOR_GREEN, 0); // Active highlight
            lv_obj_set_style_text_color(btn, PINERE_COLOR_BG, 0);
        } else {
            lv_obj_set_style_text_color(btn, PINERE_COLOR_TEXT, 0);
        }
    }

    /* 4. Render Footer (Help/Global Context) */
    lv_obj_t *footer = lv_label_create(lv_scr_act());
    lv_label_set_text(footer, lookup_asset_text("global.footer"));
    lv_obj_align(footer, LV_ALIGN_BOTTOM_MID, 0, -10);
}
