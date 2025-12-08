🔍 查看分支列表
查看本地分支：
git branch
查看远程分支：
git branch -r

🧹 删除本地分支
git branch -d 分支名

修改本地分支名称
git branch -M name

🧩 创建本地分支
创建分支但不切换过去
git branch dev


🌐 创建远程分支
git push origin dev

CMake生成 Debug 版本
在命令行里重新配置 CMake，加上 -DCMAKE_BUILD_TYPE=Debug：

powershell
cmake .. -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_BUILD_TYPE=Debug
cmake --build .
这样编译出来的 myapp.exe 就包含调试符号，gdb 或 VSCode 调试器才能识别断点。
