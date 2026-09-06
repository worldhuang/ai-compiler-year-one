# W02-D06：ASan / UBSan 越界检测

## 今日目标

开启 AddressSanitizer（ASan）和 UndefinedBehaviorSanitizer（UBSan），
制造一次堆越界访问，并验证 MiniTensor 的正常测试在 Sanitizer 下不报错。

## 环境

- 日期：2026-09-05
- 编译器：<填写 g++ --version 的结果>
- CMake：<填写 cmake --version 的结果>
- 构建类型：Debug
- Sanitizer：AddressSanitizer + UndefinedBehaviorSanitizer

## 故意制造的错误

实验文件：

```text
labs/W02-D06/out_of_bounds.cpp

```CPP
int main() {
    int* values = new int[2];

    values[0] = 10;
    values[1] = 20;
    values[2] = 30;  // 故意越界：合法下标只有 0、1

    delete[] values;
    return 0;
}


编译:
```bash
mkdir -p build-asan

c++ -std=c++20 -O0 -g \
    -fsanitize=address,undefined \#野指针/未定义
    -fno-omit-frame-pointer \#保留栈帧，**崩溃时打印完整调用栈**，方便定位源码
    labs/W02-D06/out_of_bounds.cpp \
    -o build-asan/out_of_bounds

./build-asan/out_of_bounds


用 Sanitizer 跑正常项目:
```bash

#1 配置，生成asan构建目录 build‑asan
cmake -S . -B build-asan \
    -DCMAKE_BUILD_TYPE=Debug \
    -DMINITENSOR_ENABLE_SANITIZERS=ON

#2 编译全部目标
cmake --build build-asan

#3 -N = Dry run，只列出所有测试，不执行
ctest --test-dir build-asan -N

#4 设置ASan运行时选项，执行测试
ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 \
ctest --test-dir build-asan --output-on-failure
#- `halt_on_error=1`：遇到越界 / 野指针直接 abort，**整个#测试失败，CI 可以拦截**
#- 如果`halt_on_error=0`，ASan 打印告警但程序继续往下跑，容易掩盖 bug，**测试不会失败，不适合门禁**



#git 处理

问题
```text
hsj@power360:~/ai-compiler-year-one$ git add .
warning: adding embedded git repository: projects/minitensor/build-asan/_deps/googletest-src

`build‑asan/_deps/googletest-src`**本身是一个独立 git 仓库**（里面有`.git`文件夹）。
你执行`git add .`，试图把**另一个 git 仓库嵌套提交到主仓库**，Git 弹出警告：
> 不要直接把内嵌仓库提交进去！克隆你的项目时不会拉取这个子仓库代码

解决方法：
build*/
#在仓库根目录编辑 `.gitignore`，增加一行

git rm --cached -r projects/minitensor/build-asan
#把已经暂存的`build‑asan`从暂存区撤掉（**不会删除本地文**）
#`--cached`：只从 git 索引移除，磁盘上文件夹保留

git status
#此时`build‑asan`不再出现在待提交列表

git ls-files
#作用：列出**已经加入暂存 / 已经提交**、Git 正在跟踪的全部文件。