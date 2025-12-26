### 删除本地分支
查看当前分支

```bash
git branch
确认你要删除的分支不是当前正在使用的分支。
```

### 删除本地分支
```bash
普通删除（如果分支已合并到主分支）：

git branch -d branch_name
强制删除（即使未合并）：

git branch -D branch_name
```

### 创建远程分支
远程分支本质上是你把本地分支推送到远程仓库。

创建本地分支（如果还没有）：

```bash
git checkout -b branch_name
推送到远程仓库并建立追踪关系：

git push -u origin branch_name
origin 是远程仓库的默认名字。

-u 表示建立本地分支与远程分支的关联，之后可以直接用 git push 和 git pull。
```

```bash
确认远程分支是否创建成功：

git branch -r
或者在远程仓库（如 GitHub/GitLab）查看。
```

### 一次性移除所有已跟踪但在 .gitignore 中的文件

```bash
git rm -r --cached .
git add .
git commit -m "Apply .gitignore and remove tracked files"
```