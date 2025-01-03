/**
 * TM Emulator
 * 
 * Author: Wenze Jin
 */

#ifndef FLA_TM_EMULATOR_H
#define FLA_TM_EMULATOR_H

#include "tm/context.h"
#include "tm/tape.h"
#include "utils/exception.h"
#include <string>
#include <vector>

class TMEmulator {

    TMContext context;

    bool verbose_mode = false;

    enum class EmulatorState {
        NEW,
        RUNNING,
        ACCEPT,
        HALT,
    };

    void verboseLog(const std::string &message);

    void verboseLogError(const std::string &message);

    void verboseLogID(const std::string &current_state, const std::vector<TMTape> &tapes, const int step_cnt);

    void verboseLogSyntaxError(const std::string& input, const int idx);

    /**
     * Check if the input string has syntax error.
     * 
     * @param input The input string.
     * @return The index of the syntax error, -1 if no error.
     */

    int checkSyntaxError(const std::string &input);

public:
    
    explicit TMEmulator(const TMContext &context);

    /**
     * Run the TM emulator
     *
     * @param input The input string.
     * @return the non_blank symbols on tape0.
     */
    std::string run(const std::string &input);

    void setVerboseMode(bool mode);
};

#endif