/**
 * Definition of the TMParser class.
 * 
 * Author: Wenze Jin
 */

#ifndef FLA_TM_PARSER_H
#define FLA_TM_PARSER_H

#include "tm/context.h"
#include <string>
#include <regex>
#include "utils/exception.h"

class TMParser {

    static void parseLine(const std::string& line, TMContext& context);

    static void linePreprocess(std::string &line);

    static std::set<std::string> control_tokens;

    static std::regex state_regex;

    static bool isValidSymbol(char c);

public:

    /**
     * Parse a TM configuration file.
     * 
     * @param filepath The filepath of the configuration file.
     * @return The TM context parsed from the file.
     */
    static TMContext parse(const std::string &filepath);
};

#endif