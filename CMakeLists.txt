cmake_minimum_required (VERSION 3.8 FATAL_ERROR)

project (ListLib)

set (DEBUG_FLAGS -DNDEBUG -ggdb3 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wno-unused-parameter -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fPIC -fno-omit-frame-pointer -pie -fPIE -Werror=vla -Wno-write-strings -fsanitize=address,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr)
set (RELEASE_FLAGS -std=c++17 -O3)

add_library (${PROJECT_NAME} INTERFACE)
target_include_directories (${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# -----------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------- Gtests configuration ----------------------------------------------------
# -----------------------------------------------------------------------------------------------------------------------------

enable_testing ()
add_executable (MainTest mainTest.cpp)
target_include_directories (MainTest PRIVATE ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

target_compile_definitions (MainTest PRIVATE _SHOW_STACK_TRACE)

target_compile_options (MainTest PRIVATE $<$<CONFIG:Debug>:${DEBUG_FLAGS}>)
target_compile_options (MainTest PRIVATE $<$<CONFIG:Release>:${RELEASE_FLAGS}>)

target_link_options (MainTest PRIVATE $<$<CONFIG:Debug>:${DEBUG_FLAGS}>)
target_link_options (MainTest PRIVATE $<$<CONFIG:Release>:${RELEASE_FLAGS}>)

target_link_libraries (MainTest PRIVATE ${PROJECT_NAME})
target_link_libraries (MainTest PRIVATE gtest gtest_main)

include (GoogleTest)
gtest_discover_tests (MainTest)
