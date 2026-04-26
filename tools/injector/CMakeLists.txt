add_executable(camelotpad_injector
    injector_main.cpp
)

target_compile_definitions(camelotpad_injector
    PRIVATE
        NOMINMAX
        WIN32_LEAN_AND_MEAN
)

target_compile_options(camelotpad_injector
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
)
