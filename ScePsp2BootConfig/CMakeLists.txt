cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(ScePsp2BootConfig)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-q -Wall -O3 -fno-inline")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions")

link_directories(
  ${CMAKE_SOURCE_DIR}/extra/
)

add_executable(ScePsp2BootConfig
  src/psp2bootconfig.c
)

target_link_libraries(ScePsp2BootConfig
  SceKblForKernel_stub.a
)

set_target_properties(ScePsp2BootConfig
  PROPERTIES LINK_FLAGS "-nostdlib"
  COMPILE_FLAGS "-D__VITA_KERNEL__"
)

vita_create_self(psp2bootconfig.skprx ScePsp2BootConfig
  CONFIG exports.yml
  UNSAFE
)
