// =======================
// 文件: main.cpp
// =======================
#include "HuffmanTree.h"
int main(){
    HuffmanTree tree;
    int op;
    cout<<"1. Compress"<<endl;
    cout<<"2. Decompress"<<endl;
    cin>>op;
    string inputFile;
    string outputFile;
    cout<<"Input Output:"<<endl;
    cin>>inputFile>>outputFile;
    if(op==1){

        tree.compress(
            inputFile,
            outputFile
        );
    }
    else if(op==2){

        tree.decompress(
            inputFile,
            outputFile
        );
    }
    else{
        cout<<"输入错误"<<endl;
    }
    return 0;
}