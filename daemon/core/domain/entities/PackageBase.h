#pragma once

#include "core/domain/entities/AggregateRoot.h"
#include "core/domain/entities/Architecture.h"
#include "core/domain/entities/Branch.h"
#include "core/domain/entities/Repo.h"

#include "core/domain/value_objects/Name.h"
#include "core/domain/value_objects/PackageArchitecture.h"
#include "core/domain/value_objects/PackageVersion.h"

#include <string>

namespace bxt::Core::Domain
{
class PackageBase : public AggregateRoot<>
{
public:
    struct Section
    {
        Branch m_branch;
        Repo m_repo;
        Architecture m_arch;
    };
    const std::string& name() const { return m_name; }
    const PackageVersion& version() const  { return m_version; }
    const std::string& architecture() const { return m_architecture; }

    PackageBase(const Section& section,
                const std::string& name,
                const PackageVersion& version,
                const PackageArchitecture& arch)
        : m_section(section), m_name(name), m_version(version), m_architecture(arch)
    {}

    virtual ~PackageBase() = default;

    std::string string() const
    {
        return fmt::format("{}-{}-{}", name(), version().string(), architecture());
    }

protected:
    void set_name(const std::string& new_name) { m_name = new_name; }

    void set_version(const PackageVersion& new_version) { m_version = new_version; }

    void set_architecture(const std::string& new_architecture)
    {
        m_architecture = new_architecture;
    }

private:
    Section m_section;

    Name m_name;
    PackageVersion m_version;
    PackageArchitecture m_architecture;
};

} // namespace bxt::Core::Domain
