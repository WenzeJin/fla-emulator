/**
 * Implementation of the PDAContext class.
 * 
 * Author: Wenze Jin
 */

#include <pda/context.h>
#include <algorithm>  // For std::all_of
#include <iostream>

/**
 * Validate the PDA context to ensure it is correctly configured.
 * 
 * @return true if the context is valid, false otherwise.
 */
bool PDAContext::validate() const {
    // 1. 检查 start_state 是否在 states 中
    if (states.find(start_state) == states.end()) {
        return false;
    }

    // 2. 检查 final_states 是否是 states 的子集
    if (!std::all_of(final_states.begin(), final_states.end(), [this](const std::string& state) {
        return states.find(state) != states.end();
    })) {
        return false;
    }

    //3. 检查 stack_start_symbol 是否在 stack_alphabet 中，或为 '_'
    if (stack_start_symbol != '_' && stack_alphabet.find(stack_start_symbol) == stack_alphabet.end()) {
        return false;
    }


    // 4. 检查 transitions 中的每个键值对是否有效
    for (auto it = transitions.begin(); it != transitions.end(); ++it) {
        const PDATransitionKey& key = it->first;
        const PDATransitionValue& value = it->second;

        // q: 检查当前状态是否有效
        if (states.find(key.state) == states.end()) {
            return false;
        }

        // a: 检查输入符号是否在输入字母表中
        if (input_alphabet.find(key.input) == input_alphabet.end() && key.input != '_') {
            return false;
        }

        // X: 检查栈顶符号是否在栈字母表中
        if (stack_alphabet.find(key.stack_top) == stack_alphabet.end()) {
            return false;
        }

        // q': 检查下一个状态是否在 states 中
        if (states.find(value.next_state) == states.end()) {
            return false;
        }

        // Y: 检查栈操作是否只包含有效的栈符号
        if (!std::all_of(value.stack_action.begin(), value.stack_action.end(), [this](char c) {
            return stack_alphabet.find(c) != stack_alphabet.end();
        })) {
            return false;
        }
    }
    
    return true;
}

/**
 * Add a transition to the transition table.
 * 
 * @return true if the transition is added successfully, false otherwise.
 */
bool PDAContext::addTransition(const std::string& state, const char input_symbol, const char stack_top, const std::string& next_state, const std::string& stack_action) {
    // 创建 PDATransitionKey 和 PDATransitionValue
    PDATransitionKey key(state, input_symbol, stack_top);
    PDATransitionValue value(next_state, stack_action);

    // 插入到 transitions 中
    transitions[key] = value;
    return true;
}

/**
 * Get the transition from the transition table.
 * 
 * @return The transition result in PDAQueryResult.
 */
PDAQueryResult PDAContext::getTransition(const std::string& state, const char input_symbol, const char stack_top) const {
    PDATransitionKey key(state, input_symbol, stack_top);
    return getTransition(key);
}

/**
 * Get the transition from the transition table using the key.
 * 
 * @return The transition result in PDAQueryResult.
 */
PDAQueryResult PDAContext::getTransition(const PDATransitionKey& key) const {
    auto it = transitions.find(key);
    if (it != transitions.end() && it->second.next_state != "") {
        return PDAQueryResult(it->second.next_state, it->second.stack_action);
    } else {
        return PDAQueryResult();  // 返回默认的失败查询结果
    }
}