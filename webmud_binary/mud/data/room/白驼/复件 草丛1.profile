#class=YRoom
begin of YPropertyGroup
string values:
名字=草丛
描述=@LONG
    这里是一片乱草丛，根本没有道路可以走，到处都是齐腰高的蒿草，常有毒蛇出没。<br>
    这里明显的出口是<a href=ne onClick='return sendmsg("ne");' target=result>北东</a>,<a href=nw onClick='return sendmsg("nw");' target=result>北西</a>和<a href=sd onClick='return sendmsg("sd");' target=result>南down</a>。
LONG
东北=草丛2
西北=草丛3
南下=后门
NPC列表=竹叶青蛇2
long values:
float values:
end of YPropertyGroup
begin of YObjectGroup
object=npc/竹叶青蛇2
end of YObjectGroup