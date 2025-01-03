/**
 * Implementation of TM context.
 * 
 * Author: Wenze Jin
 */

#include "tm/context.h"
#include "utils/exception.h"
#include <algorithm>

bool TMContext::validate() const {
    // 1. 检查 start_state 是否在 states 中
    if (states.find(start_state) == states.end()) {
        return false;
    }

    // 2. 检查 final_states 是否是 states 的子集
    if (!std::all_of(final_states.begin(), final_states.end(),
                     [this](const std::string &state) { return states.find(state) != states.end(); })) {
        return false;
    }

    // 3. 检查纸带数量是否大于0
    if (!(tape_num > 0)) {
        return false;
    }

    // 4. 检查 blank_char 是否在 tape_alphabet 中
    if (tape_alphabet.find(blank_char) == tape_alphabet.end()) {
        return false;
    }

    // 5. 检查 blank_char 是否在 input_alphabet 中
    if (input_alphabet.find(blank_char) != input_alphabet.end()) {
        return false;
    }

    // 6. 检查 input_alphabet 是否是 tape_alphabet 的子集
    if (!std::all_of(input_alphabet.begin(), input_alphabet.end(),
                     [this](char ch) { return tape_alphabet.find(ch) != tape_alphabet.end();  })) {
        return false;
    }

    // 7. 检查 transisions 中每个键值对是否有效
    auto tm_map = transitions.getMap();
    for (auto it = tm_map.begin(); it != tm_map.end(); it++) {
        const TMTransitionKey &key = it->first;
        const TMTransitionValue &value = it->second;

        // q: 检查当前状态是否有效
        if (states.find(key.state) == states.end()) {
            return false;
        }

        // X: 检查输入字符长度是否和带数相同，是否都是带字符或*
        if (key.input_chars.length() != tape_num) {
            return false;
        }
        for (char ch: key.input_chars) {
            if (ch == '*') {
                continue;
            }
            if (tape_alphabet.find(ch) == tape_alphabet.end()) {
                return false;
            }
        }

        // Y: 检查替换字符长度是否和带数相同，是否都是带字符或*
        if (value.replace_chars.length() != tape_num) {
            return false;
        }
        for (char ch: value.replace_chars) {
            if (ch == '*') {
                continue;
            }
            if (tape_alphabet.find(ch) == tape_alphabet.end()) {
                return false;
            }
        }

        // R/L: 检查带移动方向的长度是否和带数相同
        if (value.tape_directions.size() != tape_num) {
            return false;
        }

        // q': 检查下一个状态是否在 states 中
        if (states.find(value.next_state) == states.end()) {
            return false;
        }
    }

    // 全部检查通过
    return true;
}


bool TMContext::addTransition(const std::string &state, const std::string& input_chars, const std::string &replace_chars,
                       const std::vector<TapeDirection> &tape_directions, const std::string &next_state) {
    TMTransitionKey key(state, input_chars);
    TMTransitionValue value(next_state, replace_chars, tape_directions);

    transitions.insert(key, value);

    return true;
}

TMQueryResult TMContext::getTransition(const std::string& state, const std::string& input_chars) const {
    TMTransitionKey key(state, input_chars);
    return getTransition(key);
}

TMQueryResult TMContext::getTransition(const TMTransitionKey &key) const {
    return transitions.query(key);
}