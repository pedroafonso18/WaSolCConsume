#include "redis.h"
#include "utils.h"
#include <jansson.h>
#include <stdlib.h>

redisContext* connectRedis(String redis_url, Arena *arena) {
    const Conn conn = parseRedisUrl(arena, redis_url);
    char *endptr;
    errno = 0;
    const long val = strtol(conn.port.data, &endptr, 10);
    if (*endptr != '\0' || errno != 0) {
        printf("Error: Couldn't convert port string to integer.\n");
        return nullptr;
    }
    if (val < 0 || val > 65535) {
        printf("Error: Port number out of range.\n");
        return nullptr;
    }
    const int port = (int)val;

    if (!conn.ip.data) {
        printf("Error: IP address missing.\n");
        return nullptr;
    }

    redisContext* c = redisConnect(conn.ip.data, port);
    if (c == nullptr || c->err) {
        if (c) {
            printf("Error: Couldn't connect to Redis: %s\n", c->errstr);
            redisFree(c);
        } else {
            printf("Error: Couldn't allocate redis context.\n");
        }
        return nullptr;
    }

    if (conn.password.data && conn.password.data[0] != '\0') {
        redisReply *reply = nullptr;
        if (conn.user.data && conn.user.data[0] != '\0') {
            reply = redisCommand(c, "AUTH %s %s", conn.user.data, conn.password.data);
        } else {
            reply = redisCommand(c, "AUTH %s", conn.password.data);
        }
        if (!reply) {
            printf("AUTH command failed.\n");
            redisFree(c);
            return nullptr;
        }
        if (reply->type == REDIS_REPLY_ERROR) {
            printf("AUTH error: %s\n", reply->str);
            freeReplyObject(reply);
            redisFree(c);
            return nullptr;
        }
        freeReplyObject(reply);
    }

    return c;
}


void ensureChatExists(redisContext* redis_conn, Arena* arena, String chat_id, String remote_jid, String chat_metadata, String message_data) {
    String norm_chat_id = normalizeChatId(arena, chat_id);
    String chat_key = F(arena, "chat:%s", norm_chat_id.data);
    redisReply* exists_reply = redisCommand(redis_conn, "EXISTS %s", chat_key.data);
    if (!exists_reply) {
        LogError("Redis EXISTS command failed.");
        return;
    }
    int exists = 0;
    if (exists_reply->type == REDIS_REPLY_INTEGER) {
        exists = exists_reply->integer;
    }
    freeReplyObject(exists_reply);
    if (!exists) {
        String chat_data;
        if (!StrIsNull(chat_metadata)) {
            chat_data = chat_metadata;
        } else {
            char* at = strchr(remote_jid.data, '@');
            String number = at ? StrSlice(arena, remote_jid, 0, at - remote_jid.data) : remote_jid;
            String instance_id = (String){0};
            if (!StrIsNull(message_data)) {
                json_error_t error;
                json_t* root = json_loads(message_data.data, 0, &error);
                if (root) {
                    json_t* apikey = json_object_get(root, "apikey");
                    if (apikey && json_is_string(apikey)) {
                        instance_id = StrNew(arena, (char*)json_string_value(apikey));
                    }
                    json_decref(root);
                }
            }
            chat_data = F(arena,
                "{\"id\":\"%s\",\"situation\":\"enqueued\",\"is_active\":true,\"agent_id\":null,\"tabulation\":null,\"instance_id\":\"%s\",\"number\":\"%s\"}",
                norm_chat_id.data, instance_id.data ? instance_id.data : "", number.data);
        }
        redisReply* rpush_reply = redisCommand(redis_conn, "RPUSH %s %s", chat_key.data, chat_data.data);
        if (rpush_reply) freeReplyObject(rpush_reply);
        LogInfo("Created new chat entry in Redis (as list): %s", chat_key.data);
        redisReply* sadd_reply = redisCommand(redis_conn, "SADD chats %s", norm_chat_id.data);
        if (sadd_reply) freeReplyObject(sadd_reply);
        LogInfo("Added chat_id %s to 'chats' set", norm_chat_id.data);
    } else {
        LogInfo("Chat entry already exists in Redis: %s", chat_key.data);
    }
}

void insertMessageToChat(redisContext* redis_conn, Arena* arena, String chat_id, String message_json, String remote_jid, String chat_metadata, String message_data) {
    String norm_chat_id = normalizeChatId(arena, chat_id);
    printf("Inserting message into chat:%s for remote_jid:%s\n", norm_chat_id.data, remote_jid.data);
    ensureChatExists(redis_conn, arena, norm_chat_id, remote_jid, chat_metadata, message_data);
    String key = F(arena, "chat:%s:messages", norm_chat_id.data);
    printf("Pushing message to Redis list: %s\n", key.data);
    redisReply* rpush_reply = redisCommand(redis_conn, "RPUSH %s %s", key.data, message_json.data);
    if (rpush_reply) freeReplyObject(rpush_reply);
    printf("Successfully inserted message into Redis for chat:%s\n", norm_chat_id.data);
}
