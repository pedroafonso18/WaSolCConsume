#include "library.h"
#include <cjson/cJSON.h>

Request parse_request_from_json(Arena* arena, const char* json_str) {
    Request req = {0};
    cJSON* root = cJSON_Parse(json_str);
    if (!root) return req;

    cJSON* action = cJSON_GetObjectItem(root, "action");
    if (cJSON_IsString(action)) req.action = StrNew(arena, action->valuestring);

    cJSON* method = cJSON_GetObjectItem(root, "method");
    if (cJSON_IsString(method)) req.method = StrNew(arena, method->valuestring);

    cJSON* url = cJSON_GetObjectItem(root, "url");
    if (cJSON_IsString(url)) req.url = StrNew(arena, url->valuestring);

    cJSON* headers = cJSON_GetObjectItem(root, "headers");
    if (cJSON_IsArray(headers)) {
        cJSON* header = NULL;
    }
    return req;
}

Chat parse_chat_from_json(Arena* arena, const char* json_str) {
    Chat chat = {0};
    cJSON* root = cJSON_Parse(json_str);
    if (!root) return chat;
    cJSON* id = cJSON_GetObjectItem(root, "id");
    if (cJSON_IsNumber(id)) chat.id = id->valueint;
    cJSON* situation = cJSON_GetObjectItem(root, "situation");
    if (cJSON_IsString(situation)) chat.situation = StrNew(arena, situation->valuestring);
    cJSON* is_active = cJSON_GetObjectItem(root, "is_active");
    if (cJSON_IsBool(is_active)) chat.is_active = cJSON_IsTrue(is_active);
    cJSON* agent_id = cJSON_GetObjectItem(root, "agent_id");
    if (cJSON_IsNumber(agent_id)) chat.agent_id = agent_id->valueint;
    cJSON* tabulation = cJSON_GetObjectItem(root, "tabulation");
    if (cJSON_IsString(tabulation)) chat.tabulation = StrNew(arena, tabulation->valuestring);
    cJSON* customer_id = cJSON_GetObjectItem(root, "customer_id");
    if (cJSON_IsNumber(customer_id)) chat.customer_id = customer_id->valueint;
    cJSON_Delete(root);
    return chat;
}

Message parse_message_from_json(Arena* arena, const char* json_str) {
    Message msg = {0};
    cJSON* root = cJSON_Parse(json_str);
    if (!root) return msg;
    cJSON* id = cJSON_GetObjectItem(root, "id");
    if (cJSON_IsNumber(id)) msg.id = id->valueint;
    cJSON* from = cJSON_GetObjectItem(root, "from");
    if (cJSON_IsString(from)) msg.from = StrNew(arena, from->valuestring);
    cJSON* to = cJSON_GetObjectItem(root, "to");
    if (cJSON_IsString(to)) msg.to = StrNew(arena, to->valuestring);
    cJSON* delivered = cJSON_GetObjectItem(root, "delivered");
    if (cJSON_IsBool(delivered)) msg.delivered = cJSON_IsTrue(delivered);
    cJSON* text = cJSON_GetObjectItem(root, "text");
    if (cJSON_IsString(text)) msg.text = StrNew(arena, text->valuestring);
    cJSON* chat_id = cJSON_GetObjectItem(root, "chat_id");
    if (cJSON_IsNumber(chat_id)) msg.chat_id = chat_id->valueint;
    cJSON_Delete(root);
    return msg;
}

Customer parse_customer_from_json(Arena* arena, const char* json_str) {
    Customer cust = {0};
    cJSON* root = cJSON_Parse(json_str);
    if (!root) return cust;
    cJSON* id = cJSON_GetObjectItem(root, "id");
    if (cJSON_IsNumber(id)) cust.id = id->valueint;
    cJSON* name = cJSON_GetObjectItem(root, "name");
    if (cJSON_IsString(name)) cust.name = StrNew(arena, name->valuestring);
    cJSON* number = cJSON_GetObjectItem(root, "number");
    if (cJSON_IsString(number)) cust.number = StrNew(arena, number->valuestring);
    cJSON* last_chat_id = cJSON_GetObjectItem(root, "last_chat_id");
    if (cJSON_IsString(last_chat_id)) cust.last_chat_id = StrNew(arena, last_chat_id->valuestring);
    cJSON_Delete(root);
    return cust;
}
