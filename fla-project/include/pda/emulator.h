/**
 * PDA Emulator.
 * 
 * Author: Wenze Jin
 */

#include <pda/context.h>
#include <string>
#include <utils/exception.h>
#include <stack>

class PDAEmulator {
    
    PDAContext context;

    bool verbose_mode = false;

    enum class EmulatorState {
        NEW,            // 创建态
        RUNNING,        // 运行态
        EOS,            // 因为输入结束而停止       End of String
        COS,            // 因为栈空而停止       Clear of Stack
        HALT            // 因为无可用的转移函数而停止
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