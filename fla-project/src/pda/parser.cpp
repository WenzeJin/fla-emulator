/**
 * Implementation of the PDAParser class.
 * 
 * Author: Wenze Jin
 */

#include "pda/parser.h"

#include "utils/parse_token.h"
#include <fstream>
#include <sstream>

#include <algorithm>




std::set<std::string> PDAParser::control_tokens = {
    "#Q",
    "#S",
    "#G",
    "#q0",
    "#z0",
    "#F"
};

std::regex PDAParser::state_regex = std::regex("[a-zA-Z0-9_]+");


/**
 * Parse a PDA configuration file.
 * 
 * @param filepath The filepath of the configuration file.
 * @return The PDA context parsed from the file.
 */
PDAContext PDAParser::parse(const std::string& filepath) {
    // 读取文件
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }

    PDAContext context;

    std::string line;
    int line_idx = 0;

    while (std::getline(file, line)) {
        // 行预处理
        linePreprocess(line);

        line_idx++;
        // 解析行
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

void PDAParser::linePreprocess(std::string& line) {
    // 去掉注释
    // 找到 ';' 在字符串中的位置
    size_t pos = line.find(';');
    
    // 如果找到了 ';'，则截取之前的部分
    if (pos != std::string::npos) {
        line = line.substr(0, pos); // 截取到 ';' 前的内容
    }
    
    // 去除末尾可能存在的多余空格
    line.erase(line.find_last_not_of(" \t") + 1);
}


/**
 * Parse a line to PDA context.
 * 
 * @param line The line to parse.
 */
void PDAParser::parseLine(const std::string& line, PDAContext& context) {
    
    // 分割字符串 into tokens
    std::vector<std::string> tokens;

    std::istringstream iss(line);
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }

    if (tokens.size() > 0) {
        if (line[0] == ' ') {
            throw AutomataSyntaxException(line, "leading space but not empty");
        }
    } else {
        return;
    }

    // 检查控制符
    if (control_tokens.find(tokens[0]) == control_tokens.end()) {
        // 无可用控制符，此时第一个token不应该以#开头
        if (tokens[0][0] == '#') {
            throw AutomataSyntaxException(tokens[0], "Start with '#' but not a valid control token.");
        }

        // 是转移函数
        // 检查tokens的数量是否满足转移函数要求：5个
        if (tokens.size() != 5) {
            throw AutomataSyntaxException(line, "Invalid number of tokens for a transition");
        }

        std::string state, next_state, stack_action;
        char input_symbol, stack_top_symbol;

        // 检查状态名是否合法
        if (std::regex_match(tokens[0], state_regex)) {
            state = tokens[0];
        } else {
            throw AutomataSyntaxException(tokens[0], "only [a-zA-Z0-9_]+ allowed");
        }

        if (std::regex_match(tokens[3], state_regex)) {
            next_state = tokens[3];
        } else {
            throw AutomataSyntaxException(tokens[3], "only [a-zA-Z0-9_]+ allowed");
        }

        // 检查输入符号是否合法
        if (tokens[1].size() == 1) {
            input_symbol = tokens[1][0];
            if (!isValidSymbol(input_symbol) && input_symbol != '_') {
                throw AutomataSyntaxException(std::to_string(input_symbol), "only printable ASCII characters or '_' allowed");
            }
        } else {
            throw AutomataSyntaxException(tokens[1], "only single character allowed");
        }

        // 检查栈顶符号是否合法
        if (tokens[2].size() == 1) {
            stack_top_symbol = tokens[2][0];
            if (!isValidSymbol(stack_top_symbol)) {
                throw AutomataSyntaxException(std::to_string(stack_top_symbol), "only printable ASCII characters or allowed");
            }
        } else {
            throw AutomataSyntaxException(tokens[2], "only single character allowed");
        }

        // 检查栈操作是否合法
        if (tokens[4] == "_") {
            stack_action = "";
        } else {
            stack_action = tokens[4];
            for (char c : stack_action) {
                if (!isValidSymbol(c)) {
                    throw AutomataSyntaxException(std::to_string(c), "only printable ASCII characters allowed, or ONLY '_' for empty");
                }
            }
        }

        // 添加转移函数
        context.addTransition(state, input_symbol, stack_top_symbol, next_state, stack_action);

        return;

    } else {
        // 有控制符
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

        } else if (tokens[0] == "#G") {
            // 栈符号集
            std::set<char> symbols = parseCharSet(tokens[2]);

            // 检查栈符号是否合法
            for (auto symbol : symbols) {
                if (!isValidSymbol(symbol)) {
                    throw AutomataSyntaxException(std::to_string(symbol), "only printable ASCII characters allowed");
                }
            }

            context.stack_alphabet = std::move(symbols);

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

        } else if (tokens[0] == "#q0") {
            // 初始状态
            std::string start_state = tokens[2];

            // 检查初始状态是否合法
            if (!std::regex_match(start_state, state_regex)) {
                throw AutomataSyntaxException(start_state, "only [a-zA-Z0-9_]+ allowed");
            }

            context.start_state = std::move(start_state);

        } else if (tokens[0] == "#z0") {
            // 初始栈符号
            if (tokens[2].size() != 1) {
                throw AutomataSyntaxException(tokens[2], "only single character allowed");
            }

            char stack_start_symbol = tokens[2][0];

            // 检查初始栈符号是否合法

            if (!isValidSymbol(stack_start_symbol) && stack_start_symbol != '_') {
                throw AutomataSyntaxException(std::to_string(stack_start_symbol), "only printable ASCII characters allowed");
            }

            context.stack_start_symbol = stack_start_symbol;

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
        }
    }

    
}

bool PDAParser::isValidSymbol(char c) {
    // 检查是否是 ASCII 可显示字符
    if (c >= 32 && c <= 126) {
        // 排除不符合要求的字符
        if (c != ' ' && c != ',' && c != ';' && c != '{' && c != '}' && c != '*' && c != '_') {
            return true;
        }
    }
    return false;
}