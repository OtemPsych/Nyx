include(FetchContent)

add_library(NyxEngine)

if (BUILD_SHARED_LIBS)
    target_compile_definitions(NyxEngine
        PUBLIC NYX_SHARED
        PRIVATE NYX_API_EXPORTING
    )
endif()

set_target_properties(NyxEngine PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    CXX_VISIBILITY_PRESET hidden
    POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS}
    LINKER_LANGUAGE CXX
)

if (MSVC)
    target_compile_options(NyxEngine
        PUBLIC $<$<COMPILE_LANGUAGE:CXX>:/Zc:preprocessor>
        PRIVATE /W4
    )
else()
    target_compile_options(NyxEngine
        PRIVATE -Wall -Wextra -Wpedantic
    )
endif()

# -- Dependencies --
FetchContent_Declare(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_Declare(entt
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG v3.15.0
)
FetchContent_Declare(stb_image
    URL https://raw.githubusercontent.com/nothings/stb/refs/heads/master/stb_image.h
    DOWNLOAD_NO_EXTRACT ON
)
FetchContent_Declare(vma
    URL https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator/blob/master/include/vk_mem_alloc.h
    DOWNLOAD_NO_EXTRACT ON
)
FetchContent_MakeAvailable(glfw entt stb_image vma)

find_package(Vulkan REQUIRED)

target_link_libraries(NyxEngine
    PUBLIC EnTT::EnTT Vulkan::Vulkan glfw
)
target_include_directories(NyxEngine
    PUBLIC include
    PRIVATE ${stb_image_SOURCE_DIR}
)
target_sources(NyxEngine PRIVATE "include/nyx/core/math/vector.h" "include/nyx/core/application.cpp" "include/nyx/core/math/util.h")
