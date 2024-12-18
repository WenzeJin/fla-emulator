# fla: Formal Language and Automata, emulating PDAs and TMs

本项目为2024年秋季《形式语言与自动机》课程的课程项目，旨在开发一个命令行工具`fla`，用于模拟PDA和TM的行为，判断某个串是否被某个PDA或TM接受。

## 如何构建

### 环境要求

- CMake 3.15.0 以上版本
- 支持 C++14 的编译器

### 编译

```bash
./build.sh
```

## 如何使用

目前已经支持模拟 PDA。

```
Usage: fla [-h|--help] [-v|--verbose] <automata_file> <input_str>
```

- `automata_file_dir`：PDA 或 TM 的描述文件，后缀名为`.pda`或`.tm`
- `input_str`：待判断的字符串
- `-v|--verbose`：输出详细的运行信息，包括每一步的状态转移，详细的错误信息等

一般情况下的输出：

__for PDA__: `true` 或 `false`, 或错误信息 `syntax error` （PDA描述文件语法或语义有误）`illegal input`（输入串不合法）