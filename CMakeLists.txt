cmake_minimum_required(VERSION 3.15.20190914 FATAL_ERROR)
project(application DESCRIPTION "Application" VERSION 0.1.0 LANGUAGES CXX)

set(PROJECT_VENDOR "Company")
configure_file(res/version.h.in ${CMAKE_CURRENT_BINARY_DIR}/src/version.h LF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32)
  configure_file(res/launch.vs.json.in ${CMAKE_CURRENT_SOURCE_DIR}/.vs/launch.vs.json LF)
  add_definitions(-D_UNICODE -DUNICODE -DWIN32_LEAN_AND_MEAN -DNOMINMAX -DWINVER=0x0A00 -D_WIN32_WINNT=0x0A00)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE)
  add_definitions(-D_ATL_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS)
else()
  set(CMAKE_INSTALL_RPATH "$ORIGIN/lib")
endif()

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_LIST_DIR}/res/cmake)

file(GLOB_RECURSE headers CONFIGURE_DEPENDS src/*.hpp)
file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.cpp src/*.manifest src/*.rc)

add_executable(${PROJECT_NAME} ${headers} ${sources})
target_include_directories(${PROJECT_NAME}
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)
target_compile_definitions(${PROJECT_NAME} PRIVATE FMT_EXCEPTIONS=0)

find_package(TBB CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE TBB::tbb)
target_compile_definitions(${PROJECT_NAME} PRIVATE TBB_USE_EXCEPTIONS=0)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION .)

if(WIN32)
  file(GLOB THIRD_PARTY_LIBRARIES CONFIGURE_DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/*.dll")
  install(FILES ${THIRD_PARTY_LIBRARIES} DESTINATION .)
  install(FILES res/license.rtf DESTINATION .)
else()
  install(FILES res/license.txt DESTINATION .)
  install(FILES
    /opt/llvm/lib/libc++.so
    /opt/llvm/lib/libc++.so.1
    /opt/llvm/lib/libc++.so.1.0
    /opt/llvm/lib/libc++abi.so
    /opt/llvm/lib/libc++abi.so.1
    /opt/llvm/lib/libc++abi.so.1.0
    /opt/llvm/lib/libgomp.so
    /opt/llvm/lib/libiomp5.so
    /opt/llvm/lib/libomp.so
    /opt/llvm/lib/libomptarget.rtl.x86_64.so
    /opt/llvm/lib/libomptarget.so
    /opt/llvm/lib/libtbb.so
    /opt/llvm/lib/libtbb.so.2
    /opt/llvm/lib/libtbbmalloc.so
    /opt/llvm/lib/libtbbmalloc.so.2
    /opt/llvm/lib/libtbbmalloc_proxy.so
    /opt/llvm/lib/libtbbmalloc_proxy.so.2
    /opt/llvm/lib/libunwind.so
    /opt/llvm/lib/libunwind.so.1
    /opt/llvm/lib/libunwind.so.1.0
    DESTINATION lib)
endif()

set(CPACK_PACKAGE_VENDOR "${PROJECT_VENDOR}")
set(CPACK_PACKAGE_DESCRIPTION "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_EXECUTABLES "${PROJECT_NAME}" "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
set(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}")
string(UUID PROJECT_GUID TYPE SHA1
  NAMESPACE 6BA7B814-9DAD-11D1-80B4-00C04FD430C8
  NAME "CN=${PROJECT_NAME}, O=${PROJECT_VENDOR}")
if(WIN32)
  set(CPACK_GENERATOR "WIX")
  set(CPACK_WIX_UPGRADE_GUID "${PROJECT_GUID}")
  set(CPACK_PACKAGE_NAME "${PROJECT_DESCRIPTION}")
  set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_DESCRIPTION}")
  set(CPACK_WIX_LICENSE_RTF "${CMAKE_CURRENT_SOURCE_DIR}/res/license.rtf")
  set(CPACK_WIX_PRODUCT_ICON "${CMAKE_CURRENT_SOURCE_DIR}/src/main.ico")
  set(CPACK_WIX_UI_BANNER "${CMAKE_CURRENT_SOURCE_DIR}/res/bitmap/banner.bmp")
  set(CPACK_WIX_UI_DIALOG "${CMAKE_CURRENT_SOURCE_DIR}/res/bitmap/dialog.bmp")
  set(CPACK_WIX_PROGRAM_MENU_FOLDER ".")
else()
  set(CPACK_GENERATOR "DEB")
  set(CPACK_PACKAGING_INSTALL_PREFIX "/opt/${PROJECT_NAME}")
  set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/res/license.txt")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_VENDOR}")
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.27)")
  set(CPACK_DEBIAN_PACKAGE_SECTION "misc")
endif()
include(CPack)
