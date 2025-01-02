/**
 * PDA Emulator.
 * 
 * Author: Wenze Jin
 */
#ifndef FLA_PDA_EMULATOR_H
#define FLA_PDA_EMULATOR_H

#include "pda/context.h"
#include <string>
#include "utils/exception.h"
#include <stack>

class PDAEmulator {
    
    PDAContext context;

    bool verbose_mode = false;

    enum class EmulatorState {
        NEW,            // 创建态
        RUNNING,        // 运行态
        ACCEPT,         // 接受态
        REJECT,         // 拒绝态
    };

    void verboseLog(const std::string& message);

    void verboseLogError(const std::string& message);

    void verboseLogID(const std::string current_state, const std::stack<char>& stack, const int step_cnt);

    void verboseLogSyntaxError(const std::string& input, const int idx);

    /**
     * Check if the input string has syntax error.
     * 
     * @param input The input string.
     * @return The index of the syntax error, -1 if no error.
     */
    int checkSyntaxError(const std::string& input);

  public:
    PDAEmulator(const PDAContext& context);

    /**
     * Run the PDA emulator.
     * 
     * @param input The input string.
     * @return true if the input is accepted, false otherwise.
     */
    bool run(const std::string& input);

    void setVerboseMode(bool mode);
};

#endif