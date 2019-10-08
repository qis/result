file(GLOB_RECURSE sources
  src/*.hpp src/*.h
  src/*.cpp src/*.c)
foreach(source ${sources})
  message(${source})
endforeach()
