cmake_minimum_required(VERSION 3.24...4.0)
project(Nyx
  VERSION 1.0
  DESCRIPTION "Game engine using C++ and Vulkan"
  LANGUAGES CXX
)

add_subdirectory(NyxEngine)
add_subdirectory(NyxTestbed)
