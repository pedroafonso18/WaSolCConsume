#pragma once
#include "include/base.h"

static String normalizeChatId(Arena *arena, String jid) {
    char *at = strchr(jid.data, '@');
    if (!at) {
        return StrNew(arena, jid.data);
    }
    size_t number_len = at - jid.data;
    if (number_len < 12 || strncmp(jid.data, "55", 2) != 0) {
        return StrNew(arena, jid.data);
    }
    char country_code[3] = {0};
    char area_code[3] = {0};
    char rest[32] = {0};
    memcpy(country_code, jid.data, 2);
    memcpy(area_code, jid.data + 2, 2);
    size_t rest_len = number_len - 4 > 31 ? 31 : number_len - 4;
    memcpy(rest, jid.data + 4, rest_len);
    rest[rest_len] = '\0';
    String new_rest;
    if (rest[0] == '9') {
        new_rest = StrNew(arena, rest);
    } else {
        new_rest = F(arena, "9%s", rest);
    }
    String normalized_number = F(arena, "%s%s%s", country_code, area_code, new_rest.data);
    String domain = StrNew(arena, at + 1);
    return F(arena, "%s@%s", normalized_number.data, domain.data);
}