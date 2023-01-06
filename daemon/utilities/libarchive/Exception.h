/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include <archive.h>
#include <exception>
#include <string>

namespace Archive {
class Exception : public std::exception {
public:
    explicit Exception(const std::string& message) : m_message(message) {}

private:
    std::string m_message;

    // exception interface
public:
    virtual const char* what() const noexcept override {
        return m_message.c_str();
    }
};

class InvalidEntry : public Exception {
public:
    explicit InvalidEntry() : Exception("The entry has no linked archive!") {}
};

class LibException : public Exception {
public:
    explicit LibException(int status, const std::string& message)
        : Exception(message), m_status(status) {}

private:
    int m_status = ARCHIVE_OK;
};

} // namespace Archive
