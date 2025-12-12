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