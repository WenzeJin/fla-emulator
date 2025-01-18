# 自动机实验报告

> 221220028 金文泽

## 实验完成情况

1. 实现了 PDA 和 TM 的解析器/模拟器
2. 为 PDA 和 TM 均实现了对应的 verbose 模式 __bonus__
3. 完成了 PDA 和 TM 的用例程序 `pda/case.py` 和 `tm/case1.py` `tm/case2.py`
4. 能够检测以下三种错误：
   1. PDA TM 文件语法错误，例如该有括号的地方没有，使用了不合法的字符等。程序中体现为`AutomataSyntaxException`。最终输出 `syntax error`，且在 verbose 模式下输出更多错误信息。__bonus__
   2. PDA TM 文件逻辑（语义）错误，例如状态转移函数中的状态不存在，状态转移函数中的符号不存在等。程序中体现为`AutomataStructureException`。最终输出 `syntax error` （因为此部分实验中未要求，所以同样输出，其实并不合适）。__bonus__
   3. 非法的输入串，输入串中有符号未在字母表中，程序中体现为 `InputSyntaxError`，最终输出 `illegal input`。
5. TM 支持一些更高级的功能，比如如果有完全确定的状态转移函数，那么可以直接使用，而不会使用可能存在通配符的状态转移函数。

## 设计思路

PDA 和 TM 的思路类似，总体上均包含以下几个模块（分别用 PDA/TM 作为前缀）：

1. `Context`: 用于存储自动机运行的必要信息，具体而言，就是那个定义中的多元组。同时，`Context`利用一个 unordered_map 来存储状态转移函数，这样可以在 O(1) 的时间复杂度内找到对应的状态转移函数。为了能够使用 STL 的 unordered_map，设计了键值类 `TransitionKey` 和 `TransitionValue`，分别用于作为 unordered_map 的键和值。同时由于只能使用 C++14，所以没有办法使用 optinal，所以自定义了 `QueryResult` 类，用于表示可能失败的查询结果。
2. `Parser`: 用于解析 PDA/TM 文件，将其转换为 `Context` 对象。具体而言，`Parser` 会读取文件，进行语法和语义检查，然后将其转换为 `Context` 对象。在转换过程中，`Parser` 会检查语法错误，而不会检查语义错误。
3. `Emulator`: 用于模拟 PDA/TM 的运行。具体而言，`Emulator` 会接受一个输入串，然后根据状态转移函数进行状态转移，直到达到终止状态。在模拟过程中，`Emulator` 会检查输入串是否合法，即输入串中的符号是否在字母表中。`Emulator` 同时会在运行前检查自动机是否合法，也就是语义错误。PDA 的模拟器运用一个栈来模拟栈操作，而 TM 的模拟器则使用自定义的 `TMTape` 类来模拟磁带操作。

## 部分结果展示

### PDA

```shell
$ ./bin/fla pda/anbn.pda aabbb 
false

$ ./bin/fla -v pda/anbn.pda aabbb
Input: aabbb
==================== RUN ====================
Step : 0
State: q0
Stack: z ||(Stack Bottom)
---------------------------------------------
Step : 1
State: q1
Stack: 1 z ||(Stack Bottom)
---------------------------------------------
Step : 2
State: q1
Stack: 1 1 z ||(Stack Bottom)
---------------------------------------------
Step : 3
State: q2
Stack: 1 z ||(Stack Bottom)
---------------------------------------------
Step : 4
State: q2
Stack: z ||(Stack Bottom)
---------------------------------------------
Step : 5
State: accept
Stack: ||(Stack Bottom)
---------------------------------------------
Result: false
==================== END ====================

$ ./bin/fla -v pda/anbn.pda aabbc
Input: aabbc
==================== ERR ====================
error: 'c' was not declared in the set of input symbols
Input: aabbc
           ^
==================== END ====================

$ ./bin/fla pda/anbn.pda aabbc
illegal input

$ ./bin/fla pda/anbn.pda aabb
true

$ ./bin/fla pda/test.pda aabb # 一个有语法错误的 PDA 文件
syntax error

$ ./bin/fla -v pda/test.pda aabb
syntax error
Syntax error at line 4: #Q = {q0,q1,q2,accept
Location: {q0,q1,q2,accept
Description: Expected {}.
```

### TM

```shell
$ ./bin/fla -v tm/palindrome_detector_2tapes.tm 10001B10001
Input: 10001B10001
==================== ERR ====================
error: 'B' was not declared in the set of input symbols
Input: 10001B10001
            ^
==================== END ====================

$ ./bin/fla tm/palindrome_detector_2tapes.tm 10001B1001
illegal input

$ ./bin/fla -v tm/palindrome_detector_2tapes.tm 1001001
Input: 1001001
==================== RUN ====================
Step   : 0
State  : 0
Index0 : 0 1 2 3 4 5 6 
Tape0  : 1 0 0 1 0 0 1 
Head0  : ^             
Index1 : 0 
Tape1  : _ 
Head1  : ^ 
---------------------------------------------
Step   : 1
State  : cp
Index0 : 0 1 2 3 4 5 6 
Tape0  : 1 0 0 1 0 0 1 
Head0  : ^             
Index1 : 0 
Tape1  : _ 
Head1  : ^ 
---------------------------------------------
Step   : 2
State  : cp
Index0 : 0 1 2 3 4 5 6 
Tape0  : 1 0 0 1 0 0 1 
Head0  :   ^           
Index1 : 0 1 
Tape1  : 1 _ 
Head1  :   ^ 
---------------------------------------------
Step   : 3
State  : cp
Index0 : 0 1 2 3 4 5 6 
Tape0  : 1 0 0 1 0 0 1 
Head0  :     ^         
Index1 : 0 1 2 
Tape1  : 1 0 _ 
Head1  :     ^ 
---------------------------------------------
Step   : 4
State  : cp
Index0 : 0 1 2 3 4 5 6 
Tape0  : 1 0 0 1 0 0 1 
Head0  :       ^       
Index1 : 0 1 2 3 
Tape1  : 1 0 0 _ 
Head1  :       ^ 
---------------------------------------------
Step   : 5
State  : cp
Index0 : 0 1 2 3 4 5 6 
Tape0  : 1 0 0 1 0 0 1 
Head0  :         ^     
Index1 : 0 1 2 3 4 
Tape1  : 1 0 0 1 _ 
Head1  :         ^ 
---------------------------------------------
······
---------------------------------------------
Step   : 28
State  : accept4
Index0 : 7 8 9 10 
Tape0  : t r u _  
Head0  :       ^  
Index1 : 1 
Tape1  : _ 
Head1  : ^ 
---------------------------------------------
Step   : 29
State  : halt_accept
Index0 : 7 8 9 10 
Tape0  : t r u e  
Head0  :       ^  
Index1 : 1 
Tape1  : _ 
Head1  : ^ 
---------------------------------------------
Result: true
==================== END ====================

$ ./bin/fla tm/palindrome_detector_2tapes.tm 1001001
true

$ ./bin/fla tm/palindrome_detector_2tapes.tm 100100
false
```

## 总结与感想

总体而言，这个实验还是比较简单的。可以感受到因为 PDA TM 的定义比较简洁，带来的结果是编程非常受限，但是模拟他们却非常好实现。实验的难点在于实现状态转移函数，其他则是按定义编写就可以了。这个实验即让我感受到了自动机的强大，也让我感受到了自动机的局限性。自动机的定义非常简洁，但是却能够解决很多问题，这是非常有趣的。
