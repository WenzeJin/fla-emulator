/**
 * Implementation of the PDAParser class.
 * 
 * Author: Wenze Jin
 */

#include <pda/parser.h>

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <regex>

#include <iostream>

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
    int i = 0;
    while (std::getline(file, line)) {
        std::cout << "---------------------" << std::endl;

        std::cout << "Line " << i << ": " << line << std::endl;
        i++;

        // 行预处理
        linePreprocess(line);

        std::cout << "After preprocess: " << line << std::endl;
        // 解析行
        parseLine(line, context);
    }

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
            throw SyntaxException("Invalid line: " + line + "(leading space but not empty)");
        }
    } else {
        return;
    }

    // 检查控制符
    if (control_tokens.find(tokens[0]) == control_tokens.end()) {
        // 无控制符 是转移函数
        // TODO: 解析转移函数
        std::cout << "Type: Transition" << std::endl;
        return;
    } else {
        // 有控制符
        if (tokens.size() < 3 || tokens[1] != "=") {
            throw SyntaxException("Invalid line: " + line + "(missing '=')");
        }

        if (tokens[0] == "#Q") {
            // 状态集
            std::cout << "Type: #Q" << std::endl;
            std::set<std::string> states = parseBraces(line); 

            // 检查状态名是否合法
            for (auto state : states) {
                if (!std::regex_match(state, state_regex)) {
                    throw SyntaxException("Invalid state name: " + state + "(only [a-zA-Z0-9_]+ allowed)");
                }
            }
            context.states = std::move(states);

        } else if (tokens[0] == "#S") {
            // 栈符号集
            std::cout << "Type: #S" << std::endl;
        } else if (tokens[0] == "#G") {
            // 输入符号集
            std::cout << "Type: #G" << std::endl;
            
        } else if (tokens[0] == "#q0") {
            // 初始状态
            std::cout << "Type: #q0" << std::endl;

        } else if (tokens[0] == "#z0") {
            // 初始栈符号
            std::cout << "Type: #z0" << std::endl;


        } else if (tokens[0] == "#F") {
            // 终止状态集
            std::cout << "Type: #F" << std::endl;
            std::set<std::string> final_states = parseBraces(line);

            // 检查终止状态集合是否合法
            for (auto state : final_states) {
                if (!std::regex_match(state, state_regex)) {
                    throw SyntaxException("Invalid state name: " + state + "(only [a-zA-Z0-9_]+ allowed)");
                }
            }

            context.final_states = std::move(final_states);
        }
    }
}

std::set<std::string> PDAParser::parseBraces(const std::string& input) {
    std::set<std::string> result;

    // 找到第一个 '{' 和最后一个 '}'
    size_t start = input.find('{');
    size_t end = input.find('}');

    // 如果找到有效的大括号位置
    if (start != std::string::npos && end != std::string::npos && end > start + 1) {
        std::string content = input.substr(start + 1, end - start - 1);  // 提取 {} 中的内容

        // 使用 stringstream 按逗号分割字符串，但不去除空格
        std::stringstream ss(content);
        std::string token;
        while (std::getline(ss, token, ',')) {
            result.insert(token);  
        }
    }

    return result;
}