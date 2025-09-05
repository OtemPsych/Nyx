cmake_minimum_required(VERSION 3.13)

add_executable(NyxTestbed
  "src/main.cpp"
)

target_link_libraries(NyxTestbed PRIVATE NyxEngine)
set_target_properties(NyxTestbed PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

if(BUILD_SHARED_LIBS)
  add_custom_command(TARGET NyxTestbed POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${NyxEngine_BINARY_DIR}/NyxEngine.dll"
      NyxTestbed_BINARY_DIR
  )
endif()
