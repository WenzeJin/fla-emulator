/**
 * Implementations of the key-value pair for transition table.
 * 
 * Author: Wenze Jin
 */

#include "tm/tran_kv.h"



TMTransitionKey::TMTransitionKey(std::string state, std::string input_chars) : state(state), input_chars(input_chars) {}

bool TMTransitionKey::operator==(const TMTransitionKey& other) const {
    return state == other.state && input_chars == other.input_chars;
}



std::size_t TMTransitionKeyHash::operator()(const TMTransitionKey& key) const {
    // Combine hashes of state and input_chars
    std::size_t hash1 = std::hash<std::string>()(key.state);
    std::size_t hash2 = std::hash<std::string>()(key.input_chars);
    return hash1 ^ (hash2 << 1); // Combine hashes
}



TMTransitionValue::TMTransitionValue(std::string next_state, std::string replace_chars, const std::vector<TapeDirection>& tape_directions) 
    : next_state(next_state), replace_chars(replace_chars), tape_directions(tape_directions){}



TMQueryResult::TMQueryResult(std::string next_state, std::string replace_chars, const std::vector<TapeDirection>& tape_directions)
    : next_state(next_state), replace_chars(replace_chars), tape_directions(tape_directions), success(true) {}

TMQueryResult::TMQueryResult() : success(false) {}


void TMDeltaMap::insert(const TMTransitionKey &key, const TMTransitionValue &value) {
    _map.emplace(key, value);
}

// Helper function to check if input_chars matches stored_chars with wildcard logic
bool matchesWithWildcard(const std::string& input_chars, const std::string& stored_chars) {
    if (input_chars.size() != stored_chars.size()) return false;

    for (size_t i = 0; i < input_chars.size(); ++i) {
        if (stored_chars[i] != '*' && stored_chars[i] != input_chars[i]) {
            return false;
        }
    }
    return true;
}


TMQueryResult TMDeltaMap::query(const TMTransitionKey &key) const {
    TMTransitionValue result("", "", std::vector<TapeDirection>());
    bool success = false;

    for (const auto& pair : _map) {
        const TMTransitionKey& stored_key = pair.first;
        if (stored_key.state == key.state && matchesWithWildcard(key.input_chars, stored_key.input_chars)) {
            result = pair.second;
            success = true;
            break;
        }
    }

    if (success) {
        return TMQueryResult(result.next_state, result.replace_chars, result.tape_directions);
    } else {
        return TMQueryResult();
    }
}