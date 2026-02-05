#include <iostream>
#include <set>
#include <utility>
#include <cstdint>
#include "Encoder.h"

void HuffmanEncoder::add(int freq, int symbol, Node* left, Node* right) {
    Node* curr = new Node();
    curr->symbol = symbol;
    curr->left = left;
    curr->right = right;
    s.insert({freq, curr});
}

void HuffmanEncoder::remTree(Node* curr) {
    if(curr == nullptr) return;
    remTree(curr->left);
    remTree(curr->right);
    delete curr;
}

HuffmanEncoder::HuffmanEncoder(std::vector<unsigned char> inp) {
    text = inp;
    root = nullptr;
    for(int i = 0; i < 256; i++) {
        freq[i] = 0;
    }
}

HuffmanEncoder::~HuffmanEncoder() {
    remTree(root);
}

void HuffmanEncoder::cntFreq() {
    for(auto ch : text) {
        freq[ch]++;
    }
}

void HuffmanEncoder::buildTree() {
    for(int i = 0; i < 256; i++) {
        if(freq[i] > 0) {
            add(freq[i], i);
        }
    }
    while(s.size() > 1) {
        auto it1 = s.begin();
        auto it2 = std::next(it1);
        add(it1->first + it2->first, 0, it1->second, it2->second);
        s.erase(it1);
        s.erase(it2);
    }
    root = s.begin()->second;
}

void HuffmanEncoder::create_codes(Node* curr, int len, unsigned int val) {
    if(curr == nullptr) return;
    if(curr->left == nullptr && curr->right == nullptr) {
        codes[curr->symbol].val = val;
        codes[curr->symbol].len = len;
        return;
    }
    create_codes(curr->left, len + 1, (val << 1));
    create_codes(curr->right, len + 1, (val << 1) ^ 1);
}

std::vector<uint8_t> HuffmanEncoder::compress() {
    std::vector<uint8_t> res;
    int tot_len = text.size();
    uint8_t* bytes_tot_len = (uint8_t*)&tot_len;
    for(int i = 0; i < 4; i++) {
        res.push_back(bytes_tot_len[i]);
    }
    for(int i = 0; i < 256; i++) {
        uint8_t* bytes_freq = (uint8_t*)&freq[i];
        for(int j = 0; j < 4; j++) {
            res.push_back(bytes_freq[j]);
        }
    }
    uint8_t buff= 0;
    int bit_cnt = 0;
    for(auto ch : text) {
        for(int i = codes[ch].len - 1; i >= 0; i--) {
            buff <<= 1;
            if((1<<i) & codes[ch].val) {
                buff ^= 1;
            }
            bit_cnt++;
            if(bit_cnt == 8) {
                res.push_back(buff);
                buff = 0;
                bit_cnt = 0;
            }
        }
    }
    if(bit_cnt > 0) {
        buff <<= (8 - bit_cnt);
        res.push_back(buff);
    }
    return res;   
}
