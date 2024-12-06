/**
 * Exception class for syntax errors
 * 
 * Author: Wenze Jin
 */

#ifndef FLA_UTILS_SYNTAX_EXCPT_H
#define FLA_UTILS_SYNTAX_EXCPT_H

#include <exception>
#include <string>

class SyntaxException : public std::exception {
private:
    std::string item;
    std::string description;
    std::string message;

  public:
    // 构造函数接受一个错误信息
    explicit SyntaxException(const std::string &item, const std::string &description)
        : item(item), description(description) {
            message = std::string("Syntax error:\nAt: ") + item + "\n" + "Description: " + description;
        };

    // 重写 what() 方法返回错误信息
    const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif