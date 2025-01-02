/**
 * Execution context for TMs.
 * 
 * Author: Wenze Jin
 */

#ifndef FLA_TM_CONTEXT_H
#define FLA_TM_CONTEXT_H

#include "tm/tran_kv.h"
#include <set>

/**
 * Execution context for TMs. All the information needed to run a TM is stored here.
 * All the attributes are public for easy access.
 * Should only be used in TMEmulator.
 * 
 * 我们规定：
 * 纸带可以有多个。
 * 在转移函数中，如果输入字符的第i个为通配符*，则当前第i个纸带上无论是什么内容都可以转移。
 * 在转移函数中，如果替换字符的第i个为通配符*，则当前第i个纸带的内容不进行操作。
 */
struct TMContext {
    std::set<std::string> states;
    std::set<char> input_alphabet;
    std::set<char> tape_alphabet;
    std::string start_state;
    std::set<std::string> final_states;
    int tape_num;

    char blank_char;
    TMDeltaMap transitions;
    

    /**
     * Validate the TM context.
     * User should call this function before running the TM.
     * 
     * @return true if the context is valid, false otherwise.
     */
    bool validate() const;


    bool addTransition(const std::string &state, const std::string& input_chars, const std::string &replace_chars,
                       const std::vector<TapeDirection> &tape_directions, const std::string &next_state);

    TMQueryResult getTransition(const std::string& state, const std::string& input_chars) const;

private:
    
    TMQueryResult getTransition(const TMTransitionKey &key) const;
};

#endif