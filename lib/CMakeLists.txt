project(${PROJECT_LIB} VERSION 1.0.0)

set(CMAKE_INSTALL_PREFIX /usr/local/${PROJECT_LIB})

include_directories (
        ../include
        ../src
        ../src/transactions_wrapper
        ../vendor/nlohmann/
        ../vendor/mile-crypto/include
        ../vendor/mile-crypto/src/ed25519/include
)

FILE(GLOB PUBLIC_INCLUDE_FILES
        ../include/*.hpp
        )

FILE(GLOB INCLUDE_FILES
        ../vendor/nlohmann/*.hpp
        ../vendor/mile-crypto/src/ed25519/include/*.c
        ../vendor/mile-crypto/src/private_include/*.h
        ../vendor/mile-crypto/include/*.h
        ${PUBLIC_INCLUDE_FILES}
        )

FILE(GLOB SOURCES
        ../src/*.cpp
        ../src/transactions_wrapper/*.cpp
        ../vendor/mile-crypto/src/ed25519/src/*.c
        ../vendor/mile-crypto/src/*.c
        ../vendor/mile-crypto/src/*.cpp
        )

set (BOOST_COMPONENTS system)

find_package (Boost  REQUIRED COMPONENTS ${BOOST_COMPONENTS})
include_directories(${Boost_INCLUDE_DIRS})

if (MSVC)
    add_definitions (/W4)
elseif (CMAKE_COMPILER_IS_GNUCXX)
    add_definitions (-Wall -pedantic)
else ()
    message ("Unknown compiler")
endif ()

add_library(
        ${PROJECT_LIB}
        ${SOURCES})

target_link_libraries (
        ${PROJECT_LIB}  PUBLIC
        ${Boost_LIBRARIES}
)

target_include_directories(
        ${PROJECT_LIB}
        PUBLIC
        ../include
)

set(config_install_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

# Configuration
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(TARGETS_EXPORT_NAME "${PROJECT_NAME}")
set(namespace "${PROJECT_NAME}::")

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
        "${version_config}" COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
        "cmake/Config.cmake.in"
        "${project_config}"
        INSTALL_DESTINATION "${config_install_dir}"
)

install(TARGETS ${PROJECT_LIB}          DESTINATION lib)
install(FILES   ${PUBLIC_INCLUDE_FILES} DESTINATION include)
install(FILES   ${project_config} ${version_config} DESTINATION lib/cmake/${PROJECT_LIB})
install(FILES   cmake/${PROJECT_LIB}.cmake DESTINATION lib/cmake/${PROJECT_LIB})

#install(FILES "${project_config}" "${version_config}"  DESTINATION "${config_install_dir}")

