/**
 *            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *                    Version 2, December 2004
 *
 * Copyright (C) 2022 R4nd0m6uy <r4nd0m6uy@r4nd0m6uy.ch>
 *
 * Everyone is permitted to copy and distribute verbatim or modified
 * copies of this license document, and changing it is allowed as long
 * as the name is changed.
 *
 *            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 *
 *  0. You just DO WHAT THE FUCK YOU WANT TO.
 */

#include <getopt.h>

#include <iostream>

#include "MainLoop.h"

struct CmdLineArgs {
    spdlog::level::level_enum logLevel = spdlog::level::info;
};

enum class ParseStatus {
    SUCCESS,
    DO_NOT_START,
    FAILURE
};

void usage(const char* appName) {
    std::cout << appName << ":\n";
    std::cout << "-h | --help       Show this message and exit\n";
    std::cout << "-l | --log-level  Log level (0:trace 6:off)\n";
}

ParseStatus parseCommandLine(int argc, char *argv[], CmdLineArgs &cmdLineArgs) {
    int opt;
    int logLevel;
    static struct option long_options[] = {
            {"help",      no_argument,       0, 'h'},
            {"log-level", required_argument, 0, 'l'},
            {0, 0,                           0, 0}
    };

    while ((opt = getopt_long(argc, argv, "hl:", long_options, 0)) != -1) {
        switch (opt) {
            case 'h':
                usage(argv[0]);
                return ParseStatus::DO_NOT_START;
            case 'l':
                logLevel = atoi(optarg);
                if (logLevel >= spdlog::level::n_levels || logLevel < 0) {
                    std::cerr << "Invalid log level " << logLevel << "\n";
                    return ParseStatus::FAILURE;
                }
                cmdLineArgs.logLevel = static_cast<spdlog::level::level_enum>(logLevel);
                break;
            default:
                usage(argv[0]);
                return ParseStatus::FAILURE;
        }
    }

    return ParseStatus::SUCCESS;
}

int main(int argc, char* argv[]) {
    DearQOI::MainWindow mainWindow;
    int status;
    CmdLineArgs cmdLineArgs;

    switch (parseCommandLine(argc, argv, cmdLineArgs)) {
        case ParseStatus::DO_NOT_START:
            return 0;
        case ParseStatus::FAILURE:
            return -1;
        case ParseStatus::SUCCESS:
            break;
    }

    DearQOI::Logger::Init(cmdLineArgs.logLevel);
    status = mainWindow.init();
    if (status) {
        return status;
    }

    status = DearQOI::MainLoop::run(mainWindow);

    DearQOI::Logger::Main()->info("Exiting with code {}", status);
    return status;
}
