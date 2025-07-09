cmake_minimum_required(VERSION 3.31)
project(WaSolCConsume C)

set(CMAKE_C_STANDARD 23)

add_executable(WaSolCConsume main.c
        config.c
        config.h
        include/dotenv.c
        base_impl.c
        rabbit.c
        rabbit.h
        api.c
        api.h
        database.c
        database.h
        redis.c
        redis.h
        utils.h
)

if (MSVC)
    target_compile_options(WaSolCConsume PRIVATE /W4 /WX)
else()
    target_compile_options(WaSolCConsume PRIVATE -Wall -Wextra -Werror)
endif()
