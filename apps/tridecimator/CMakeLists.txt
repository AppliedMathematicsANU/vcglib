
add_library(vcg_tridecimator tridecimator.cpp ../../wrap/ply/plylib.cpp)
add_executable(tridecimator main.cpp)
target_link_libraries(tridecimator vcg_tridecimator)
install(TARGETS tridecimator RUNTIME DESTINATION bin)
install(TARGETS vcg_tridecimator LIBRARY DESTINATION lib)

ADD_LIBRARY(bp_tridecimator MODULE tridecimatorpy.cpp )
SET_TARGET_PROPERTIES(bp_tridecimator PROPERTIES PREFIX "" )
SET_TARGET_PROPERTIES(bp_tridecimator PROPERTIES OUTPUT_NAME _tridecimator )
TARGET_LINK_LIBRARIES(
  bp_tridecimator
  vcg_tridecimator
  ${BOOST_LIBRARIES}
  ${VCGLIB_BOOST_PYTHON_LIBRARIES}
  ${PYTHON_LIBRARIES}
)
SET_TARGET_PROPERTIES(bp_tridecimator PROPERTIES INSTALL_NAME_DIR ${VCGLIB_PYTHON_SITE_PACKAGES_INSTALL_DIR}/apps/tridecimator )
INSTALL(TARGETS bp_tridecimator DESTINATION ${VCGLIB_PYTHON_SITE_PACKAGES_INSTALL_DIR}/apps/tridecimator)
