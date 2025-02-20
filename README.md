# fla: Formal Language and Automata, emulating PDAs and TMs

本项目为2024年秋季南京大学《形式语言与自动机》课程的课程项目，旨在开发一个命令行工具`fla`，用于模拟PDA和TM的行为，判断某个串是否被某个PDA或TM接受。

本项目将 PDA 和 TM 建模为 `Parser` -> `Context` -> `Emulator` 的结构，其中：
- `Parser` 负责解析 PDA 或 TM 的描述文件，将其转化为内部数据结构，
- `Context` 负责维护 PDA 或 TM 的运行时上下文，包括状态定义、字母表定义、转移函数定义等，
- `Emulator` 负责模拟 PDA 或 TM 的运行过程，判断某个串是否被接受或模拟图灵机的运行过程。

## 如何构建

### 环境要求

- CMake 3.15.0 以上版本
- 支持 C++14 的编译器

### 编译

```bash
./build.sh
```

或

1. 将所有`.h` 以及`.cpp`文件置于`/fla-project`文件夹下，或者将该框架的`CMakeLists.txt`复制至你的工程根目录
2. 在工程根目录处，使用指令`cmake -B build`
3. 在工程根目录处，使用指令`cd ./build; make`

## 如何使用

```
Usage: fla [-h|--help] [-v|--verbose] <automata_file> <input_str>
```

- `automata_file_dir`：PDA 或 TM 的描述文件，后缀名为`.pda`或`.tm`
- `input_str`：待判断的字符串
- `-v|--verbose`：输出详细的运行信息，包括每一步的状态转移，详细的错误信息等

一般情况下的输出：

__for PDA__: `true` 或 `false`, 或错误信息 `syntax error` （PDA描述文件语法或语义有误）`illegal input`（输入串不合法）

__for TM__: Tape0 上的最后有效内容，或错误信息 `syntax error` （TM描述文件语法或语义有误）`illegal input`（输入串不合法）

## 如何测试

参见 `test` 文件夹下的测试脚本及对应的 README.md