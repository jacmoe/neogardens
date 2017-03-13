set(TARGET_NAME pal)

set(Sources
    ./main.c
)


set(Common_Headers
    ${COMMON_INC}/app.h
)

add_executable(${TARGET_NAME}
    ${Sources}
    ${Common_Headers}
)

set_property(TARGET ${TARGET_NAME} PROPERTY C_STANDARD 99) # 90 99 11
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra")

include_directories(
    ${OPENGL_INCLUDE_DIRS}
    ${COMMON_INC}
)

target_link_libraries(${TARGET_NAME}
    ${OPENGL_LIBRARIES}
)

set_property(TARGET ${TARGET_NAME} PROPERTY C_STANDARD 99) # 90 99 11
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra")
