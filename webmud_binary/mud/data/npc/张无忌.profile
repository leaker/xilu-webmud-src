#class=YNPC
begin of YPropertyGroup
string values:
名字=张无忌
性别=男
门派=明教
头衔=明教第三十四代教主
描述=他看起来二十来岁，是明教的第三十四代教主。脸上带着英资勃发的笑容。真是少年有为，当年光明顶一战成名，一人尽伏天下各门英豪。
所会技能=基本内功 基本招架 基本轻功 基本掌法 基本刀法 基本剑法 基本拳法 读书写字 圣火神功 梯云纵 青蝠身法 寒冰棉掌 光明刀法 圣火令法 七伤拳 太极拳 太极剑
拜师_条件一=门派 字符串 等于 明教 张无忌对&s说到：你非我明教兄弟，阁下请回吧。
拜师_条件二=圣火神功 数值 大于 149 张无忌对&s说道：你的内功修为还不够深厚，还需多加领悟。
拜师_条件三=正气值 数值 大于 299999 张无忌对&s说道：我明教于外人积怨颇深，我们当多行侠义，减少误解才是。
同意拜师的回答=张无忌对&s点点头说：你可要记住，为人不可作恶，世人误会我教已久，你凡事要作到无愧于心就好。
武器=长剑
衣服=白色圣衣
武器类型=剑
被看=张无忌对你憨厚的笑了笑。说到；你可知道太师傅他老人家最近可好。
梯云纵_条件一=小于 妖狼 张无忌对&s笑笑不语。
太极拳_条件一=小于 妖狼 张无忌对&s笑笑不语。
太极剑_条件一=小于 妖狼 张无忌对&s笑笑不语。
轻功_使用=梯云纵
刀_攻击武功=圣火令法
空手_攻击武功=太极拳
剑_攻击武功=太极剑
long values:
辈分=34
年龄=22
容貌=70
重量=100
经验=2000000
正气值=300000
梯云纵_条件个数=1
太极拳_条件个数=1
太极剑_条件个数=1
收徒=1
拜师_条件个数=3

气血=4000
最大气血=4000
固定攻击力=65000
固定防御力=62000
武器攻击力=10

后天臂力=55
后天身法=55
后天根骨=55

基本掌法=280
基本拳法=280
基本招架=280
基本剑法=280
基本刀法=280
基本内功=280
基本轻功=280
寒冰棉掌=280
读书写字=200
七伤拳=280
光明刀法=280
圣火令法=280
太极拳=280
太极剑=280
梯云纵=280
圣火神功=280
青蝠身法=280
float values:
end of YPropertyGroup
begin of YObjectGroup
object=thing/长剑
object=thing/白色圣衣
end of YObjectGroup