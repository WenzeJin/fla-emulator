
#include "utils/parse_token.h"
#include "utils/exception.h"

std::set<std::string> parseStrSet(const std::string &input) {
    std::set<std::string> result;

    // 找到第一个 '{' 和第一个 '}'
    size_t start = input.find('{');
    size_t end = input.find('}');

    // 如果找到有效的大括号位置
    if (start != std::string::npos && end != std::string::npos && start == 0 && end == input.length() - 1) {
        std::string content = input.substr(start + 1, end - start - 1);  // 提取 {} 中的内容

        // 使用 stringstream 按逗号分割字符串，但不去除空格
        std::stringstream ss(content);
        std::string token;
        while (std::getline(ss, token, ',')) {
            result.insert(token);  
        }
    } else {
        throw AutomataSyntaxException(input, "Expected {}.");
    }

    return result;
}



std::set<char> parseCharSet(const std::string &input) {
    std::set<char> result;

    // 找到第一个 '{' 和最后一个 '}'
    size_t start = input.find('{');
    size_t end = input.find('}');

    // 如果找到有效的大括号位置
    if (start != std::string::npos && end != std::string::npos && start == 0 && end == input.length() - 1) {
        std::string content = input.substr(start + 1, end - start - 1);  // 提取 {} 中的内容

        // 使用 stringstream 按逗号分割字符串，但不去除空格
        std::stringstream ss(content);
        std::string token;
        while (std::getline(ss, token, ',')) {
            if (token.size() != 1) {
                throw AutomataSyntaxException(token, "only single character allowed");
            }
            result.insert(token[0]);  
        }
    } else {
        throw AutomataSyntaxException(input, "Expected {}.");
    }

    return result;
}