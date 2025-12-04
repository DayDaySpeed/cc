🔍 查看分支列表
查看本地分支：

bash
git branch
查看远程分支：

bash
git branch -r

🧹 删除本地分支
bash
git branch -d 分支名



🧩 创建本地分支
创建分支但不切换过去

git branch dev


🌐 创建远程分支
远程分支其实是你把本地分支推送到远程仓库：
git push origin dev
这样远程仓库（比如 GitHub）就会出现一个 dev 分支。