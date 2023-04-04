#class=YDoorRoom
begin of YPropertyGroup
string values:
名字=西门
描述=@LONG
    这里是长安城的西门，厚实的城墙，高耸的城楼给人一种气势不凡的感觉。城门内外人来人往，都在为着生计忙活着，城门边上有几个士兵在巡逻。<br>
    这里明显的出口是<a href='javascript:sendmsg("w")'>西</a>和<a href='javascript:sendmsg("e")'>东</a>。
LONG
西=西门外
东=西大街2
NPC列表=官兵37 官兵38 武将19
禁止方向列表=东 西
老板名字=官兵37 官兵38
官兵37警告信息=官兵37坐在地上紧紧地抱住你的双腿，嚷着对你说：No Money,No Go!
官兵38警告信息=官兵38拿了一把小刀在你面前晃来晃去，不停地问你：你怕不怕？你怕不怕？
long values:
禁止所有人=1
float values:
end of YPropertyGroup
begin of YObjectGroup
object=npc/武将19
object=npc/官兵38
object=npc/官兵37
end of YObjectGroup