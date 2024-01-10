cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "0" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(TNN VERSION ${PROJECT_VESRION})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(TNN TNN.cpp)
add_library(reservoir_sampling reservoir_sampling.cpp)
add_executable(testTNN testTNN.cpp)

set_target_properties(TNN testTNN PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(TNN
    PRIVATE "${CMAKE_BINARY_DIR}"
)
set_target_properties(testTNN PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_link_libraries(reservoir_sampling PRIVATE
    reservoir_sampling
)
target_link_libraries(testTNN
    ${Boost_LIBRARIES}
#    helloworld
)

if (MSVC)
    target_compile_options(TNN PRIVATE
        /W4
    )
   target_compile_options(reservoir_sampling PRIVATE
       /W4
   )
    target_compile_options(testTNN PRIVATE
        /W4
    )
else ()
    target_compile_options(TNN PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
   target_compile_options(reservoir_sampling PRIVATE
       -Wall -Wextra -pedantic -Werror
   )
    target_compile_options(testTNN PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()



install(TARGETS TNN RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

add_test(test_version test_version)
