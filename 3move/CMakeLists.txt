set(TARGET_NAME move)

set(Sources
    ./main.c
)

add_executable(${TARGET_NAME}
    ${Sources}
)

set_property(TARGET ${TARGET_NAME} PROPERTY C_STANDARD 99) # 90 99 11
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra")

target_link_libraries(${TARGET_NAME}
    m
    ${CONAN_LIBS}
    ${SDL2_LIBRARIES}
    ${SDL2IMAGE_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
)

set_property(TARGET ${TARGET_NAME} PROPERTY C_STANDARD 99) # 90 99 11
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra")
