cmake_minimum_required(VERSION 3.0)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(SceKernelIntrMgr)
set(SELF_NAME "intrmgr")
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-q -Wall -O2 -fno-inline")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions")

set(VITA_MAKE_FSELF_FLAGS "${VITA_MAKE_FSELF_FLAGS} -a 0x2808000000020025")

enable_language(C ASM)

link_directories(
  ${CMAKE_SOURCE_DIR}/extra/
)

add_executable(${PROJECT_NAME}
  src/irq_handler.S
  src/irq_stack.S
  src/SceIntrmgrForKernel_A60D79A4.S
  src/irq_callback.c
  src/svc_handler.S
  src/intrmgr.c
  src/SceIntrmgrForKernel_01E5233E.S
  src/setjmp.S
)

target_link_libraries(${PROJECT_NAME}
  SceCpuForDriver_stub
  SceCpuForKernel_stub
  SceDebugForDriver_stub
  SceSysmemForDriver_stub
  SceSysmemForKernel_stub
  SceSysclibForDriver_stub
  SceExcpmgrForKernel_stub
  SceKernelSuspendForDriver_stub
  SceSysrootForDriver_stub
  SceSysrootForKernel_stub
)

set_target_properties(${PROJECT_NAME}
  PROPERTIES LINK_FLAGS "-nostdlib"
  COMPILE_FLAGS "-D__PSP2_KERNEL__"
)

vita_create_self(${SELF_NAME}.skprx ${PROJECT_NAME}
  CONFIG exports.yml
  UNSAFE
  STRIPPED
  REL_OPTIMIZE
)
