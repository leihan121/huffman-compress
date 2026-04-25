#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <queue>
using namespace std;

// ================= 节点 =================
struct Node {
    unsigned char ch;
    int freq;
    Node* left;
    Node* right;

    Node(unsigned char c, int f){
        ch = c;
        freq = f;
        left = nullptr;
        right = nullptr;
    }
};

// ================= 小根堆 =================
struct cmp {
    bool operator()(Node* a, Node* b){
        return a->freq > b->freq;
    }
};

// ================= 统计频率 =================
vector<int> getFrequency(const string& filename){
    vector<int> freq(256,0);
    ifstream fin(filename, ios::binary);

    if(!fin){
        cout<<"打开文件失败!"<<endl;
        return freq;
    }

    unsigned char ch;
    while(fin.read((char*)&ch,1)){
        freq[ch]++;
    }

    fin.close();
    return freq;
}

// ================= 建树 =================
Node* buildTree(const vector<int>& freq){
    priority_queue<Node*, vector<Node*>, cmp> pq;

    for(int i=0;i<256;i++){
        if(freq[i]>0)
            pq.push(new Node(i,freq[i]));
    }

    if(pq.empty()) return nullptr;

    // 单字符特殊处理
    if(pq.size()==1){
        Node* only = pq.top(); pq.pop();
        Node* parent = new Node(0,only->freq);
        parent->left = only;
        pq.push(parent);
    }

    while(pq.size()>1){
        Node* l = pq.top(); pq.pop();
        Node* r = pq.top(); pq.pop();

        Node* p = new Node(0,l->freq+r->freq);
        p->left = l;
        p->right = r;

        pq.push(p);
    }

    return pq.top();
}

// ================= 生成编码 =================
void buildCode(Node* root,string code,
               unordered_map<unsigned char,string>& codes){

    if(!root) return;

    if(!root->left && !root->right){
        if(code=="") code="0";
        codes[root->ch]=code;
        return;
    }

    buildCode(root->left,code+"0",codes);
    buildCode(root->right,code+"1",codes);
}

// ================= 压缩 =================
void encodeFile(const string& input,
                const string& output,
                unordered_map<unsigned char,string>& codes,
                vector<int>& freq){

    ifstream fin(input,ios::binary);
    ofstream fout(output,ios::binary);

    if(!fin || !fout){
        cout<<"文件打开失败"<<endl;
        return;
    }

    // 写频率表
    for(int i=0;i<256;i++){
        fout.write((char*)&freq[i],sizeof(int));
    }

    // 写原始字符数
    int total=0;
    for(int i=0;i<256;i++) total+=freq[i];
    fout.write((char*)&total,sizeof(int));

    unsigned char ch;
    string buffer;

    while(fin.read((char*)&ch,1)){
        buffer+=codes[ch];

        while(buffer.size()>=8){
            string s=buffer.substr(0,8);
            buffer.erase(0,8);

            unsigned char byte=0;
            for(int i=0;i<8;i++){
                byte<<=1;
                if(s[i]=='1') byte|=1;
            }

            fout.write((char*)&byte,1);
        }
    }

    // 处理剩余
    if(!buffer.empty()){
        while(buffer.size()<8) buffer+='0';

        unsigned char byte=0;
        for(int i=0;i<8;i++){
            byte<<=1;
            if(buffer[i]=='1') byte|=1;
        }

        fout.write((char*)&byte,1);
    }

    fin.close();
    fout.close();

    cout<<"压缩完成"<<endl;
}

// ================= 解压 =================
void decodeFile(const string& input,const string& output){
    ifstream fin(input,ios::binary);
    ofstream fout(output,ios::binary);

    if(!fin || !fout){
        cout<<"文件打开失败"<<endl;
        return;
    }

    vector<int> freq(256);

    // 读频率表
    for(int i=0;i<256;i++){
        fin.read((char*)&freq[i],sizeof(int));
    }

    // 读原始长度
    int total;
    fin.read((char*)&total,sizeof(int));

    Node* root = buildTree(freq);
    Node* cur = root;

    unsigned char byte;
    int count = 0;

    while(fin.read((char*)&byte,1) && count < total){
        for(int i=7;i>=0 && count < total;i--){
            int bit = (byte>>i)&1;

            if(bit==0) cur=cur->left;
            else cur=cur->right;

            if(!cur->left && !cur->right){
                fout.write((char*)&cur->ch,1);
                cur=root;
                count++;
            }
        }
    }

    fin.close();
    fout.close();

    cout<<"解压完成"<<endl;
}

// ================= 主函数 =================
int main(){
    int op;
    cout<<"1. 压缩\n2. 解压\n";
    cin>>op;

    string in,out;
    cout<<"输入文件 输出文件:\n";
    cin>>in>>out;

    if(op==1){
        vector<int> freq = getFrequency(in);
        Node* root = buildTree(freq);

        unordered_map<unsigned char,string> codes;
        buildCode(root,"",codes);

        encodeFile(in,out,codes,freq);
    }
    else if(op==2){
        decodeFile(in,out);
    }

    return 0;
}
