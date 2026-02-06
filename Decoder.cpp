#include <iostream>
#include <set>
#include <utility>
#include <vector>
#include <cstdint>
#include "Decoder.h"

void HuffmanDecoder::add(unsigned int freq, uint8_t symbol, Node* left, Node* right) {
    Node* curr = new Node();
    curr->symbol = symbol;
    curr->left = left;
    curr->right = right;
    s.insert({freq, curr});
}

void HuffmanDecoder::remTree(Node* curr) {
    if(curr == nullptr) return;
    remTree(curr->left);
    remTree(curr->right);
    delete curr;
}

void HuffmanDecoder::buildTree() {
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

void HuffmanDecoder::readData() {
    uint8_t* bytes_tot_len = (uint8_t*)&tot_len;
    int wsk = 0;
    for(int i = 0; i < 4; i++) {
        bytes_tot_len[i] = compressed[wsk++];
    }
    for(int i = 0; i < 256; i++) {
        uint8_t* bytes_freq = (uint8_t*)&freq[i];
        for(int j = 0; j < 4; j++) {
            bytes_freq[j] = compressed[wsk++];
        }
    }
}

HuffmanDecoder::HuffmanDecoder(std::vector<uint8_t> inp) {
    compressed = inp;
    root = nullptr;
    tot_len = 0;
    for(int i = 0; i < 256; i++) {
        freq[i] = 0;
    }
}

HuffmanDecoder::~HuffmanDecoder() {
    remTree(root);
}

std::vector<uint8_t> HuffmanDecoder::decompress() {
    readData();
    buildTree();
    std::vector<uint8_t> res;
    Node* curr = root;
    int rem = 0;
    for(int i = 4 + 256 * 4; i < compressed.size(); i++) {
        for(int j = 7; j >= 0; j--) {
            if(compressed[i] & (1<<j)) {
                curr = curr->right;
            }else {
                curr = curr->left;
            }
            if(curr->left == nullptr && curr->right == nullptr) {
                res.push_back(curr->symbol);
                curr = root;
                rem++;
                if(rem == tot_len) {
                    return res;
                }
            }
        }
    }
    return res;
}