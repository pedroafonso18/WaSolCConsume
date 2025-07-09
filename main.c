#include <stdio.h>
#include "config.h"

int main(void) {
    Arena* arena = ArenaCreate(1024 * 1024);

    Dotenv* dotenv = load_env(arena);

    if (!StrIsNull(dotenv->rabbit_url)) {
        printf("RabbitMq URL: %s\n", dotenv->rabbit_url.data);
    }

    ArenaFree(arena);
}