// =======================
// 文件: HuffmanTree.h
// =======================

#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <fstream>

using namespace std;

// ================= 节点 =================
struct Node{

    unsigned char ch;
    int freq;

    Node* left;
    Node* right;

    Node(unsigned char c,int f){

        ch = c;
        freq = f;

        left = nullptr;
        right = nullptr;
    }
};

// ================= 小根堆 =================
struct cmp{

    bool operator()(Node* a,Node* b){

        return a->freq > b->freq;
    }
};

// ================= 哈夫曼树类 =================
class HuffmanTree{

private:

    Node* root;

    vector<int> freq;

    unordered_map<unsigned char,string> codes;

public:

    HuffmanTree();

    void getFrequency(const string& filename);

    void buildTree();

    void buildCode(Node* node,string code);

    void compress(
        const string& input,
        const string& output
    );

    void decompress(
        const string& input,
        const string& output
    );
};

#endif