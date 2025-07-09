#pragma once
#include "include/base.h"
#include <cjson/cJSON.h>

/* Library will hold the structs and types for most of the project, including the structs i will parse json into.*/


/* ====== [REQUEST TYPES] ====== */

typedef struct {
    String key;
    String value;
} KeyValue;

VEC_TYPE(KeyValueVec, KeyValue);

typedef struct {
    String action;
    String method;
    String url;
    KeyValueVec headers;
    cJSON body;
} Request;

/* ====== [REQUEST TYPES] ====== */


/* ====== [WASOL TYPES] ====== */

typedef struct {
    i32 id;
    String situation;
    bool is_active;
    i32 agent_id;
    String tabulation;
    i32 customer_id;
} Chat;

typedef struct {
    i32 id;
    String from;
    String to;
    bool delivered;
    String text;
    i32 chat_id;
} Message;

typedef struct {
    i32 id;
    String name;
    String number;
    String last_chat_id;
} Customer;

/* ====== [WASOL TYPES] ====== */


/* ====== [WEBHOOK TYPES] ====== */

typedef struct {
    String message_remotejid;
    String message_conversation;
    String message_status_string;
    String key;
    String message;
} Webhook;

/* ====== [WEBHOOK TYPES] ====== */
