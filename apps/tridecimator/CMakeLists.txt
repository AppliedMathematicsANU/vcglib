
add_executable(tridecimator tridecimator.cpp ../../wrap/ply/plylib.cpp)
install(TARGETS tridecimator RUNTIME DESTINATION bin)
