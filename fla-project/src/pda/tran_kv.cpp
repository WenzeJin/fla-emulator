/**
 * Implementation of the PDATransitionKey and PDATransitionValue of PDA
 * 
 * Author: Wenze Jin
 */

#include <pda/tran_kv.h>


/**
 * PDATransitionKey Initialization
 */
PDATransitionKey::PDATransitionKey(std::string state, std::string input, std::string stack_top)
    : state(std::move(state)), input(std::move(input)), stack_top(std::move(stack_top)) {}

/**
 * PDATransitionKey Comparison
 */
bool PDATransitionKey::operator==(const PDATransitionKey& other) const {
    return state == other.state && input == other.input && stack_top == other.stack_top;
}

/**
 * PDATransitionKeyHash Implementation
 * 
 * Inorder to use PDATransitionKey as key in unordered_map, 
 * we need to define a hash function for it.
 */
std::size_t PDATransitionKeyHash::operator()(const PDATransitionKey& key) const {
    return std::hash<std::string>()(key.state) ^
           (std::hash<std::string>()(key.input) << 1) ^
           (std::hash<std::string>()(key.stack_top) << 2);
}

/**
 * PDATransitionValue Initialization
 */
PDATransitionValue::PDATransitionValue(std::string next_state, std::string stack_action)
    : next_state(std::move(next_state)), stack_action(std::move(stack_action)) {}