/**
 * Implementation of TM context.
 * 
 * Author: Wenze Jin
 */

#include "tm/context.h"
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
    if (!tape_num > 0) {
        return false;
    }

    // 4. 检查 blank_char 是否在 tape_alphabet 中
    if (tape_alphabet.find(blank_char) == tape_alphabet.end()) {
        return false;
    }

    // 5. 检查 input_alphabet 是否是 tape_alphabet 的子集
}