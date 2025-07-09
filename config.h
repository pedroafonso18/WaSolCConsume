#pragma once

/* This is the Dotenv type, which will be used thorough the project to
 * load Environment variables, we are using one dependency here, to load the actual variables. */

typedef struct {
    char* rabbit_url;
    char* db_url;
    char* redis_url;
} Dotenv;

Dotenv* load_env();