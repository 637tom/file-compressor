#pragma once
#include <cstdint>

struct Node {
    Node* left;
    Node* right;
    uint8_t symbol;
};

struct Code {
    unsigned int val;
    unsigned int len;
};
