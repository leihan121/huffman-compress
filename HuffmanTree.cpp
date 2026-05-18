// =======================
// 文件: HuffmanTree.cpp
// =======================
#include "HuffmanTree.h"
// ================= 构造函数 =================
HuffmanTree::HuffmanTree(){
    root = nullptr;
    freq.resize(256,0);
}
// ================= 统计频率 =================
void HuffmanTree::getFrequency(const string& filename){
    fill(freq.begin(),freq.end(),0);
    ifstream fin(filename,ios::binary);
    if(!fin){
        cout<<"打开文件失败"<<endl;
        return;
    }
    unsigned char ch;
    while(fin.read((char*)&ch,1)){
        freq[ch]++;
    }
    fin.close();
}
// ================= 建树 =================
void HuffmanTree::buildTree(){
    priority_queue<Node*,vector<Node*>,cmp> pq;
    for(int i=0;i<256;i++){

        if(freq[i]>0){
            pq.push(new Node(i,freq[i]));
        }
    }
    if(pq.empty()){
        root = nullptr;
        return;
    }
    // 特殊情况：只有一个字符
    if(pq.size()==1){
        Node* only = pq.top();
        pq.pop();
        Node* parent = new Node(0,only->freq);
        parent->left = only;
        pq.push(parent);
    }
    while(pq.size()>1){
        Node* left = pq.top();
        pq.pop();
        Node* right = pq.top();
        pq.pop();
        Node* parent = new Node(
            0,
            left->freq + right->freq
        );
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }
    root = pq.top();
}
// ================= 建编码 =================
void HuffmanTree::buildCode(
    Node* node,
    string code
){
    if(!node) return;
    // 叶子节点
    if(!node->left && !node->right){
        if(code=="") code="0";
        codes[node->ch] = code;
        return;
    }
    buildCode(node->left,code+"0");
    buildCode(node->right,code+"1");
}
// ================= 压缩 =================
void HuffmanTree::compress(
    const string& input,
    const string& output
){
    getFrequency(input);
    buildTree();
    codes.clear();
    buildCode(root,"");
    ifstream fin(input,ios::binary);
    ofstream fout(output,ios::binary);
    if(!fin || !fout){
        cout<<"Failed to open file"<<endl;
        return;
    }
    // ================= 写频率表 =================
    for(int i=0;i<256;i++){
        fout.write(
            (char*)&freq[i],
            sizeof(int)
        );
    }
    // ================= 写原文件大小 =================
    int total = 0;
    for(int i=0;i<256;i++){
        total += freq[i];
    }
    fout.write(
        (char*)&total,
        sizeof(int)
    );
    // ================= 真正压缩 =================
    unsigned char ch;
    string buffer;
    while(fin.read((char*)&ch,1)){
        buffer += codes[ch];
        while(buffer.size() >= 8){
            string s = buffer.substr(0,8);
            buffer.erase(0,8);
            unsigned char byte = 0;
            for(int i=0;i<8;i++){
                byte <<= 1;
                if(s[i]=='1'){
                    byte |= 1;
                }
            }
            fout.write((char*)&byte,1);
        }
    }
    // ================= 补剩余bit =================
    if(!buffer.empty()){
        while(buffer.size()<8){
            buffer += '0';
        }
        unsigned char byte = 0;
        for(int i=0;i<8;i++){
            byte <<= 1;
            if(buffer[i]=='1'){
                byte |= 1;
            }
        }
        fout.write((char*)&byte,1);
    }
    fin.close();
    fout.close();
    cout<<"Compression complete"<<endl;
}
// ================= 解压 =================
void HuffmanTree::decompress(
    const string& input,
    const string& output
){
    ifstream fin(input,ios::binary);
    ofstream fout(output,ios::binary);
    if(!fin || !fout){
        cout<<"Failed to open file"<<endl;
        return;
    }
    // ================= 读取频率表 =================
    for(int i=0;i<256;i++){
        fin.read(
            (char*)&freq[i],
            sizeof(int)
        );
    }
    // ================= 读取原文件大小 =================
    int total;
    fin.read(
        (char*)&total,
        sizeof(int)
    );
    // ================= 重建哈夫曼树 =================
    buildTree();
    Node* cur = root;
    unsigned char byte;
    int count = 0;
    while(
        fin.read((char*)&byte,1)
        && count < total
    ){
        for(
            int i=7;
            i>=0 && count<total;
            i--
        ){
            int bit = (byte>>i)&1;
            if(bit==0){
                cur = cur->left;
            }
            else{
                cur = cur->right;
            }
            // 到叶子
            if(!cur->left && !cur->right){
                fout.write(
                    (char*)&cur->ch,
                    1
                );
                cur = root;
                count++;
            }
        }
    }
    fin.close();
    fout.close();
    cout<<"Decompression complete"<<endl;
}