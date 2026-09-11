include(FetchContent)

add_executable(NyxTestbed "src/main.cpp")

target_link_libraries(NyxTestbed PRIVATE NyxEngine)
set_target_properties(NyxTestbed PROPERTIES
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)

FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.3
)
FetchContent_MakeAvailable(glm)