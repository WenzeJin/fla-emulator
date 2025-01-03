/**
 * Implementation of the TMEmulator class.
 * 
 * Author: Wenze Jin
 */


#include "tm/emulator.h"
#include "utils/exception.h"
#include <iostream>


TMEmulator::TMEmulator(const TMContext &context) : context(context) {
    // XXX: throw exceptions in constructer is not recommended.
    if (!context.validate()) {
        throw AutomataStructureException("Emulator using invalid TM context.");
    }
}

std::string TMEmulator::run(const std::string &input) {
    EmulatorState e_state = EmulatorState::NEW;

    std::string state = context.start_state;
    std::vector<TMTape> tapes(context.tape_num);
    tapes[0].init(input);

    int idx = checkSyntaxError(input);
    if (idx != -1) {
        verboseLogError("Input: " + input);
        verboseLogSyntaxError(input, idx);
        verboseLog("==================== END ====================");
        throw InputSyntaxError(input);
    } else {
        verboseLog("Input: " + input);
    }

    e_state = EmulatorState::RUNNING;
    verboseLog("==================== RUN ====================");

    int step_cnt = 0;
    
    while (e_state == EmulatorState::RUNNING) {
        verboseLogID(state, tapes, step_cnt);

        std::string next_state;
        std::string input_chars;
        std::string replace_chars;
        std::vector<TapeDirection> tape_directions;

        if (context.final_states.find(state) != context.final_states.end()) {
            // 已经到达终止状态
            e_state = EmulatorState::ACCEPT;
            break;
        }

        for (auto& tape: tapes) {
            input_chars += tape.read();
        }

        auto result = context.getTransition(state,input_chars);

        if (!result.success) {
            e_state = EmulatorState::HALT;
            break;
        }

        for (int i = 0; i < input_chars.size(); i++) {
            if (result.replace_chars[i] == '*') {
                replace_chars[i] = input_chars[i];
            } else {
                replace_chars[i] = result.replace_chars[i];
            }
        }

        for (int i = 0; i < context.tape_num; i++) {
            tapes[i].write(replace_chars[i]);
        }

        for (int i = 0; i < context.tape_num; i++) {
            switch(result.tape_directions[i]) {
            case TapeDirection::LEFT:
                tapes[i].moveLeft();
                break;
            case TapeDirection::RIGHT:
                tapes[i].moveRight();
                break;
            case TapeDirection::STAY:
                break;
            }
        }

        state = result.next_state;
        step_cnt++;
    }

    if (e_state == EmulatorState::ACCEPT || e_state == EmulatorState::HALT) {
        auto content = tapes[0].getNonBlank();
        verboseLog("Result: " + content);
        verboseLog("==================== END ====================");
        return content;
    } else {
        std::string content;
        verboseLog("Result: " + content);
        verboseLog("==================== END ====================");
        return content;
    }


}

void TMEmulator::setVerboseMode(bool mode) {
    verbose_mode = mode;
}

void TMEmulator::verboseLog(const std::string &message) {
    if (verbose_mode) {
        std::cout << message << std::endl;
    }
}

void TMEmulator::verboseLogError(const std::string &message) {
    if (verbose_mode) {
        std::cerr << message << std::endl;
    }
}

void TMEmulator::verboseLogSyntaxError(const std::string& input, const int idx) {
    if (verbose_mode) {
        verboseLogError("==================== ERR ====================");
        std::string message;
        message += "error: '" + std::string(1, input[idx]) + "' was not declared in the set of input symbols\n";
        message += "Input: " + input + "\n";
        message += "       ";
        for (int i = 0; i < idx; i++) {
            message += " ";
        }
        message += "^";
        verboseLogError(message);
    }
}

void TMEmulator::verboseLogID(const std::string &current_state, const std::vector<TMTape> &tapes, const int step_cnt) {
    if (verbose_mode) {
        std::cout << "Step   : " << step_cnt << std::endl;
        std::cout << "State  : " << current_state << std::endl;
        for (int i = 0; i < tapes.size(); i++) {
            std::string index;
            std::string tape;
            std::string head;

            std::string content;
            int idx = 0;

            content = tapes[i].getNonBlank(idx);

            for (auto ch: content) {
                std::string idx_str = std::to_string(abs(idx));
                idx_str += ' ';
                std::string char_str;
                char_str += ch;
                while (char_str.length() < idx_str.length()) {
                    char_str += ' ';
                }
                index += idx_str;
                tape += char_str;
                std::string head_str(idx_str.length(), ' ');
                if (tapes[i].getHead() == idx) {
                    head_str[0] = '^';
                }
                head += head_str;
                idx++;
            }

            std::string idx_head;
            std::string tape_head;
            std::string head_head;

            idx_head += "Index" + std::to_string(i);
            tape_head += "Tape" + std::to_string(i);
            head_head += "Head" + std::to_string(i);

            while (idx_head.length() < 7) {
                idx_head += ' ';
            }
            while (tape_head.length() < 7) {
                tape_head += ' ';
            }
            while (head_head.length() < 7) {
                head_head += ' ';
            }

            index = idx_head + ": " + index;
            tape = tape_head + ": " + tape;
            head = head_head + ": " + head;

            std::cout << index << std::endl;
            std::cout << tape << std::endl;
            std::cout << head << std::endl;

        }

        std::cout << "---------------------------------------------" << std::endl;
    }
}

int TMEmulator::checkSyntaxError(const std::string &input) {
    for (int i = 0; i < input.size(); i++) {
        if (context.input_alphabet.find(input[i]) == context.input_alphabet.end()) {
            return i;
        }
    }
    return -1;
}