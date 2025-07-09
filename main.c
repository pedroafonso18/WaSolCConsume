#include <stdio.h>
#include "config.h"

int main(void) {
    Arena* arena = ArenaCreate(1024 * 1024);

    Dotenv* dotenv = load_env(arena);


    ArenaFree(arena);
}