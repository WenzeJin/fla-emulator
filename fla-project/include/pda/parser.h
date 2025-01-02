/**
 * PDA parser.
 * 
 * Author: Wenze Jin
 */

#ifndef FLA_PDA_PARSER_H
#define FLA_PDA_PARSER_H

#include "pda/context.h"
#include <string>
#include <regex>
#include "utils/exception.h"

class PDAParser {
    /**
     * Parse a line to PDA context.
     * 
     * @param line The line to parse.
     */
    static void parseLine(const std::string& line, PDAContext& context);

    /**
     * 解析集合类型的输入，解析为字符串集合。
     */
    static std::set<std::string> parseStrBraces(const std::string &input);

    /**
     * 解析集合类型的输入，解析为字符集合。
     */
    static std::set<char> parseCharBraces(const std::string &input);

    static void linePreprocess(std::string& line);

    static std::set<std::string> control_tokens;

    static std::regex state_regex;

    static bool isValidSymbol(char c);

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