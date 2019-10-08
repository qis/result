file(READ CMakeLists.txt SRC)
string(REGEX REPLACE ".*project *\\( *([^ ]+) .*" "\\1" DST ${SRC})
message(${DST})
