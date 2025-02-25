/**
 * Define the tape structure and the tape functions.
 * 
 * Author: Wenze Jin
 */

#ifndef FLA_TM_TAPE_H
#define FLA_TM_TAPE_H

#include <deque>
#include <string>

class TMTape {
    std::deque<char> tape;

    int left_idx;
    int head;
    
    // character for blank spaces
    char blank;

public:
    
    TMTape();

    TMTape(char blank_char);

    /**
     * Init the tape with input string.
     * @param init_string The initial input string. 
     */
    void init(std::string init_string);

    char read() const;

    void write(char x);

    void moveLeft();

    void moveRight();

    void clear();

    /**
     * This function will minimize the deque, by removing useless blank characters around.
     */
    void minimize();


    inline int getLeftIdx() const {
        return left_idx;
    }

    inline int getHead() const {
        return head;
    }

    inline const std::deque<char>& getTape() const {
        return tape;
    }

    std::string getAnswer() const;

    std::string getNonBlank(int &idx) const;
};

#endif