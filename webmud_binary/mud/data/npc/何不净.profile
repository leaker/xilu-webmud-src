#class=YNPC
begin of YPropertyGroup
string values:
名字=何不净
性别=男
门派=丐帮
头衔=丐帮第二十代弟子
描述=他是丐帮七袋弟子。
武器=铁杖
衣服=破衣服
武器类型=棍
所会技能=基本内功 基本轻功 基本掌法 基本棍法 基本招架 读书写字 混天气功 逍遥游 打狗棒 降龙十八掌 疯魔杖
棍_攻击武功=疯魔杖
轻功_使用=逍遥游
拜师_条件一=门派 字符串 等于 丐帮 何不净对&s说道:既然已有名师指点，何必又来拜老叫化呢？
拜师_条件二=混天气功 数值 大于 49 何不净对&s说道：我看你内功还不扎实，如何行走得了四方！回去多练练吧！
同意拜师的回答=何不净对&s说道:好吧，我就收下你了。
long values:
收徒=1
拜师_条件个数=2
辈分=20
年龄=25
容貌=70

气血=1500
最大气血=1500
固定攻击力=6000
固定防御力=3600
武器攻击力=10
后天臂力=40
后天身法=40
后天根骨=40
正气值=5000

读书写字=100
基本掌法=100
基本招架=100
基本内功=100
基本轻功=100
基本棍法=100
疯魔杖=100
降龙十八掌=50
混天气功=100
打狗棒=50
逍遥游=100
float values:
end of YPropertyGroup
begin of YObjectGroup
object=thing/破衣服
object=thing/铁杖
end of YObjectGroup
