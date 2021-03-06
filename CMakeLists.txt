project(Liseuse)

find_package(wxWidgets)

include(${wxWidgets_USE_FILE})

add_executable(${PROJECT_NAME} test.cpp)
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})

