/**
 * Implementation of the Emulator class.
 * 
 * Author: Wenze Jin
 */

#include <pda/emulator.h>
#include <pda/tran_kv.h>
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

        if(stack.empty()) {
            e_state = EmulatorState::COS;
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
                // 输入结束
                e_state = EmulatorState::EOS;
                break;
            }

            result = context.getTransition(state, input[idx], stack.top());
            if (result.success) {
                next_state = result.next_state;
                stack_action = result.stack_action;
                idx++;
            } else {
                // 没有可用的转移函数 HALT
                e_state = EmulatorState::HALT;
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

    // 目前已停机，检查Emulator状态和PDA状态

    // 如果是因为 HALT 而停机
    if (e_state == EmulatorState::HALT) {
        verboseLog("Result: false");
        verboseLog("==================== END ====================");
        return false;
    }

    // 如果是因为 COS 而停机
    if (e_state == EmulatorState::COS) {
        // 检查输入串是否已经消耗完毕
        if (idx >= input.size()) {
            e_state = EmulatorState::EOS;
        } else {
            verboseLog("Result: false");
            verboseLog("==================== END ====================");
            return false;
        }
    }

    // 如果是因为 EOS 而停机
    if (e_state == EmulatorState::EOS) {
        // 检查是否在终止状态
        if (context.final_states.find(state) != context.final_states.end()) {
            verboseLog("Result: true");
            verboseLog("==================== END ====================");
            return true;
        } else {
            verboseLog("Result: false");
            verboseLog("==================== END ====================");
            return false;
        }
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