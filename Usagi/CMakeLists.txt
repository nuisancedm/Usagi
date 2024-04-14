project(Usagi)

# Using file GLOB to collect all source files
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.h")

add_library(Usagi SHARED ${SOURCE_FILES})

target_include_directories(Usagi PUBLIC
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/vendor/spdlog/include
)

target_compile_definitions(Usagi PRIVATE
  USAGI_PLATFORM_WINDOWS
  USAGI_BUILD_DLL
)

if(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(Usagi PRIVATE -Wall -Wextra -pedantic)
endif()

target_compile_definitions(Usagi PUBLIC "$<$<CONFIG:DEBUG>:USG_DEBUG>")
target_compile_definitions(Usagi PUBLIC "$<$<CONFIG:RELEASE>:USG_RELEASE>")
target_compile_definitions(Usagi PUBLIC "$<$<CONFIG:DIST>:USG_DIST>")
