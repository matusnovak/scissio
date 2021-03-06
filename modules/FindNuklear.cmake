include(FindPackageHandleStandardArgs)

if(NOT TARGET Nuklear)
  set(NUKLEAR_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/Nuklear/src)
  mark_as_advanced(FORCE NUKLEAR_INCLUDE_DIR)
  file(GLOB NUKLEAR_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/libs/Nuklear/src/*.c)
  add_library(Nuklear STATIC ${NUKLEAR_SOURCES}) 
  target_compile_definitions(Nuklear PUBLIC NK_INCLUDE_DEFAULT_ALLOCATOR=1)
  target_compile_definitions(Nuklear PRIVATE NK_MEMSET=nk_memset NK_MEMCPY=nk_memcopy)
  set_property(TARGET Nuklear PROPERTY POSITION_INDEPENDENT_CODE ON)
  set_target_properties(Nuklear PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${NUKLEAR_INCLUDE_DIR})
endif()

find_package_handle_standard_args(Nuklear DEFAULT_MSG NUKLEAR_INCLUDE_DIR)
