/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>

void setup_logger() {
    using namespace boost::log;

    add_common_attributes();
    add_console_log(std::cout,
                    boost::log::keywords::format =
                        "[%TimeStamp%][%Severity%]: %Message%",
                    boost::log::keywords::auto_flush = true);
    add_file_log("journal.log",
                 boost::log::keywords::format =
                     "[%TimeStamp%][%Severity%]: %Message%",
                 boost::log::keywords::auto_flush = true);
}

int main() {
    setup_logger();
    return 0;
}
