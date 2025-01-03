#include <iostream>

#include <string>
#include <vector>
#include <stdexcept>

#include "pda/context.h"
#include "pda/parser.h"
#include "pda/emulator.h"

#include "tm/parser.h"
#include "tm/emulator.h"

#include "utils/exception.h"

// 假设 PDAHandler 和 TMHandler 的接口
void PDAHandler(const std::string& pdaFile, const std::string& inputStr, bool verbose) {
    PDAContext context = PDAParser::parse(pdaFile);
    PDAEmulator emulator(context);
    emulator.setVerboseMode(verbose);
    bool result = emulator.run(inputStr);
    if (!verbose) {
        std::cout << (result ? "true" : "false") << std::endl;
    }
}

void TMHandler(const std::string& tmFile, const std::string& inputStr, bool verbose) {
    TMContext context = TMParser::parse(tmFile);
    TMEmulator emulator(context);
    emulator.setVerboseMode(verbose);
    auto result = emulator.run(inputStr);
    if (!verbose) {
        std::cout << result << std::endl;
    }
}

// 检查字符串是否以指定后缀结尾
bool endsWith(const std::string& str, const std::string& suffix) {
    if (str.size() >= suffix.size()) {
        return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    }
    return false;
}

// 打印帮助信息
void printHelp() {
    std::cout << "usage: fla [-v|--verbose] [-h|--help] <pda> <input>\n"
                 "       fla [-v|--verbose] [-h|--help] <tm> <input>\n"
                 "\noptions:\n"
                 "  -v, --verbose          Enable verbose mode\n"
                 "  -h, --help             Print usage\n";
}

// 解析命令行参数
void parseArguments(int argc, char* argv[], 
                    std::string& automataFile, 
                    std::string& inputStr, 
                    bool& verbose, 
                    bool& showHelp) {
    verbose = false;
    showHelp = false;
    std::vector<std::string> positionalArgs;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            showHelp = true;
        } else if (arg == "-v" || arg == "--verbose") {
            verbose = true;
        } else {
            positionalArgs.push_back(arg);
        }
    }

    if (showHelp) {
        return; // 如果请求帮助，则无需检查其他参数
    }

    if (positionalArgs.size() < 2) {
        throw std::invalid_argument("Both automata file and input file are required!");
    }

    automataFile = positionalArgs[0];
    inputStr = positionalArgs[1];
}

int main(int argc, char* argv[]) {
    std::string automataFile;
    std::string inputStr;
    bool verbose = false;
    bool showHelp = false;
    
    try {

        // 解析命令行参数
        parseArguments(argc, argv, automataFile, inputStr, verbose, showHelp);

        // 如果是帮助请求，打印帮助信息并退出
        if (showHelp) {
            printHelp();
            return 0;
        }

        // 判断文件类型并调用对应 Handler
        if (endsWith(automataFile, ".pda")) {
            PDAHandler(automataFile, inputStr, verbose);
        } else if (endsWith(automataFile, ".tm")) {
            TMHandler(automataFile, inputStr, verbose);
        } else {
            throw std::invalid_argument("Unsupported automata type. File must have extension .pda or .tm");
        }

    } catch (const InputSyntaxError& e) {
        // 打印错误信息并退出
        if (!verbose) {
            std::cerr << "illegal input" << std::endl;
        }
        return 1;
    } catch (const AutomataSyntaxException& e) {
        // 打印错误信息并退出
        std::cerr << "syntax error" << std::endl;
        if (verbose) {
            std::cerr << e.what() << std::endl;
        }
        return 1;
    } catch (const AutomataStructureException& e) {
        // 打印错误信息并退出
        // XXX: maybe we should print structure or sementic error, but the requirement is not clear
        std::cerr << "syntax error" << std::endl;
        if (verbose) {
            std::cerr << e.what() << std::endl;
        }
        return 1;
    } catch (const std::exception& e) {
        // 打印错误信息并退出
        std::cerr << "Error: " << e.what() << std::endl;
        printHelp();
        return 1;
    }

    return 0;
}