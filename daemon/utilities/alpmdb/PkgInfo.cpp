/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "PkgInfo.h"

#include <algorithm>
#include <boost/algorithm/string/regex.hpp>
#include <iostream>
#include <regex>
#include <sstream>

namespace bxt::Utilities::AlpmDb {

void PkgInfo::parse(const std::string &contents) {
    std::istringstream stream(contents);
    std::string line;
    while (std::getline(stream, line)) {
        if (line.starts_with("#")) { continue; }

        std::vector<std::string> result;
        boost::algorithm::split_regex(result, line, boost::regex(" = "));

        if (result.size() != 2) { continue; }

        m_values.insert({result[0], result[1]});
    }
}

std::set<std::string> PkgInfo::values(const std::string &key) {
    std::set<std::string> result;
    auto iterator_range = m_values.equal_range(key);

    static auto select2nd = []<typename TValue>(const TValue &val) {
        return val.second;
    };

    std::transform(iterator_range.first, iterator_range.second,
                   std::inserter(result, result.end()), select2nd);

    return result;
}

} // namespace bxt::Utilities::AlpmDb
