/**
 * Define the key-value pair for transition table
 * 
 * Author: Wenze Jin
 */

#ifndef FLA_PDA_TRAN_KV_H
#define FLA_PDA_TRAN_KV_H

#include <string>

/**
 * Key for transition table.
 */
struct PDATransitionKey {
    
    std::string state;       // Current q
    std::string input;       // input symbol
    std::string stack_top;   // stack top symbol

    PDATransitionKey(std::string state, std::string input, std::string stack_top);

    bool operator==(const PDATransitionKey& other) const;
};

/**
 * Hash function for PDATransitionKey
 */
struct PDATransitionKeyHash {
    std::size_t operator()(const PDATransitionKey& key) const;
};

/**
 * Value for transition table.
 */
struct PDATransitionValue {
    std::string next_state;   // next state
    std::string stack_action; // stack action

    PDATransitionValue(std::string next_state, std::string stack_action);
};

#endif