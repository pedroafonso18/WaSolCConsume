#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
#include "config.h"
#include "include/base.h"
#include "rabbit.h"

static void parse_rabbit_url(const char *url, char *host, int *port, char *user, char *pass, char *vhost) {
    strcpy(host, "localhost");
    *port = 5672;
    strcpy(user, "guest");
    strcpy(pass, "guest");
    strcpy(vhost, "/");
    if (!url) return;
    const char *p = strstr(url, "://");
    if (!p) return;
    p += 3;
    const char *at = strchr(p, '@');
    if (at) {
        const char *colon = strchr(p, ':');
        if (colon && colon < at) {
            strncpy(user, p, colon - p); user[colon - p] = 0;
            strncpy(pass, colon + 1, at - colon - 1); pass[at - colon - 1] = 0;
        }
        p = at + 1;
    }
    const char *slash = strchr(p, '/');
    const char *colon = strchr(p, ':');
    if (colon && (!slash || colon < slash)) {
        strncpy(host, p, colon - p); host[colon - p] = 0;
        *port = atoi(colon + 1);
    } else if (slash) {
        strncpy(host, p, slash - p); host[slash - p] = 0;
    } else {
        strcpy(host, p);
    }
    if (slash) {
        strcpy(vhost, slash);
    }
}

amqp_connection_state_t connect_rabbitmq(Dotenv *env) {
    char host[256], user[128], pass[128], vhost[128];
    int port;
    parse_rabbit_url(env->rabbit_url.data, host, &port, user, pass, vhost);
    amqp_connection_state_t conn = amqp_new_connection();
    amqp_socket_t *socket = amqp_tcp_socket_new(conn);
    if (!socket) {
        fprintf(stderr, "Creating TCP socket failed\n");
        return nullptr;
    }
    if (amqp_socket_open(socket, host, port)) {
        fprintf(stderr, "Opening TCP socket failed\n");
        return nullptr;
    }
    amqp_rpc_reply_t login_reply = amqp_login(
        conn, vhost, 0, 131072, 0,
        AMQP_SASL_METHOD_PLAIN, user, pass
    );
    if (login_reply.reply_type != AMQP_RESPONSE_NORMAL) {
        fprintf(stderr, "RabbitMQ login failed\n");
        return nullptr;
    }
    return conn;
}

void create_rabbitmq_consumer(Dotenv *env, const char *queue_name) {
    amqp_connection_state_t conn = connect_rabbitmq(env);
    if (!conn) return;
    amqp_channel_open(conn, 1);
    if (amqp_get_rpc_reply(conn).reply_type != AMQP_RESPONSE_NORMAL) {
        fprintf(stderr, "Opening channel failed\n");
        return;
    }
    amqp_basic_consume(conn, 1, amqp_cstring_bytes(queue_name), amqp_cstring_bytes("WasolConsumer"), 0, 1, 0, amqp_empty_table);
    if (amqp_get_rpc_reply(conn).reply_type != AMQP_RESPONSE_NORMAL) {
        fprintf(stderr, "Basic consume failed\n");
        return;
    }
    // ... Add your message loop here ...
}
