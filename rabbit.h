#pragma once
#include <rabbitmq-c/amqp.h>
#include "config.h"

amqp_connection_state_t connect_rabbitmq(Dotenv *env);

void create_rabbitmq_consumer(Dotenv *env, const char *queue_name);
