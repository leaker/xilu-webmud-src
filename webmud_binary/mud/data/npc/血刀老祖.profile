#class=YNPC
begin of YPropertyGroup
string values:
名字=血刀老祖
性别=男
门派=血刀门
头衔=血刀门第四代掌教
描述=他身穿黄袍，年纪极老，尖头削耳，脸上都是皱纹。
所会技能=基本招架 基本内功 基本轻功 基本拳法 基本刀法 读书写字 血刀心法 血刀刀法 鬼魅身法 无影神拳
轻功_使用=鬼魅身法
空手_攻击武功=无影神拳
刀_攻击武功=血刀刀法
拜师_条件一=门派 字符串 等于 血刀门 血刀老祖对&s说道：佛爷今天没兴趣理你！
拜师_条件二=血刀心法 数值 大于 149 血刀老祖不耐烦的挥了挥手对&s说道：再去打打内功根基再来！
拜师_条件三=正气值 数值 小于 -299999 血刀老祖对&s说道：我看你不够心狠手辣，怎入得我血刀门下！
拜师_条件四=性别 字符串 不等于 女 血刀老祖奸笑着对&s说道：我血刀从不收女人，不过小娘子到是可以和我共参欢喜禅！
同意拜师的回答=血刀老祖左右看了看&s，说道：佛爷今天心情好，就收下你了！
衣服=血僧袍
武器=血刀
武器类型=刀
血刀刀法_条件一=小于 血刀心法 血刀老祖说道: 本门刀法必须和本门内功相辅相对，不可贪进。
long values:
辈分=4
年龄=50
容貌=50
重量=100
经验=6000000
正气值=-300000
附加防御力=5
武器攻击力=60
收徒=1
拜师_条件个数=4
血刀刀法_条件个数=1
后天臂力=55
后天根骨=55
后天身法=55
气血=4000
最大气血=4000
固定攻击力=65000
固定防御力=62000

基本内功=280
基本招架=280
基本轻功=280
基本拳法=280
基本刀法=280
读书写字=200
血刀心法=280
血刀刀法=280
鬼魅身法=280
无影神拳=280
float values:
end of YPropertyGroup
begin of YObjectGroup
object=thing/血刀
object=thing/金子 3
object=thing/血僧袍
end of YObjectGroup