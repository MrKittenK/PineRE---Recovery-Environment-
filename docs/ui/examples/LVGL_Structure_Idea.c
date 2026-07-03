/* Basic LVGL structure idea for PineRE */
lv_obj_t * menu = lv_list_create(lv_scr_act());
lv_obj_set_size(menu, 720, 720);
lv_list_add_btn(menu, NULL, "Boot Manager");
lv_list_add_btn(menu, NULL, "Repair Installation");
/* ... and so on */
