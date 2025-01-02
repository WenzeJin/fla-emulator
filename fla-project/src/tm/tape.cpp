/**
 * The implementation of Tape for Turing Machines
 * 
 * Author: Wenze Jin
 */

#include "tm/tape.h"

TMTape::TMTape() : blank('_'), left_idx(0), head(0), tape(1) {
    tape[0] = blank;
}

TMTape::TMTape(char blank_char) : blank(blank_char), left_idx(0), head(0), tape(1) {
    tape[0] = blank;
}

void TMTape::init(std::string init_string) {
    left_idx = 0;
    head = 0;
    tape.clear();
    for (auto x: init_string) {
        tape.push_back(x);
    }
}

void TMTape::clear() {
    left_idx = 0;
    head = 0;
    tape.clear();
    tape.push_back(blank);
}

char TMTape::read() {
    return tape[head-left_idx];
}

void TMTape::moveLeft() {
    if (head == left_idx) {
        tape.push_front(blank);
        left_idx--;
        head--;
    } else {
        head--;
    }
}

void TMTape::moveRight() {
    if (head - left_idx + 1 == tape.size()) {
        tape.push_back(blank);
        head++;
    } else {
        head++;
    }
}

void TMTape::minimize() {
    while (left_idx < head && tape.size() > 1 && tape.front() == blank) {
        tape.pop_front();
        left_idx++;
    }
    while (head - left_idx + 1 < tape.size() && tape.size() > 1 && tape.back() == blank) {
        tape.pop_back();
    }
}