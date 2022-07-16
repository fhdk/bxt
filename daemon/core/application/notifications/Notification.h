/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include <string>

namespace bxt::Core::Application {
class Notification {
public:
    virtual ~Notification() = default;

    const std::string &title() const { return m_title; }
    void set_title(const std::string &new_title) { m_title = new_title; }

    const std::string &content() const { return m_content; }
    void set_content(const std::string &new_content) {
        m_content = new_content;
    }
    virtual bool valid() const { return !m_title.empty(); }

private:
    std::string m_title;
    std::string m_content;
};

} // namespace bxt::Core::Application
