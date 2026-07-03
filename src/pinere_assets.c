#include "pinere_menu.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
const char* lookup_asset_text(const char *key_tag) {
    static char buffer[TEXT_BUFFER_SIZE];
    char section_header[TEXT_BUFFER_SIZE];
    snprintf(section_header, sizeof(section_header), "[%s]", key_tag);
    FILE *fp = fopen(ASSET_FILE_PATH, "r");
    if (!fp) return key_tag;
    char line[TEXT_BUFFER_SIZE];
    bool found_header = false;
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '#' || isspace(line[0])) continue;
        if (!found_header) { if (strstr(line, section_header)) found_header = true; }
        else {
            line[strcspn(line, "\r\n")] = 0;
            if (strlen(line) > 0) { strncpy(buffer, line, sizeof(buffer) - 1); buffer[sizeof(buffer) - 1] = '\0'; fclose(fp); return buffer; }
        }
    }
    fclose(fp);
    return key_tag;
}