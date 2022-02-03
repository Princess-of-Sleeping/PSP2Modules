cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(SceSblACMgr)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-q -Wall -Os -fno-inline -fno-builtin")

set(VITA_MAKE_FSELF_FLAGS "${VITA_MAKE_FSELF_FLAGS} -a 0x2808000000020002")

enable_language(C ASM)

add_executable(${PROJECT_NAME}
  src/acmgr.c
  src/acmgr_media.c
  src/acmgr_pfs_attr.c
  src/acmgr_has_flag.c
  src/acmgr_is_program.c
  src/acmgr_is_allow.c
  src/acmgr_user.c
)

target_link_libraries(${PROJECT_NAME}
  SceDipswForDriver_stub
  SceSysmemForDriver_stub
  SceSysclibForDriver_stub
  SceSysrootForKernel_stub
  SceQafMgrForDriver_stub
  ${CMAKE_CURRENT_BINARY_DIR}/../extra/libSceQafMgrForDriver_stub.a
  ${CMAKE_CURRENT_BINARY_DIR}/../extra/libSceSysrootForDriver_stub.a
  ${CMAKE_CURRENT_BINARY_DIR}/../extra/libSceSysrootForKernel_stub.a
)

set_target_properties(${PROJECT_NAME}
  PROPERTIES LINK_FLAGS "-nostdlib"
  COMPILE_FLAGS "-D__PSP2_KERNEL__"
)

vita_create_self(acmgr.skprx ${PROJECT_NAME}
  CONFIG exports.yml
  STRIPPED
  REL_OPTIMIZE
)

vita_create_stubs(stubs ${PROJECT_NAME} exports.yml KERNEL)
