#### 合并分支

```bash
切换到目标分支
git checkout main
```

```bash
执行合并命令
git merge feature
```
##### 冲突
可能出现三种情况：

1. Fast-forward 合并：如果 main 没有新的提交，Git 会直接把指针移动到 feature，没有冲突。

2. 自动合并成功：Git 会生成一个新的合并提交（merge commit）。

3. 冲突 (conflict)：如果两个分支修改了同一个文件的同一部分，Git 无法自动决定，就会提示你手动解决。

#### 构建makefile文件，并携带调试信息
```bash
cmake .. -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_BUILD_TYPE=Debug
```
#### 使用makefile文件生成可执行文件
```bash
cmake --build .
```
### 运行 
```powershell
cd build/build-mingw
.\syntax.exe test.txt
```