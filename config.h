#pragma once
#include "include/base.h"

/* This is the Dotenv type, which will be used thorough the project to
 * load Environment variables, we are using one dependency here, to load the actual variables. */

typedef struct {
    String rabbit_url;
    String db_url;
    String redis_url;
} Dotenv;

Dotenv* load_env(Arena* arena);