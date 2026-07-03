#include "pinere_menu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static float scaling_modifier = 1.0f;
static char text_scratchpad[TEXT_BUFFER_SIZE];
static pinere_state_t engine_state = {0};

/* UI Object Handles */
static lv_obj_t *main_container;
static lv_obj_t *header_label;
static lv_obj_t *menu_list_container;
static lv_obj_t *footer_label;
static lv_obj_t *help_modal_obj = NULL;
static lv_obj_t *help_text_label = NULL;

void pinere_calculate_scale(int current_width) {
    scaling_modifier = (float)current_width / DESIGN_BASE_WIDTH;
}

int scale_px(int px) {
    int scaled = (int)((float)px * scaling_modifier);
    return (scaled < 1 && px > 0) ? 1 : scaled;
}

const char* lookup_asset_text(const char *key_tag) {
    FILE *file = fopen(ASSET_FILE_PATH, "r");
    if (!file) {
        return "Error: Asset file missing.";
    }

    char line[256];
    char target_tag[128];
    snprintf(target_tag, sizeof(target_tag), "[%s]", key_tag);

    text_scratchpad[0] = '\0';
    int capturing = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0;
        if (line[0] == '#') continue;

        if (capturing) {
            if (line[0] == '[') break;
            if (strlen(text_scratchpad) > 0) {
                strncat(text_scratchpad, "\n", TEXT_BUFFER_SIZE - strlen(text_scratchpad) - 1);
            }
            strncat(text_scratchpad, line, TEXT_BUFFER_SIZE - strlen(text_scratchpad) - 1);
        } else {
            if (strcmp(line, target_tag) == 0) {
                capturing = 1;
            }
        }
    }
    fclose(file);

    if (strlen(text_scratchpad) == 0) {
        snprintf(text_scratchpad, TEXT_BUFFER_SIZE, "Missing asset: %s", key_tag);
    }
    return text_scratchpad;
}

void pinere_ui_init(void) {
    lv_obj_t *scr = lv_scr_act();
    lv_obj_set_style_bg_color(scr, PINERE_COLOR_BG, 0);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);

    /* Main Flex Flow Box Layout */
    main_container = lv_obj_create(scr);
    lv_obj_set_size(main_container, lv_pct(100), lv_pct(100));
    lv_obj_set_flex_flow(main_container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(main_container, scale_px(20), 0);
    lv_obj_set_style_bg_opa(main_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(main_container, 0, 0);

    /* Title Row Layout */
    header_label = lv_label_create(main_container);
    lv_obj_set_style_text_color(header_label, PINERE_COLOR_TEXT, 0);
    
    /* Divider 1 */
    lv_obj_t *sep1 = lv_obj_create(main_container);
    lv_obj_set_size(sep1, lv_pct(100), scale_px(2));
    lv_obj_set_style_bg_color(sep1, PINERE_COLOR_DIM, 0);
    lv_obj_set_style_border_width(sep1, 0, 0);
    lv_obj_set_style_margin_top(sep1, scale_px(10), 0);
    lv_obj_set_style_margin_bottom(sep1, scale_px(10), 0);

    /* Center Menu Body Content Area */
    menu_list_container = lv_obj_create(main_container);
    lv_obj_set_width(menu_list_container, lv_pct(100));
    lv_obj_set_flex_grow(menu_list_container, 1);
    lv_obj_set_flex_flow(menu_list_container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_bg_opa(menu_list_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(menu_list_container, 0, 0);
    lv_obj_set_style_pad_all(menu_list_container, 0, 0);

    /* Divider 2 */
    lv_obj_t *sep2 = lv_obj_create(main_container);
    lv_obj_set_size(sep2, lv_pct(100), scale_px(2));
    lv_obj_set_style_bg_color(sep2, PINERE_COLOR_DIM, 0);
    lv_obj_set_style_border_width(sep2, 0, 0);
    lv_obj_set_style_margin_top(sep2, scale_px(10), 0);
    lv_obj_set_style_margin_bottom(sep2, scale_px(10), 0);

    /* Bottom Interactive Dynamic Footer Row */
    footer_label = lv_label_create(main_container);
    lv_obj_set_style_text_color(footer_label, PINERE_COLOR_DIM, 0);
    lv_obj_set_width(footer_label, lv_pct(100));

    /* Initialize Help Popup Template structures */
    help_modal_obj = lv_obj_create(scr);
    lv_obj_set_size(help_modal_obj, lv_pct(80), lv_pct(50));
    lv_obj_center(help_modal_obj);
    lv_obj_set_style_bg_color(help_modal_obj, PINERE_COLOR_BG, 0);
    lv_obj_set_style_border_color(help_modal_obj, PINERE_COLOR_DIM, 0);
    lv_obj_set_style_border_width(help_modal_obj, scale_px(2), 0);
    lv_obj_set_style_pad_all(help_modal_obj, scale_px(15), 0);
    lv_obj_add_flag(help_modal_obj, LV_OBJ_FLAG_HIDDEN);

    help_text_label = lv_label_create(help_modal_obj);
    lv_obj_set_width(help_text_label, lv_pct(100));
    lv_obj_set_style_text_color(help_text_label, PINERE_COLOR_TEXT, 0);
}

void render_pinere_menu(pinere_menu_t *menu) {
    if (!menu) return;
    engine_state.current_active_menu = menu;
    
    lv_obj_clean(menu_list_container);
    lv_label_set_text(header_label, menu->title);

    for (int i = 0; i < menu->item_count; i++) {
        lv_obj_t *row = lv_obj_create(menu_list_container);
        lv_obj_set_size(row, lv_pct(100), LV_SIZE_CONTENT);
        lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
        lv_obj_set_style_bg_opa(row, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(row, 0, 0);
        lv_obj_set_style_pad_all(row, scale_px(4), 0);

        lv_obj_t *ptr = lv_label_create(row);
        if (i == engine_state.selected_index) {
            lv_label_set_text(ptr, "► ");
            lv_obj_set_style_text_color(ptr, PINERE_COLOR_GREEN, 0);
        } else {
            lv_label_set_text(ptr, "  ");
        }

        char full_key[128];
        snprintf(full_key, sizeof(full_key), "%s.label", menu->items[i].asset_key_prefix);
        const char *label_text = lookup_asset_text(full_key);

        lv_obj_t *lbl = lv_label_create(row);
        lv_label_set_text(lbl, label_text);

        if (i == engine_state.selected_index) {
            lv_obj_set_style_text_color(lbl, PINERE_COLOR_TEXT, 0);
            
            /* Update footer dynamically using the highlighted row metadata */
            snprintf(full_key, sizeof(full_key), "%s.footer", menu->items[i].asset_key_prefix);
            lv_label_set_text(footer_label, lookup_asset_text(full_key));
        } else {
            lv_obj_set_style_text_color(lbl, PINERE_COLOR_DIM, 0);
        }
    }
}
