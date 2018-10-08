# engine
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/engine)
add_subdirectory(engine)

# game source
set(GAME_SRC
	simphy/main.cpp
)

add_executable(${PROJECT_NAME} ${GAME_SRC})
# link engine library
target_link_libraries(${PROJECT_NAME}  engine-lib)
