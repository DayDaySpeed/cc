### *查看分支列表*
```bash
#### 查看本地分支：
git branch
#### 查看远程分支：
git branch -r

#### 删除本地分支
git branch -d 分支名

#### 修改本地分支名称
git branch -M name

#### 创建本地分支(创建分支但不切换过去)
git branch dev

#### 创建远程分支
git push origin dev
```
### *合并分支*
```bash
#### 1.先切换到目标分支
git checkout main
#### 或者在新版本 Git 中：
git switch main
#### 2.执行合并命令：
git merge feature
```


### CMake
```bash
cmake .. -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_BUILD_TYPE=Debug
```
```bash
cmake --build .
```

### 问题
1. 符号表：对同名变量用history保存全局变量，函数结束后取出history恢复。此实现不好，identifier的每个字段都应该有意义，而不是用来保存

2. 定义的int类型，必须和指针位数一样，对数据类型的处理不完善

3. 采用自顶而下的递归下降，向前看一个token，太简陋了

4. 不是编译器，是解释器
