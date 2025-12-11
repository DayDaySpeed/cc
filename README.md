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
