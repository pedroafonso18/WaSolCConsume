#include "process.h"
#include "include/base.h"
#include <string.h>
#include "library.h"
#include "database.h"
#include "api.h"

void process_outgoing(char* data, PGconn* client, redisContext* conn, Arena* arena) {
    if (!data || !client || !arena) {
        LogError("process_outgoing: Invalid arguments (data, client, or arena is NULL)");
        return;
    }
    if (strstr(data, "upsertChat") != nullptr) {
        LogInfo("Starting UpsertChat process...");
        Chat chat = parse_chat_from_json(arena, data);
        if (StrIsNull(chat.situation)) {
            LogError("UpsertChat: Failed to parse chat from JSON: %s", data);
            return;
        }
        upsert_chats(client, &chat);
        LogSuccess("UpsertChat process completed.");
    } else if (strstr(data, "upsertCustomer") != nullptr) {
        LogInfo("Starting UpsertCustomer process...");
        Customer customer = parse_customer_from_json(arena, data);
        if (StrIsNull(customer.name)) {
            LogError("UpsertCustomer: Failed to parse customer from JSON: %s", data);
            return;
        }
        upsert_customer(client, &customer);
        LogSuccess("UpsertCustomer process completed.");
    } else if (strstr(data, "sendMessage") != nullptr) {
        LogInfo("Starting UpsertMessage process...");
        Message message = parse_message_from_json(arena, data);
        if (StrIsNull(message.from) || StrIsNull(message.to)) {
            LogError("UpsertMessage: Failed to parse message from JSON: %s", data);
            return;
        }
        upsert_messages(client, &message);
        LogSuccess("UpsertMessage process completed.");
    } else if (strstr(data, "sendRequest") != nullptr) {
        LogInfo("Starting SendRequest process...");
        Request req = parse_request_from_json(arena, data);
        if (StrIsNull(req.action) || StrIsNull(req.method) || StrIsNull(req.url)) {
            LogError("SendRequest: Failed to parse request from JSON: %s", data);
            return;
        }
        make_request(&req, arena);
        LogSuccess("SendRequest process completed.");
    } else {
        LogWarn("Unknown message type. Message content: %s", data);
    }
}
