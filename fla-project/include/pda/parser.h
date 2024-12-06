/**
 * PDA parser.
 * 
 * Author: Wenze Jin
 */

#ifndef FLA_PDA_PARSER_H
#define FLA_PDA_PARSER_H

#include <pda/context.h>
#include <string>
#include <exception>
#include <regex>

class SyntaxException : public std::exception {
private:
    std::string message;

public:
    // 构造函数接受一个错误信息
    explicit SyntaxException(const std::string& msg) : message(msg) {}

    // 重写 what() 方法返回错误信息
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class PDAParser {
    /**
     * Parse a line to PDA context.
     * 
     * @param line The line to parse.
     */
    static void parseLine(const std::string& line, PDAContext& context);

    /**
     * 解析集合类型的输入
     */
    static std::set<std::string> parseBraces(const std::string &input);

    static void linePreprocess(std::string& line);

    static std::set<std::string> control_tokens;

    static std::regex state_regex;

  public:
    /**
     * Parse a PDA configuration file.
     * 
     * @param filepath The filepath of the configuration file.
     * @return The PDA context parsed from the file.
     */
    static PDAContext parse(const std::string& filepath);
};

#endif