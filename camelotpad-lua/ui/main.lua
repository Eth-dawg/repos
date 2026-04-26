local M = {}

M.state = {
  showConfig = false,
  profileName = "default",
  reticleEnabled = true,
  highContrast = true,
  radialMenu = false,
  cameraPreset = "tactical",
  interruptAlerts = true,
  showPerfOverlay = true
}

function M.toggle_config()
  M.state.showConfig = not M.state.showConfig
end

function M.draw()
  if not M.state.showConfig then
    return
  end

  if imgui.Begin("CamelotPad Config") then
    imgui.Text("Controller-first DAoC overlay")
    changed, M.state.profileName = imgui.InputText("Profile", M.state.profileName)
    changed, M.state.reticleEnabled = imgui.Checkbox("Enable Reticle", M.state.reticleEnabled)
    changed, M.state.highContrast = imgui.Checkbox("High Contrast Mode", M.state.highContrast)
    changed, M.state.radialMenu = imgui.Checkbox("Enable Radial Menu", M.state.radialMenu)
    changed, M.state.interruptAlerts = imgui.Checkbox("Interrupt Alerts", M.state.interruptAlerts)
    changed, M.state.showPerfOverlay = imgui.Checkbox("Show FPS/Ping Overlay", M.state.showPerfOverlay)

    if imgui.Button("Set Tactical Camera") then
      M.state.cameraPreset = "tactical"
    end
    imgui.SameLine()
    if imgui.Button("Set Free-Look Camera") then
      M.state.cameraPreset = "freelook"
    end
  end
  imgui.End()
end

return M
