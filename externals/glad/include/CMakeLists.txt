include_directories(${GLAD_DIR}/include)

add_library(gladstatic STATIC ${GLAD_DIR}/src/glad.c)