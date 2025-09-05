cmake_minimum_required(VERSION 3.24...4.0)
include(FetchContent)

add_library(NyxEngine)

if(BUILD_SHARED_LIBS)
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
)

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
FetchContent_MakeAvailable(glfw entt stb_image)

find_package(Vulkan REQUIRED)

target_include_directories(NyxEngine
  PUBLIC include
  PRIVATE ${stb_image_SOURCE_DIR}
)
target_link_libraries(NyxEngine
  PUBLIC EnTT::EnTT Vulkan::Vulkan
  PRIVATE glfw
)
target_sources(NyxEngine PRIVATE
 "include/nyx/core/config.h" "include/nyx/core/assert.h" "include/nyx/core/logging/log.h" "src/core/logging/log.cpp")
