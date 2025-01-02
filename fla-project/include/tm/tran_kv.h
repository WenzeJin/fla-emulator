/**
 * Define the key-value pair for transition table.
 * 
 * Author: Wenze Jin
 */

#ifndef FLA_TM_TRAN_KV_H
#define FLA_TM_TRAN_KV_H

#include <string>
#include <vector>
#include <unordered_map>

enum TapeDirection {
    LEFT,
    RIGHT,
    STAY,
};

/**
 * Key for transition table.
 */
struct TMTransitionKey {
    
    std::string state;       // Current q
    std::string input_chars;       // input from each tape

    TMTransitionKey(std::string state, std::string input_chars);

    bool operator==(const TMTransitionKey& other) const;
};

/**
 * Hash function for TMTransitionKey
 */
struct TMTransitionKeyHash {
    std::size_t operator()(const TMTransitionKey& key) const;
};

/**
 * Value for transition table.
 */
struct TMTransitionValue {
    std::string next_state;   // next state
    std::string replace_chars;
    std::vector<TapeDirection> tape_directions;

    TMTransitionValue(std::string next_state, std::string replace_chars, const std::vector<TapeDirection>& tape_directions);
};

struct TMQueryResult {
    std::string next_state;
    std::string replace_chars;
    std::vector<TapeDirection> tape_directions;
    bool success;

    TMQueryResult(std::string next_state, std::string replace_chars, const std::vector<TapeDirection>& tape_directions);

    TMQueryResult();
};



using TranMap = std::unordered_map<TMTransitionKey, TMTransitionValue, TMTransitionKeyHash>;

/**
 * Map for TM transitions.
 * 对 unordered_map 进行包装，使其在存储时，将不同的 key 分开存储，
 * 但是在查询时，考虑通配符‘*’的影响。
 */
class TMDeltaMap {
    TranMap _map;

public:
    
    void insert(const TMTransitionKey &key, const TMTransitionValue &value);

    TMQueryResult query(const TMTransitionKey &key) const;

    inline const TranMap& getMap() const {
        return _map;
    }
};

#endif

