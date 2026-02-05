#pragma once

struct Node {
    Node* left;
    Node* right;
    unsigned char symbol;
};

struct Code {
    unsigned int val;
    int len;
};
