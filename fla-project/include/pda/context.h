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

class PDAContext {
    /**
     * Execution context for PDAs. All the information needed to run a PDA is stored here.
     * 
     * All the attributes are public for easy access. Should only be used in PDAExecutor.
     * 
     */

public:
    std::set<std::string> states;                   // Q
    std::string start_state;                        // q0
    std::set<std::string> final_states;             // F
    std::set<std::string> input_alphabet;           // Sigma
    std::set<std::string> stack_alphabet;           // Gamma
    std::unordered_map<std::string, std::string> transitions;   // delta


};

#endif