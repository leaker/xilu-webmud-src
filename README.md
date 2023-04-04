# xilu-webmud-src

西陆版本 WebMUD 仗剑江湖源码 修改文件编码到 UTF8

# 路径文件配置

软件版本
Liunx 6.2
g++ 2.95.2

## 各种文件的设置路径

| 存放文件    | 路径
| ----------- | -----------------------------------------------
| webmud      | /home/mud/bin
| mud121      | /var/www/mud
| login       | /var/www/cgi-bin/mud
| webmud.conf | /var/www/cgi-bin/mud 和/home/mud/bin 下各一个
| register    | /var/www/cgi-bin/mud
| index.html  | /var/www/html/mud/

## 各种目录和文件的属性

* common 目录下
  - quest 目录 里边放的是任务
  - wugong.profile 武功文件，存放武功名称
  - emote.profile 表情文件，存放 chat\*等表示的表情
  - quest.profile 任务文件，存放各 quest 目录下的 npc 的任务的文件的名
  - fifo 权限 777 写入临时文件
  - npc 存放 npc 的属性目录
  - playercount.dat 存放用登陆户的的文件
  - thing 存放物品的目录
  - denied.txt 存放禁止登陆的人名的文件
  - newplayer.profile 存放新玩家数据的文件
* player 存放用户数据的目录
* room 存放房间数据的目录
