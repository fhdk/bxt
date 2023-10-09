/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2023 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#pragma once

#include "core/application/dtos/PackageDTO.h"
#include "core/application/dtos/PackageSectionDTO.h"
#include "utilities/alpmdb/Desc.h"
#include "utilities/box/AlpmDbArchiver.h"
#include "utilities/box/Package.h"
#include "utilities/box/PoolManager.h"
#include "utilities/errors/DatabaseError.h"
#include "utilities/errors/Macro.h"
#include "utilities/lmdb/Database.h"
#include "utilities/lmdb/Environment.h"

#include <filesystem>
#include <memory>
#include <string_view>
namespace bxt::Box {

class Database {
public:
    BXT_DECLARE_RESULT(DatabaseError)

    Database(std::shared_ptr<Utilities::LMDB::Environment> env,
             std::shared_ptr<coro::io_scheduler> scheduler,
             std::vector<Core::Application::PackageSectionDTO> sections,
             const std::string_view name)
        : m_root_path("./box/"),
          m_manager(m_root_path / "pool"),
          m_db(env, name),
          m_archiver(scheduler, sections, m_root_path, *this) {}

    static std::string serialize_key(PackageSectionDTO section,
                                     std::string name);

    static std::optional<std::pair<PackageSectionDTO, std::string>>
        deserialize_key(const std::string_view key);

    coro::task<Result<void>> add(const PackageSectionDTO section,
                                 const Package package);

    coro::task<Result<void>> remove(const PackageSectionDTO section,
                                    const std::string name);

    coro::task<Result<std::vector<Package>>>
        find_by_section(PackageSectionDTO section);

    Utilities::LMDB::Database<bxt::Box::Package>& lmdb_handle() { return m_db; }

private:
    std::filesystem::path m_root_path;
    PoolManager m_manager;
    Utilities::LMDB::Database<bxt::Box::Package> m_db;
    AlpmDbArchiver m_archiver;
};

} // namespace bxt::Box