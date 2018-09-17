#
#include_directories("${CMAKE_BINARY_DIR}/include")
if (UNIX)
  message(STATUS "Build for UNIX")
  set(INVADER_LIBS
    allegro allegro_image allegro_primitives
    #allegro_dialog
    allegro_font
    allegro_ttf
    allegro_audio
    allegro_acodec
    )

endif(UNIX)

if (MINGW)
  message(STATUS "Build for MinGW")
  # The path stuff is a terrible hack. Please fix me!!
  set(ALLEGRO_BASE_DIR "/home/ksomervi/src/allegro-mingw-gcc6.2.0-x64-static-5.2.2/allegro")
  message(STATUS "ALLEGRO_BASE_DIR: ${ALLEGRO_BASE_DIR}")
  set(ALLEGRO_INCLUDE_DIR "${ALLEGRO_BASE_DIR}/include")
  set(ALLEGRO_LIB_DIR "${ALLEGRO_BASE_DIR}/lib")

  include_directories(SYSTEM ${ALLEGRO_INCLUDE_DIR})

  add_compile_options("-I${ALLEGRO_INCLUDE_DIR}")

  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${ALLEGRO_INCLUDE_DIR}")

  message(STATUS "ALLEGRO_INCLUDE_DIR: ${ALLEGRO_INCLUDE_DIR}")

  #LDFLAGS=-L$(ALLEGRO_PATH)/lib

  #target_compile
  #LDFLAGS +=-lzlibstatic
  #LDFLAGS +=-lmsvcrt
  set(INVADER_LIBS
    #allegro_monolith-debug-static
    allegro-debug-static
    allegro_image-debug-static
    allegro_primitives-debug-static
    #allegro_dialog
    allegro_font-debug-static
    allegro_ttf-debug-static
    allegro_audio-debug-static
    allegro_acodec-debug-static
    jpeg libdumb libFLAC libfreetype
    libvorbisfile libvorbis libogg opus opusfile
    libphysfs libpng16 libz dsound gdiplus
    uuid kernel32 winmm psapi opengl32 glu32
    user32 comdlg32 gdi32 shell32 ole32
    advapi32 ws2_32 shlwapi)

  message(STATUS "INVADER_LIBS: ${INVADER_LIBS}")

  find_library(ALLEGRO_LIB NAMES allegro_monolith-debug-static
    allegro_monolith-debug-static.a liballegro_monolith-debug-static.a
    HINTS ${ALLEGRO_LIB_DIR} ${ALLEGRO_BASE_DIR}
    NO_DEFAULT_PATH
    #NO_SYSTEM_ENVIRONMENT_PATH
  )

  message(STATUS "ALLEGRO_LIB: ${ALLEGRO_LIB}")

  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L${ALLEGRO_LIB_DIR}")
  #target_link_libraries(invader -L${ALLEGRO_LIB_DIR})
endif(MINGW)

add_executable(invader
  main.cpp game.cpp base_level.cpp level_1.cpp basic_object.cpp fighter.cpp
  point_2d.cpp
  controller.cpp
  logger.cpp
  )

target_link_libraries(invader ${INVADER_LIBS})
if(MINGW)
  target_link_libraries(invader -static-libstdc++ -static-libgcc)
endif(MINGW)

#message(STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")
