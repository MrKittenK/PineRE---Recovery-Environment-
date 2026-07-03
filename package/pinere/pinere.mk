# ==============================================================================
# Buildroot Recipe for PineRE Maintenance Interface Environment Block
# ==============================================================================

PINERE_VERSION = 1.0.0
PINERE_SITE = $(BR2_EXTERNAL_PINERE_PATH)/src
PINERE_SITE_METHOD = local
PINERE_DEPENDENCIES = lvgl

define PINERE_BUILD_CMDS
	$(TARGET_CC) $(TARGET_CFLAGS) -Os -I$(STAGING_DIR)/usr/include \
	    $(PINERE_SITE)/pinere_menu.c $(PINERE_SITE)/main.c \
	    -o $(@D)/pinere -L$(STAGING_DIR)/usr/lib -llvgl $(TARGET_LDFLAGS)
enddefine

define PINERE_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/pinere $(TARGET_DIR)/usr/bin/pinere
	$(INSTALL) -D -m 0644 $(PINERE_SITE)/menu_assets.txt $(TARGET_DIR)/usr/share/pinere/menu_assets.txt
enddefine

$(eval $(generic-package))
