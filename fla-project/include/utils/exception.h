/**
 * Exception class for syntax errors
 * 
 * Author: Wenze Jin
 */

#ifndef FLA_UTILS_SYNTAX_EXCPT_H
#define FLA_UTILS_SYNTAX_EXCPT_H

#include <exception>
#include <string>
#include <sstream>

class AutomataSyntaxException : public std::exception {

    std::string item;
    std::string description;
    std::string message;

    std::string line;
    int line_idx;

  public:
    // 构造函数接受一个错误信息
    explicit AutomataSyntaxException(const std::string &item, const std::string &description)
        : item(item), description(description) {
            message = std::string("Syntax error at: ") + item + "\n" + "Description: " + description;
            line_idx = -1;
        };

    void setLine(int idx, const std::string& line) {
        std::ostringstream oss;
        oss << "Syntax error at line " << idx << ": " << line << std::endl;
        oss << "Location: " << item << std::endl;
        oss << "Description: " << description;

        message = oss.str();
    }

    // 重写 what() 方法返回错误信息
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class AutomataStructureException : public std::exception {

    std::string message;

public:
    // 构造函数接受一个错误信息
    explicit AutomataStructureException(const std::string &message) : message(message) {}

    // 重写 what() 方法返回错误信息
    const char* what() const noexcept override {
        return message.c_str();
    }
};



class InputSyntaxError : public std::exception {
    std::string input;
    std::string message;
public:
    explicit InputSyntaxError(const std::string& input) : input(input) {
        message = std::string("Syntax error in input: ") + input;
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif