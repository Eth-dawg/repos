#include "camelotpad/render/Dx9LuaUiBridge.hpp"

#include <filesystem>
#include <utility>

namespace camelotpad::render {

bool Dx9LuaUiBridge::Initialize() {
    if (initialized_) {
        return true;
    }

    const std::filesystem::path luaScript = std::filesystem::path("camelotpad-lua") / "ui" / "main.lua";
    if (!std::filesystem::exists(luaScript)) {
        return false;
    }

    loadedLuaPath_ = luaScript.string();
    initialized_ = true;
    return true;
}

void Dx9LuaUiBridge::Shutdown() {
    initialized_ = false;
    loadedLuaPath_.clear();
}

void Dx9LuaUiBridge::TogglePanel() {
    showPanel_ = !showPanel_;
}

void Dx9LuaUiBridge::SetDrawCallback(DrawCallback callback) {
    drawCallback_ = std::move(callback);
}

void Dx9LuaUiBridge::OnEndScene(IDirect3DDevice9* device) {
    (void)device;
    if (!initialized_ || !showPanel_) {
        return;
    }

    // Bridge execution order for the DX9 render hook path:
    // 1) Start Dear ImGui frame for the hooked device.
    // 2) Execute Lua-backed draw callback (main.draw).
    // 3) Submit ImGui draw data to DX9.
    if (drawCallback_) {
        drawCallback_();
    }
}

} // namespace camelotpad::render
