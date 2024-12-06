/**
 * Execution context for PDAs.
 * 
 * Author: Wenze Jin
 */

#ifndef FLA_PDA_CONTEXT_H
#define FLA_PDA_CONTEXT_H

#include <unordered_map>
#include <string>
#include <vector>
#include <set>
#include <pda/tran_kv.h>

using DeltaMap = std::unordered_map<PDATransitionKey, PDATransitionValue, PDATransitionKeyHash>;

// TODO: Change the Implementation to unordered_set

/**
 * Execution context for PDAs. All the information needed to run a PDA is stored here.
 * All the attributes are public for easy access. 
 * Should only be used in PDAExecutor.
 * 我们规定：
 * 在转移函数中，如果输入符号或者栈顶符号为`'_'`，则表示空。
 * 在转移函数中，如果要表示栈操作为空，可以使用空字符串""，而不是使用`'_'`。
 */
struct PDAContext {
    std::set<std::string> states;           // Q
    std::string start_state;                // q0
    std::set<std::string> final_states;     // F
    std::set<char> input_alphabet;          // Σ
    std::set<char> stack_alphabet;          // Γ
    char stack_start_symbol;                // Z0, '_' is used as empty
    DeltaMap transitions;                   // delta

    /**
     * Validate the PDA context.
     * User should call this function before running the PDA.
     * 
     * @return true if the context is valid, false otherwise.
     */
    bool validate() const;

    /**
     * Add a transition to the transition table.
     * 
     * @param state The current state.
     * @param input_symbol The input symbol.
     * @param stack_top The stack top symbol.
     * @param next_state The next state.
     * @param stack_action The stack action.
     * 
     * @return true if the transition is added successfully, false otherwise.
     */
    bool addTransition( const std::string& state,           // q
                        const char input_symbol,    // a
                        const char stack_top,       // X
                        const std::string& next_state,      // q'
                        const std::string& stack_action );  // Y
    /**
     * Get the transition from the transition table. 
     * API for user to query the transition.
     * 
     * @param state The current state.
     * @param input_symbol The input symbol.
     * @param stack_top The stack top symbol.
     * 
     * @return The transition result, in PDAQueryResult.
     */
    PDAQueryResult getTransition( const std::string& state,             // q
                                  const char input_symbol,      // a
                                  const char stack_top ) const; // X

private:
    /**
     * Get the transition from the transition table.
     * User will not need to use structure PDATransitionKey or PDATrasitionValue directly.
     * 
     * @param key The key of the transition.
     * 
     * @return The transition result, in PDAQueryResult.
     */
    PDAQueryResult getTransition(const PDATransitionKey& key) const;
};

#endif