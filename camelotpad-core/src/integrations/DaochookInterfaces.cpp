#include "camelotpad/integrations/DaochookInterfaces.hpp"

namespace camelotpad::integrations {

DaochookInterfaceSet::DaochookInterfaceSet(IDaocMemoryBridge* memoryBridge, IDaocCommandBridge* commandBridge)
    : memoryBridge_(memoryBridge), commandBridge_(commandBridge) {}

bool DaochookInterfaceSet::IsUsable() const {
    return memoryBridge_ != nullptr && commandBridge_ != nullptr && memoryBridge_->IsReady() && commandBridge_->IsReady();
}

IDaocMemoryBridge* DaochookInterfaceSet::Memory() const {
    return memoryBridge_;
}

IDaocCommandBridge* DaochookInterfaceSet::Command() const {
    return commandBridge_;
}

bool NullDaocMemoryBridge::IsReady() const {
    return true;
}

std::optional<DaocEntitySnapshot> NullDaocMemoryBridge::GetReticleEntity() const {
    return std::nullopt;
}

bool NullDaocMemoryBridge::FaceTarget(std::uint32_t entityId) {
    (void)entityId;
    return false;
}

bool NullDaocCommandBridge::IsReady() const {
    return true;
}

bool NullDaocCommandBridge::ExecuteSlash(std::string_view command) {
    (void)command;
    return false;
}

} // namespace camelotpad::integrations
