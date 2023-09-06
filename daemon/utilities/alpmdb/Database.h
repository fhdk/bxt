/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once
#include "tl/expected.hpp"
#include "utilities/Error.h"
#include "utilities/alpmdb/Desc.h"

#include <boost/log/trivial.hpp>
#include <coro/event.hpp>
#include <coro/io_scheduler.hpp>
#include <coro/mutex.hpp>
#include <coro/sync_wait.hpp>
#include <coro/task.hpp>
#include <filesystem>
#include <fmt/format.h>
#include <frozen/map.h>
#include <frozen/set.h>
#include <frozen/string.h>
#include <iterator>
#include <memory>
#include <parallel_hashmap/phmap.h>
#include <string>
#include <string_view>

namespace bxt::Utilities::AlpmDb {

class DatabaseError : public bxt::Error {
public:
    enum class ErrorType {
        IOError,
        DatabaseMalformedError,
        InvalidPackageError
    };
    DatabaseError(ErrorType error_type) : error_type(error_type) {}

    DatabaseError(ErrorType error_type, const bxt::Error&& source)
        : bxt::Error(std::make_unique<bxt::Error>(std::move(source))),
          error_type(error_type) {}

    static inline frozen::map<ErrorType, std::string_view, 3> messages {
        {ErrorType::IOError, "IO error"},
        {ErrorType::DatabaseMalformedError, "Database is malformed"},
        {ErrorType::InvalidPackageError, "Invalid package"}};

    const std::string message() const noexcept override {
        return messages.at(error_type).data();
    }

private:
    ErrorType error_type;
};

class Database {
public:
    BXT_DECLARE_RESULT(DatabaseError)

    explicit Database(const std::filesystem::path& path = "./",
                      const std::string& name = "")
        : m_path(std::filesystem::absolute(path)), m_name(name) {
        if (name == "") { m_name = m_path.parent_path().filename(); }
        const auto load_result = coro::sync_wait(load());

        if (!load_result.has_value()) {
            BOOST_LOG_TRIVIAL(warning)
                << fmt::format("Database '{}' is not readable or doesn't "
                               "exist. Error is {}",
                               m_path.string(), load_result.error().what());
        }
    }

    coro::task<Result<void>> add(const std::set<std::string>& files);
    coro::task<Result<void>> remove(const std::set<std::string>& packages);
    std::set<std::string> packages() const {
        std::set<std::string> packages;
        std::ranges::transform(m_descriptions,
                               std::inserter(packages, packages.begin()),
                               [](const auto& value) { return value.first; });

        return packages;
    }

    Result<std::vector<std::string>>
        description_values(const std::string& key) const {
        std::vector<std::string> values;
        values.reserve(m_descriptions.size());

        for (const auto& [current_key, value] : m_descriptions) {
            const auto result = value.get(key);

            if (!result.has_value()) { continue; }

            values.push_back(*result);
        }

        return values;
    }

    coro::task<Result<void>> load();

private:
    coro::task<Result<void>> save_async();

    bool package_exists(const std::string& package_name) const;
    void create_symlinks() const;

    constexpr static frozen::set<frozen::string, 3>
        supported_package_extensions = {"pkg.tar.gz", "pkg.tar.xz",
                                        "pkg.tar.zst"};

    std::filesystem::path m_path;
    std::unique_ptr<coro::mutex> m_file_lock = std::make_unique<coro::mutex>();

    std::string m_name;
    phmap::parallel_node_hash_map<std::string, Desc> m_descriptions;
};
} // namespace bxt::Utilities::AlpmDb
