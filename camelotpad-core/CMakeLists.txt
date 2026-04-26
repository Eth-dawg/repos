add_library(camelotpad_core SHARED
    src/DllMain.cpp
    src/CamelotPadApp.cpp
    src/input/XInputProcessor.cpp
    src/discovery/ValueDiscovery.cpp
    src/integrations/DaochookInterfaces.cpp
    src/render/Dx9LuaUiBridge.cpp
    src/actions/ActionRouter.cpp
    src/config/ProfileManager.cpp
    src/platform/WinInputEmitter.cpp
)

target_include_directories(camelotpad_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(camelotpad_core
    PRIVATE
        NOMINMAX
        WIN32_LEAN_AND_MEAN
)

target_compile_options(camelotpad_core
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
)

target_link_libraries(camelotpad_core
    PRIVATE
        xinput
)
