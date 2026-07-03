#ifndef PINERE_MENU_H
#define PINERE_MENU_H

#include "lvgl/lvgl.h"
#include <stdbool.h>

/* ==============================================================================
 * PineRE Maintenance Mode — Core Framework Configurations
 * ============================================================================== */

#define DESIGN_BASE_WIDTH   720.0f
#define MAX_ITEMS_PER_MENU  10
#define TEXT_BUFFER_SIZE    512
#define ASSET_FILE_PATH     "/usr/share/pinere/menu_assets.txt"

/* Target Hardware Input Mappings via Linux evdev Event Subsystem */
#define PINERE_KEY_UP       103  /* KEY_UP */
#define PINERE_KEY_DOWN     108  /* KEY_DOWN */
#define PINERE_KEY_A        304  /* KEY_A / BTN_SOUTH */
#define PINERE_KEY_B        305  /* KEY_B / BTN_EAST */
#define PINERE_KEY_L1       310  /* KEY_TL */
#define PINERE_KEY_START    315  /* KEY_START */
#define PINERE_KEY_SELECT   314  /* KEY_SELECT */

/* Core Theme UI Colors */
#define PINERE_COLOR_BG     lv_color_black()
#define PINERE_COLOR_TEXT   lv_color_white()
#define PINERE_COLOR_DIM    lv_color_make(0x7F, 0x7F, 0x7F) /* Terminal Grey */
#define PINERE_COLOR_GREEN  lv_color_make(0x00, 0xCC, 0x00) /* Pine Green Accent */

/* Forward declaration of the menu layout structural matrix */
struct pinere_menu_s;

typedef struct {
    const char *asset_key_prefix;       /* Token lookup string tag (e.g. "boot_es") */
    void (*action_callback)(void);       /* Script trigger function hook */
    struct pinere_menu_s *target_sub;    /* Directory pointer link to sub-menu layer */
} pinere_menu_item_t;

typedef struct pinere_menu_s {
    const char *title;                             /* Screen Header String Block */
    pinere_menu_item_t items[MAX_ITEMS_PER_MENU];  /* Selectable target options array */
    int item_count;                                /* Number of population records loaded */
    struct pinere_menu_s *parent_menu;             /* Navigation step back reference tracking */
} pinere_menu_t;

/* Global Engine State Management Machine Tracking variables */
typedef struct {
    pinere_menu_t *current_active_menu;
    int selected_index;
    bool help_overlay_active;
    bool tester_mode_active;
    bool script_running_active;
} pinere_state_t;

/* Global API Interface Driver Layout wrappers */
void pinere_ui_init(void);
void pinere_calculate_scale(int current_width);
int scale_px(int px);
const char* lookup_asset_text(const char *key_tag);
void render_pinere_menu(pinere_menu_t *menu);
void pinere_handle_input_press(uint32_t hardware_key, bool is_pressed);
void init_menu_tree_matrix(void);

/* Shared references to the top-level matrix maps */
extern pinere_menu_t menu_root;
extern pinere_menu_t menu_boot;
extern pinere_menu_t menu_repair;
extern pinere_menu_t menu_storage;
extern pinere_menu_t menu_connectivity;
extern pinere_menu_t menu_system;
extern pinere_menu_t menu_developer;

#endif /* PINERE_MENU_H */
