/**
 * Implementation of the TMParser class.
 */

#include "tm/parser.h"

#include "utils/parse_token.h"
#include <fstream>
#include <sstream>

#include <algorithm>


std::set<std::string> TMParser::control_tokens = {"#Q", "#S", "#G", "#q0", "#B", "#N", "#F"};

std::regex TMParser::state_regex = std::regex("[a-zA-Z0-9_]+");

TMContext TMParser::parse(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }

    TMContext context;

    std::string line;
    int line_idx = 0;

    while (std::getline(file,line)) {
        linePreprocess(line);

        line_idx++;

        try {
            parseLine(line, context);
        } catch (AutomataSyntaxException& e) {
            e.setLine(line_idx, line);
            file.close();
            throw e;
        }
    }

    file.close();

    return context;
}

void TMParser::linePreprocess(std::string& line) {
    // 目的是去掉注释
    // 找到 ';' 在字符串中的位置
    size_t pos = line.find(';');

    if (pos != std::string::npos) {
        line = line.substr(0, pos);
    }

    // 去除末尾可能存在的多余空格
    line.erase(line.find_last_not_of(" \t") + 1);
}

void TMParser::parseLine(const std::string& line, TMContext& context) {

    std::vector<std::string> tokens;

    std::istringstream iss(line);
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }

    if (tokens.size() > 0) {
        if (line[0] == ' ') {
            throw AutomataSyntaxException(line, "starts with space but not empty");
        }
    } else {
        return;
    }

    char blank_default = '_';

    // 检查是否是控制符号
    if (control_tokens.find(tokens[0]) == control_tokens.end()) {
        // 无可用控制服，此时第一个token不应该以#开头
        if (tokens[0][0] == '#') {
            throw AutomataSyntaxException(tokens[0], "starts with '#' but not a valid control token.");
        }

        // TODO: 处理转移函数
    } else {
        if (tokens.size() != 3) {
            throw AutomataSyntaxException(line, "invalid token num");
        }

        if (tokens[1] != "=") {
            throw AutomataSyntaxException(line, "missing '='");
        }

        if (tokens[0] == "#Q") {
            // 状态集
            std::set<std::string> states = parseStrSet(tokens[2]);

            // 检查状态名是否合法
            for (auto state : states) {
                if (!std::regex_match(state, state_regex)) {
                    throw AutomataSyntaxException(state, "only [a-zA-Z0-9_]+ allowed");
                }
            }
            context.states = std::move(states);

        } else if (tokens[0] == "#S") {
            // 输入符号集
            std::set<char> symbols = parseCharSet(tokens[2]);

            // 检查输入符号是否合法
            for (auto symbol : symbols) {
                if (!isValidSymbol(symbol)) {
                    throw AutomataSyntaxException(std::to_string(symbol), "only printable ASCII characters allowed");
                }
            }

            context.input_alphabet = std::move(symbols);

        } else if (tokens[0] == "#G") {
            // 栈符号集
            std::set<char> symbols = parseCharSet(tokens[2]);

            // 检查栈符号是否合法
            for (auto symbol : symbols) {
                if (!(isValidSymbol(symbol) || (symbol == blank_default))) {
                    throw AutomataSyntaxException(std::to_string(symbol), "only printable ASCII characters allowed");
                }
            }

            context.tape_alphabet = std::move(symbols);

        } else if (tokens[0] == "#q0") {
            // 初始状态
            std::string start_state = tokens[2];

            // 检查初始状态是否合法
            if (!std::regex_match(start_state, state_regex)) {
                throw AutomataSyntaxException(start_state, "only [a-zA-Z0-9_]+ allowed");
            }

            context.start_state = std::move(start_state);

        } else if (tokens[0] == "#B") {
            // 空字符
            char blank = tokens[2][0];

            context.blank_char = blank;

            blank_default = blank;

        } else if (tokens[0] == "#F") {
            // 终止状态集
            std::set<std::string> final_states = parseStrSet(tokens[2]);

            // 检查终止状态集合是否合法
            for (auto state : final_states) {
                if (!std::regex_match(state, state_regex)) {
                    throw AutomataSyntaxException(state, "only [a-zA-Z0-9_]+ allowed");
                }
            }

            context.final_states = std::move(final_states);

        } else if (tokens[0] == "#N") {
            try {
                int number = std::stoi(tokens[2]);
                if (number < 1) {
                    throw AutomataSyntaxException(tokens[2], "require tape num > 0");
                }
            } catch (const std::invalid_argument &e) {
                throw AutomataSyntaxException(tokens[2], "not a valid number");
            } catch (const std::out_of_range &e) {
                throw AutomataSyntaxException(tokens[2], "number out of range");
            }

        }
    }
}

bool TMParser::isValidSymbol(char c) {
    // 检查是否是 ASCII 可显示字符
    if (c >= 32 && c <= 126) {
        // 排除不符合要求的字符
        if (c != ' ' && c != ',' && c != ';' && c != '{' && c != '}' && c != '*' && c != '_') {
            return true;
        }
    }
    return false;
}