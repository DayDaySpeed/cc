### 查看分支列表
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

### CMake
cmake .. -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_BUILD_TYPE=Debug

cmake --build .

