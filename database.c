#include "database.h"

PGconn* connect_db(char* db_url) {
    PGconn* conn = PQconnectdb(db_url);
    if (PQstatus(conn) != CONNECTION_OK) {
        printf("Connection to DB failed: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        return nullptr;
    }
    return conn;
}

void upsert_chats(PGconn* client, Chat* chat) {
    if (PQstatus(client) != CONNECTION_OK) {
        printf("Connection to DB failed: %s\n", PQerrorMessage(client));
        PQfinish(client);
        return;
    }

    PGresult *res = nullptr;

    if (strcmp(chat->tabulation.data, "") == 0 ) {
        const char *paramValues[5];
        paramValues[0] = chat->id;
        paramValues[1] = chat->situation.data;
        paramValues[2] = chat->is_active ? "1" : "0";
        paramValues[3] = chat->agent_id;
        paramValues[4] = chat->customer_id;
        res = PQexecParams(
            client,
            "INSERT INTO chats (id, situation, is_active, agent_id, customer_id) VALUES ($1, $2, $3, $4, $5) ON CONFLICT (id) DO UPDATE SET situation = $2, is_active = $3, agent_id = $4, customer_id = $5",
            5,
            NULL,
            paramValues,
            NULL,
            NULL,
            0);
        if (res == NULL) {
            printf("Insert failed: could not allocate result.\n");
            return;
        }
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            printf("Insert failed: %s\n", PQresultErrorMessage(res));
        } else {
            printf("Insert successfull!\n");
        }
    } else {
        const char *paramValues[6];
        paramValues[0] = chat->id;
        paramValues[1] = chat->situation.data;
        paramValues[2] = chat->is_active ? "1" : "0";
        paramValues[3] = chat->agent_id;
        paramValues[4] = chat->customer_id;
        paramValues[5] = chat->tabulation.data;
        res = PQexecParams(
            client,
            "INSERT INTO chats (id, situation, is_active, agent_id, customer_id, tabulation) VALUES ($1, $2, $3, $4, $5, $6) ON CONFLICT (id) DO UPDATE SET situation = $2, is_active = $3, agent_id = $4, tabulation = $6, customer_id = $5",
            6,
            NULL,
            paramValues,
            NULL,
            NULL,
            0);
        if (res == NULL) {
            printf("Insert failed: could not allocate result.\n");
            return;
        }
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            printf("Insert failed: %s\n", PQresultErrorMessage(res));
        } else {
            printf("Insert successfull!\n");
        }
    }

    if (res != NULL) {
        PQclear(res);
    }

}

void upsert_messages(PGconn* client, Message* messages) {
    if (PQstatus(client) != CONNECTION_OK) {
        printf("Connection to DB failed: %s\n", PQerrorMessage(client));
        PQfinish(client);
        return;
    }

    PGresult *res = nullptr;

        const char *paramValues[6];
        paramValues[0] = messages->id;
        paramValues[1] = messages->from.data;
        paramValues[2] = messages->to.data;
        paramValues[3] = messages->text.data;
        paramValues[4] = messages->delivered ? "1" : "0";
        paramValues[5] = messages->chat_id;

        res = PQexecParams(
            client,
            "INSERT INTO messages (id, from, to, text, delivered, chat_id) VALUES ($1, $2, $3, $4, $5, $6) ON CONFLICT (id) DO UPDATE SET from = $2, to = $3, text = $4, delivered = $5, chat_id = $6",
            6,
            NULL,
            paramValues,
            NULL,
            NULL,
            0);
        if (res == NULL) {
            printf("Insert failed: could not allocate result.\n");
            return;
        }
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            printf("Insert failed: %s\n", PQresultErrorMessage(res));
        } else {
            printf("Insert successfull!\n");
        }

    if (res != NULL) {
        PQclear(res);
    }

}

void upsert_customer(PGconn* client, Customer* customer) {
    if (PQstatus(client) != CONNECTION_OK) {
        printf("Connection to DB failed: %s\n", PQerrorMessage(client));
        PQfinish(client);
        return;
    }

    PGresult *res = nullptr;

    if (strcmp(customer->last_chat_id.data, "") == 0 ) {
        const char *paramValues[3];
        paramValues[0] = customer->id;
        paramValues[1] = customer->name.data;
        paramValues[2] = customer->number.data;
        res = PQexecParams(
            client,
            "INSERT INTO customers (id, name, number) VALUES ($1, $2, $3) ON CONFLICT (id) DO UPDATE SET name = $2, number = $3",
            3,
            NULL,
            paramValues,
            NULL,
            NULL,
            0);
        if (res == NULL) {
            printf("Insert failed: could not allocate result.\n");
            return;
        }
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            printf("Insert failed: %s\n", PQresultErrorMessage(res));
        } else {
            printf("Insert successfull!\n");
        }
    } else {
        const char *paramValues[4];
        paramValues[0] = customer->id;
        paramValues[1] = customer->name.data;
        paramValues[2] = customer->number.data;
        paramValues[3] = customer->last_chat_id.data;

        res = PQexecParams(
            client,
            "INSERT INTO customers (id, name, number, last_chat_id) VALUES ($1, $2, $3, $4) ON CONFLICT (id) DO UPDATE SET name = $2, number = $3, last_chat_Id = $4",
            4,
            NULL,
            paramValues,
            NULL,
            NULL,
            0);
        if (res == NULL) {
            printf("Insert failed: could not allocate result.\n");
            return;
        }
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            printf("Insert failed: %s\n", PQresultErrorMessage(res));
        } else {
            printf("Insert successfull!\n");
        }
    }

    if (res != NULL) {
        PQclear(res);
    }

}