#class=YNPC
begin of YPropertyGroup
string values:
名字=澄灭罗汉
性别=男
门派=少林派
头衔=少林寺第三十七代弟子
绰号=妙音罗汉
描述=他是一位须发花白的老僧，身穿一袭金边黑布袈裟。他身材瘦高，太阳穴高高鼓起，似乎身怀绝世武功。
所会技能=基本招架 基本内功 基本轻功 基本手法 基本棍法 读书写字 易筋经 少林身法 无常杖
拜师_条件一=门派 字符串 等于 少林派 澄灭罗汉对&s说道：阁下非我少林弟子，此话真是无从说起。
拜师_条件二=易筋经 数值 大于 69 澄灭罗汉对&s说道：你易筋经不够娴熟，还不能修习更高深的功夫。
同意拜师的回答=澄灭罗汉对&s合十道：阿弥陀佛，如此贫僧便收下你了。
武器=禅杖
武器类型=棍
轻功_使用=少林身法
棍_攻击武功=无常杖
long values:
辈分=37
年龄=62
容貌=51
衣服防御力=5
收徒=1
拜师_条件个数=2
气血=1500
最大气血=1500
固定攻击力=6000
固定防御力=5800
重量=100
附加防御力=5
经验=100000
后天臂力=40
后天根骨=40
后天身法=40
基本招架=100
基本内功=100
基本轻功=100
基本手法=100
基本棍法=100
无常杖=100
读书写字=100
易筋经=100
少林身法=100
float values:
end of YPropertyGroup
begin of YObjectGroup
object=thing/禅杖
object=thing/金边黑布袈裟
end of YObjectGroup