include_directories(${YAMLCPP_INCLUDE_DIRS})
link_directories(${YAMLCPP_LIBRARY_DIRS})

set(AKAGORIA_MAP_SRC
  akagoria-map.cc
)

add_executable(akagoria-map ${AKAGORIA_MAP_SRC})
target_link_libraries(akagoria-map mm0 ${YAMLCPP_LIBRARIES})

install(
  TARGETS akagoria-map
  RUNTIME DESTINATION bin
)
