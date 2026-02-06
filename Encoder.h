#pragma once
#include "Huffman.h"
#include <vector>
#include <set>

class HuffmanEncoder {
    private:
        Node* root;
        std::set <std::pair<unsigned int, Node*>> s;
        std::vector<uint8_t> text;
        unsigned int freq[256];
        Code codes[256];

        void add(unsigned int freq, uint8_t symbol, Node* left = nullptr, Node* right = nullptr);
        void remTree(Node* curr);
        void cntFreq();
        void buildTree();
        void create_codes(Node* curr, unsigned int len, unsigned int val);
    public:
        HuffmanEncoder(std::vector<uint8_t> inp);
        ~HuffmanEncoder();
        std::vector<uint8_t> compress();

};