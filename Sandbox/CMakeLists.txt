project(Sandbox)

# Using file GLOB to collect all source files
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.h")

add_executable(Sandbox ${SOURCE_FILES})

target_include_directories(Sandbox PRIVATE
  ${PROJECT_SOURCE_DIR}/../Usagi/src
  ${PROJECT_SOURCE_DIR}/../Usagi/vendor/spdlog/include
)

target_link_libraries(Sandbox PRIVATE Usagi)

if(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(Sandbox PRIVATE -Wall -Wextra -pedantic)
endif()

target_compile_definitions(Sandbox PUBLIC "$<$<CONFIG:DEBUG>:USG_DEBUG>")
target_compile_definitions(Sandbox PUBLIC "$<$<CONFIG:RELEASE>:USG_RELEASE>")
target_compile_definitions(Sandbox PUBLIC "$<$<CONFIG:DIST>:USG_DIST>")
