/**
 * Implementation of the Emulator class.
 * 
 * Author: Wenze Jin
 */

#include "pda/emulator.h"
#include "pda/tran_kv.h"
#include <utils/exception.h>
#include <stack>
#include <iostream>

PDAEmulator::PDAEmulator(const PDAContext& context) : context(context) {
    if (!context.validate()) {
        throw AutomataStructureException("Emulator using invalid PDA context.");
    }
}

bool PDAEmulator::run(const std::string& input) {
    EmulatorState e_state = EmulatorState::NEW;

    // 初始化状态
    std::string state = context.start_state;
    std::stack<char> stack;
    stack.push(context.stack_start_symbol);

    verboseLog("Input: " + input);

    int idx = checkSyntaxError(input);
    if (idx != -1) {
        verboseLogSyntaxError(input, idx);
        verboseLog("==================== END ====================");
        throw InputSyntaxError(input);
    }

    e_state = EmulatorState::RUNNING;
    verboseLog("==================== RUN ====================");

    int step_cnt = 0;
    idx = 0;

    while (e_state == EmulatorState::RUNNING) {
        verboseLogID(state, stack, step_cnt);

        std::string next_state;
        std::string stack_action;

        if (idx >= input.size() && context.final_states.find(state) != context.final_states.end())  {
            // 输入被消耗完且目前在终止状态，直接接受
            e_state = EmulatorState::ACCEPT;
            break;
        }

        if(stack.empty()) {
            // 栈空了，但是目前还没接受，直接拒绝
            e_state = EmulatorState::REJECT;
            break;
        }

        // 首先检查无条件转移
        PDAQueryResult result = context.getTransition(state, '_', stack.top());
        if (result.success) {
            next_state = result.next_state;
            stack_action = result.stack_action;
        } else {
            // 检查输入符号
            if (idx >= input.size()) {
                // 输入结束, 且无无条件转移, 且不在接受状态中
                e_state = EmulatorState::REJECT;
                break;
            }

            result = context.getTransition(state, input[idx], stack.top());
            if (result.success) {
                next_state = result.next_state;
                stack_action = result.stack_action;
                idx++;
            } else {
                // 没有可用的转移函数，且目前字符串没消耗完，而且不在接受状态，直接拒绝
                e_state = EmulatorState::REJECT;
                break;
            }
        }

        // 实施转移
        state = next_state;
        stack.pop();
        for (int i = stack_action.size() - 1; i >= 0; i--) {
            stack.push(stack_action[i]);
        }

        step_cnt++;
    }

    if (e_state == EmulatorState::ACCEPT) {
        verboseLog("Result: true");
        verboseLog("==================== END ====================");
        return true;
    } else if (e_state == EmulatorState::REJECT) {
        verboseLog("Result: false");
        verboseLog("==================== END ====================");
        return false;
    }

    return false;
}

void PDAEmulator::setVerboseMode(bool mode) {
    verbose_mode = mode;
}

void PDAEmulator::verboseLog(const std::string& message) {
    if (verbose_mode) {
        std::cout << message << std::endl;
    }
}

void PDAEmulator::verboseLogError(const std::string& message) {
    if (verbose_mode) {
        std::cout << "==================== ERR ====================" << std::endl;
        std::cout << message << std::endl;
    }
}

void PDAEmulator::verboseLogSyntaxError(const std::string& input, const int idx) {
    if (verbose_mode) {
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

void PDAEmulator::verboseLogID(const std::string current_state, const std::stack<char>& stack, const int step_cnt) {
    if (verbose_mode) {
        std::cout << "Step : " << step_cnt << std::endl;
        std::cout << "State: " << current_state << std::endl;
        std::cout << "Stack: ";
        std::stack<char> tmp_stack = stack;
        while (!tmp_stack.empty()) {
            std::cout << tmp_stack.top() << " ";
            tmp_stack.pop();
        }
        std::cout << "||(Stack Bottom)" << std::endl;
        std::cout << "---------------------------------------------" << std::endl;
    }
}

int PDAEmulator::checkSyntaxError(const std::string& input) {
    for (int i = 0; i < input.size(); i++) {
        if (context.input_alphabet.find(input[i]) == context.input_alphabet.end()) {
            return i;
        }
    }
    return -1;
}