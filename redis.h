#pragma once
#include <hiredis/hiredis.h>
#include "include/base.h"

redisContext* connectRedis(String redis_url, Arena *arena);

void ensureChatExists(redisContext* redis_conn, char* chat_id, char* remote_jid, char* chat_metadata, char* message_data);

typedef struct {
    String ip;
    String port;
    String user;
    String password;
} Conn;


static Conn parseRedisUrl(Arena* arena, String redis_url) {
    Conn conn = {0};
    String url = redis_url;
    if (url.length >= 9 && strncmp(url.data, "redis://", 8) == 0) {
        url = StrSlice(arena, url, 8, url.length);
    }
    char* at = strchr(url.data, '@');
    if (!at) return conn;
    size_t userpass_len = at - url.data;
    String userpass = StrSlice(arena, url, 0, userpass_len);
    String hostport = StrSlice(arena, url, userpass_len + 1, url.length);
    char* colon = strchr(userpass.data, ':');
    if (colon) {
        size_t user_len = colon - userpass.data;
        conn.user = StrSlice(arena, userpass, 0, user_len);
        conn.password = StrSlice(arena, userpass, user_len + 1, userpass.length);
    } else {
        conn.user = userpass;
        conn.password = (String){0};
    }
    char* colon2 = strrchr(hostport.data, ':');
    if (colon2) {
        size_t ip_len = colon2 - hostport.data;
        conn.ip = StrSlice(arena, hostport, 0, ip_len);
        conn.port = StrSlice(arena, hostport, ip_len + 1, hostport.length);
    } else {
        conn.ip = hostport;
        conn.port = StrNew(arena, "6379");
    }
    return conn;
}