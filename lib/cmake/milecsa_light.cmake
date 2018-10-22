add_library(milecsa_light STATIC IMPORTED)


find_library(milecsa_light_LIBRARY_PATH milecsa_light HINTS "${CMAKE_CURRENT_LIST_DIR}/../../")
set_target_properties(milecsa_light PROPERTIES IMPORTED_LOCATION "${milecsa_light_LIBRARY_PATH}")

include_directories(
        "${milecsa_light_INCLUDE_PATH}"
)

message(STATUS "CMAKE_CURRENT_LIST_DIR "  ${CMAKE_CURRENT_LIST_DIR})
message(STATUS "CMAKE_INSTALL_PREFIX " ${CMAKE_INSTALL_PREFIX})
message(STATUS "milecsa_light_LIBRARY_PATH " ${milecsa_light_LIBRARY_PATH})
message(STATUS "milecsa_light_INCLUDE_PATH " ${milecsa_light_INCLUDE_PATH})
