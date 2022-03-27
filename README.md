# RabbitEngine
Design custom game engine



# Multiple gits push -f method
ssh-agent bash

ssh-add ~/.ssh/id_rsa

ssh-add ~/.ssh/id_rsa_github

ssh-add -l

ssh -T git@githu.com

git push -f



# 多git配置

#github user

Host github.com
    Hostname github.com
    User xxx
    IdentityFile C:\Users\aaa\.ssh\id_rsa_github   // windows下需要用绝对地址
    PreferredAuthentications publickey

#gitlab user
Host gitlab.com
    Hostname gitlab.com
    User xxx
    IdentityFile C:\Users\aaa\.ssh\id_rsa
    PreferredAuthentications publickey
