
add_library(vcg_tridecimator tridecimator.cpp ../../wrap/ply/plylib.cpp)
add_executable(tridecimator main.cpp)
target_link_libraries(tridecimator vcg_tridecimator)
install(TARGETS tridecimator RUNTIME DESTINATION bin)
install(TARGETS vcg_tridecimator RUNTIME DESTINATION lib)
