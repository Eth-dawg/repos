#pragma once

#include <cstdint>
#include <optional>
#include <string_view>

namespace camelotpad::integrations {

struct DaocEntitySnapshot {
    std::uint32_t entityId {0};
    bool isHostile {true};
    float distanceMeters {0.0F};
};

class IDaocMemoryBridge {
public:
    virtual ~IDaocMemoryBridge() = default;

    virtual bool IsReady() const = 0;
    virtual std::optional<DaocEntitySnapshot> GetReticleEntity() const = 0;
    virtual bool FaceTarget(std::uint32_t entityId) = 0;
};

class IDaocCommandBridge {
public:
    virtual ~IDaocCommandBridge() = default;

    virtual bool IsReady() const = 0;
    virtual bool ExecuteSlash(std::string_view command) = 0;
};

class DaochookInterfaceSet {
public:
    DaochookInterfaceSet(IDaocMemoryBridge* memoryBridge, IDaocCommandBridge* commandBridge);

    bool IsUsable() const;
    IDaocMemoryBridge* Memory() const;
    IDaocCommandBridge* Command() const;

private:
    IDaocMemoryBridge* memoryBridge_ {nullptr};
    IDaocCommandBridge* commandBridge_ {nullptr};
};

class NullDaocMemoryBridge final : public IDaocMemoryBridge {
public:
    bool IsReady() const override;
    std::optional<DaocEntitySnapshot> GetReticleEntity() const override;
    bool FaceTarget(std::uint32_t entityId) override;
};

class NullDaocCommandBridge final : public IDaocCommandBridge {
public:
    bool IsReady() const override;
    bool ExecuteSlash(std::string_view command) override;
};

} // namespace camelotpad::integrations
