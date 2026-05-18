# Huffman Coding – 哈夫曼压缩/解压工具

基于哈夫曼树（Huffman Tree）实现的无损数据压缩与解压工具，支持任意二进制文件（文本、图像、可执行文件等）的压缩与还原。

## ✨ 特性

- 无损压缩与精确解压
- 自动统计字符频率并构建最优前缀编码
- 压缩文件中内置频率表，无需外部存储编码信息
- 支持处理单个字符文件或空文件
- 使用 C++ 标准库实现，无第三方依赖

## 📁 文件结构
.
├── HuffmanTree.h # 哈夫曼树类声明
├── HuffmanTree.cpp # 哈夫曼树实现
├── main.cpp # 命令行交互入口
└── README.md

## 🔧 编译

使用任意 C++11（或更高版本）编译器：

```bash
g++ -std=c++11 -o huffman main.cpp HuffmanTree.cpp
或使用 clang++：
clang++ -std=c++11 -o huffman main.cpp HuffmanTree.cpp
🚀 使用方法
1. 压缩文件
bash
./huffman
按提示选择 1（Compress），然后输入：

text
Input Output:
原始文件.txt 压缩文件.huff
示例：

text
1. Compress
2. Decompress
1
Input Output:
test.txt test.huff
输出：

text
Compression complete
2. 解压文件
bash
./huffman
选择 2（Decompress），输入：

text
Input Output:
压缩文件.huff 解压后.txt
示例：

text
1. Compress
2. Decompress
2
Input Output:
test.huff output.txt
输出：

text
Decompression complete
📦 压缩文件格式说明
压缩后的 .huff 文件结构如下：

内容	大小
频率表（256个int）	256 × 4 = 1024 字节
原始文件总字节数	4 字节
压缩后的比特流	变长（补零到整字节）
解压时会先读取频率表 → 重建哈夫曼树 → 按比特流解码 → 还原原始文件。
⚠️ 注意事项
内存管理
当前版本使用 new 动态分配哈夫曼树节点，程序退出后操作系统会回收内存。
若需长期运行或多次压缩/解压，建议添加析构函数释放树节点。

跨平台兼容性
频率表直接以 int 的二进制形式写入，不同平台（x86 / ARM / 大端机）之间交换压缩文件可能出错。
若需严格跨平台，可将频率表转换为网络字节序或使用文本格式存储。

压缩率

对文本文件（尤其是英文）压缩效果较好（20%～50% 压缩率）。

对已压缩文件（如 .jpg、.mp4、.zip）压缩率极低甚至文件变大。

对极短文件（< 200 字节）可能因频率表开销导致“负压缩”。

解压完整性
程序依赖压缩文件中的“原始文件总字节数”字段控制解压终止，即使末尾存在额外填充比特也不会解出多余数据。

🧪 测试示例
使用提供的 test.txt（包含中英文、符号、重复文本）测试：

bash
g++ -std=c++11 -o huffman main.cpp HuffmanTree.cpp
./huffman
1
test.txt test.huff

./huffman
2
test.huff output.txt

diff test.txt output.txt   # 在 Linux/macOS 下验证
若无输出，表示两文件完全相同，压缩解压成功。

📚 算法简介
建树：使用最小堆（优先队列）反复合并频率最小的两个节点，构建哈夫曼树。

建码：从根节点 DFS，左分支 '0'，右分支 '1'，叶子节点存储编码。

压缩：将每个字符替换为对应比特编码，按 8 比特一组写入文件。

解压：逐比特沿树向下走，到达叶子时输出字符并返回根节点。
