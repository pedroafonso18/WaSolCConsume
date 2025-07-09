#include "config.h"
#include "include/dotenv.h"
#include "include/base.h"

Dotenv* load_env(Arena* arena) {
    env_load("../.env", false);

    char* rabbit_url = getenv("RABBIT_URL");
    char* db_url = getenv("DB_URL");
    char* redis_url = getenv("REDIS_URL");

    Dotenv* dotenv = ArenaAlloc(arena, sizeof(Dotenv));

    if (rabbit_url) {
        dotenv->rabbit_url = StrNew(arena, rabbit_url);
    } else {
        dotenv->rabbit_url = (String){0};
    }

    if (db_url) {
        dotenv->db_url = StrNew(arena, db_url);
    } else {
        dotenv->db_url = (String){0};
    }

    if (redis_url) {
        dotenv->redis_url = StrNew(arena, redis_url);
    } else {
        dotenv->redis_url = (String){0};
    }

    return dotenv;
}