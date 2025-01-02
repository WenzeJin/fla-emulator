/**
 * Util functions to parse a specific token.
 */

#ifndef FLA_UTILS_PARSE_TOKEN_H
#define FLA_UTILS_PARSE_TOKEN_H

#include <string>
#include <set>

/**
 * A str set is "{abc,ac,ab ,d c}".
 * parse a str to std::set<std::string>
 * space will not be ignored
 */
std::set<std::string> parseStrSet(const std::string &input);

std::set<char> parseCharSet(const std::string &input);

#endif