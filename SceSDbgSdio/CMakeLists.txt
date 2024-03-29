cmake_minimum_required(VERSION 3.0)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(SceSDbgSdio)
set(SELF_NAME "sdbgsdio")
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-q -Wall -O2 -fno-inline")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions")

link_directories(
  ${CMAKE_SOURCE_DIR}/extra/
)

add_executable(${PROJECT_NAME}
  src/sdbgsdio.c
)

target_link_libraries(${PROJECT_NAME}
  SceCpuForDriver_stub
  SceDipswForDriver_stub
  SceSysmemForDriver_stub
  SceSysclibForDriver_stub
  SceIntrmgrForDriver_stub
  SceKernelSuspendForDriver_stub
  SceSysrootForKernel_stub
)

set_target_properties(${PROJECT_NAME}
  PROPERTIES LINK_FLAGS "-nostdlib"
  COMPILE_FLAGS "-D__VITA_KERNEL__"
)

vita_create_self(${SELF_NAME}.skprx ${PROJECT_NAME}
  CONFIG exports.yml
  UNSAFE
  STRIPPED
)
