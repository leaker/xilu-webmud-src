#class=YNPC
begin of YPropertyGroup
string values:
名字=慧空尊者
性别=男
门派=少林派
头衔=少林寺第三十八代弟子
描述=他是一位两鬓斑白的老僧，身穿一袭青布镶边袈裟。他身材略高，太阳穴微凸，双目炯炯有神。
所会技能=基本招架 基本内功 基本轻功 基本手法 基本棍法 读书写字 易筋经 无常杖
拜师_条件一=门派 字符串 等于 少林派 慧空尊者对&s说道：阁下非我少林弟子，此话真是无从说起。
拜师_条件二=易筋经 数值 大于 49 慧空尊者对&s说道：你易筋经不够娴熟，还不能修习更高深的功夫。
同意拜师的回答=慧空尊者对&s合十道：阿弥陀佛，如此贫僧便收下你了。
武器=禅杖
衣服=青布镶边袈裟
武器类型=棍
轻功_使用=少林身法
棍_攻击武功=无常杖
long values:
辈分=38
年龄=52
容貌=50
衣服防御力=5
收徒=1
拜师_条件个数=2
气血=1000
最大气血=1000
固定攻击力=3000
固定防御力=2800
重量=100
附加防御力=5
经验=70000
后天臂力=37
后天根骨=37
后天身法=37
基本招架=70
基本手法=70
基本内功=70
基本轻功=70
基本棍法=70
无常杖=70
读书写字=70
易筋经=70
少林身法=70
float values:
end of YPropertyGroup
begin of YObjectGroup
object=thing/禅杖
object=thing/青布镶边袈裟
end of YObjectGroup