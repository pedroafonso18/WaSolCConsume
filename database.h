#pragma once

#include <libpq-fe.h>
#include "library.h"

PGconn* connect_db(char* db_url);

void upsert_chats(PGconn* client, Chat* chat);

void upsert_messages(PGconn* client, Message* message);

void upsert_customer(PGconn* client, Customer* customer);