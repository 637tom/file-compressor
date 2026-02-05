#pragma once
#include "Huffman.h"
#include <vector>

class HuffmanEncoder {
    private:
        Node* root;
        std::set <std::pair<int, Node*>> s;
        std::vector<unsigned char> text;
        int freq[256];
        Code codes[256];

        void add(int freq, int symbol, Node* left = nullptr, Node* right = nullptr);
        void remTree(Node* curr);
        void cntFreq();
        void buildTree();
        void create_codes(Node* curr, int len, unsigned int val);
    public:
        HuffmanEncoder(std::vector<unsigned char> inp);
        ~HuffmanEncoder();
        std::vector<uint8_t> compress();

};