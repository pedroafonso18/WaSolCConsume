#pragma once

#include <libpq-fe.h>
#include <hiredis/hiredis.h>

void process_outgoing(char* data, PGconn* client, redisContext* conn);

void process_incoming(char* data, PGconn* client, redisContext* conn);
