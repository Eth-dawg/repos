#pragma once

#include <string>
#include <functional>

struct IDirect3DDevice9;

namespace camelotpad::render {

class Dx9LuaUiBridge {
public:
    using DrawCallback = std::function<void()>;

    bool Initialize();
    void Shutdown();

    void TogglePanel();
    void SetDrawCallback(DrawCallback callback);
    void OnEndScene(IDirect3DDevice9* device);

private:
    bool initialized_ {false};
    bool showPanel_ {true};
    std::string loadedLuaPath_ {};
    DrawCallback drawCallback_ {};
};

} // namespace camelotpad::render
