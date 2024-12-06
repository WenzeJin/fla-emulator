/**
 * Implementation of the PDATransitionKey, PDATransitionValue and PDAQueryResult classes.
 * 
 * Author: Wenze Jin
 */

#include <pda/tran_kv.h>
#include <functional>

/**
 * 构造函数：初始化 PDATransitionKey
 */
PDATransitionKey::PDATransitionKey(std::string state, char input, char stack_top)
    : state(std::move(state)), input(input), stack_top(stack_top) {}

/**
 * 比较运算符：判断两个 PDATransitionKey 是否相等
 */
bool PDATransitionKey::operator==(const PDATransitionKey& other) const {
    return state == other.state && input == other.input && stack_top == other.stack_top;
}

/**
 * 哈希函数：为 PDATransitionKey 提供哈希值，用于 unordered_map
 */
std::size_t PDATransitionKeyHash::operator()(const PDATransitionKey& key) const {
    // 使用组合哈希：将 state、input 和 stack_top 组合计算哈希值
    return std::hash<std::string>()(key.state) ^ 
           (std::hash<char>()(key.input) << 1) ^ 
           (std::hash<char>()(key.stack_top) << 2);
}

/**
 * 构造函数：初始化 PDATransitionValue
 */
PDATransitionValue::PDATransitionValue(std::string next_state, std::string stack_action)
    : next_state(std::move(next_state)), stack_action(std::move(stack_action)) {}

/**
 * 构造函数：初始化 PDATransitionValue
 */
PDATransitionValue::PDATransitionValue() : next_state(""), stack_action("") {};

/**
 * 构造函数：初始化 PDAQueryResult（用于成功返回）
 */
PDAQueryResult::PDAQueryResult(std::string next_state, std::string stack_action)
    : next_state(std::move(next_state)), stack_action(std::move(stack_action)), success(true) {}

/**
 * 默认构造函数：初始化 PDAQueryResult（用于失败返回）
 */
PDAQueryResult::PDAQueryResult() 
    : next_state(""), stack_action(""), success(false) {}