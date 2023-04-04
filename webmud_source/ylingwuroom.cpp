//---------------------------------------------------------------------------
//
// lingroom.cpp    2001年6月20日
//
// 类lingwuroom：领悟的房间
//
// 作者：刘鹏   单位：西陆资讯娱乐网
//
//---------------------------------------------------------------------------
#include "webmud.h"
//--------------------------------------------------------
YLingWuRoom::YLingWuRoom(YString initFileName):YRoom(initFileName),m_LingWuSpend(0)
                                                                  ,m_QianNengSpend(0)
                                                                  ,m_LingLingWuMax(0)
                                                                  ,m_RoomAllSkills("")
{

}
//--------------------------------------------------------
//方法isTypeOf--是否是某种类型或其派生类型
int YLingWuRoom::isTypeOf(YString className)
{
  if(className=="YLingWuRoom") return 1;
  else return YRoom::isTypeOf(className);
}
//--------------------------------------------------------
//事件onTimer的默认处理函数
int YLingWuRoom::onTimer(void)
{
  YRoom::onTimer(); //首先调用父类的处理函数
  return 1;
}
//--------------------------------------------------------
//事件AfterLoad的默认处理函数
int YLingWuRoom::onAfterLoad(void)
{
  int retVal=YRoom::onAfterLoad();
  setLingWuSpend(getAsLong("领悟消耗"));
  setQianNengSpend(getAsLong("潜能消耗"));
  setLingLingWuMax(getAsLong("领悟级别"));
  setRoomAllSkills(getAsString("允许领悟武功"));

  return retVal;
}
//--------------------------------------------------------
//事件BeforeSave的默认处理函数
int YLingWuRoom::onBeforeSave(void)
{
  int retVal=YRoom::onBeforeSave();
  setLingWuSpend(getLingWuSpend());
  setQianNengSpend(getQianNengSpend());
  setLingLingWuMax(getLingLingWuMax());
  setRoomAllSkills(getRoomAllSkills());
  return retVal;
}
//--------------------------------------------------------
//作为场所执行命令，已处理返回1，未处理返回0
int YLingWuRoom::executeCommand(YAbstractActor* executor, YString command)
{
  int retVal=0;
  YString cmd=command;
  YString commandVerb=cmd.getWord();
  if(commandVerb=="lingwu") retVal=lingwu(executor,cmd);
  if(commandVerb=="lian") retVal=lian(executor,cmd);
  if(!retVal) return YRoom::executeCommand(executor, command);
  else return retVal;
}
//--------------------------------------------------------
int YLingWuRoom::lian(YAbstractActor* executor,YString command)
{
   if (executor->isTypeOf("YPlayer"))
   {
      ((YPlayer*)executor)->display("这里不是练功的地方！！");
   }
   return 1;
}
//--------------------------------------------------------
int YLingWuRoom::lingwu(YAbstractActor* executor,YString command)
{
   YString skillname = command.getWord();
   long howmuch = (command.getWord()).toLong();
   if (howmuch <= 0 ) howmuch = 1;
   YPlayer* player;
   if (executor->isTypeOf("YPlayer"))player  = (YPlayer*)executor ;
   else
   {
      YErr::show("YLingWuRoom发命令的出错啦。");
     return 1;
   }
   //取普通武功
   if ((skillname.find("基本",0)) == YString::npos ) //找不到
   {
     player->display (skillname+"不是基本武功啊!");
     return 1;
   }
   YString allSkillname = player->getAllSkillsList();
   YString tempSkills ;
   tempSkills = allSkillname.getWord();
   long flag =0 ;//标示武功名是否找到
   while ( (tempSkills != "" ) && (flag == 0 ))
   {
      if ( tempSkills != skillname )  tempSkills = allSkillname.getWord();
      else  flag =1;
   }
   if ( flag == 0)
   {
     player->display("你不会这种武功怎么可以领悟？？");
     return 1;
   }
   //察看房间内是否可以领悟这种武功，
   flag = 0;
   YString RoomSkills = getRoomAllSkills();//getAsString("允许领悟武功");
   tempSkills = RoomSkills.getWord();
   while ( (tempSkills != "" ) && (flag == 0 ))
   {
      if ( tempSkills != skillname )  tempSkills = RoomSkills.getWord();
      else  flag =1;
   }
   if (flag ==0)
   {
      player->display("你领悟了一段时间发现"+tempSkills+"在这里不会有任何进展。");
      return 1;
   }

   long spend = getLingWuSpend() - player->getLastLearn();//getAsLong("领悟消耗");
   if (spend <= 0 ) spend =1;
   long qianneng =getQianNengSpend();// getAsLong("潜能消耗");
   long jingli = player->getEnergy()  ;
   long qixue = player->getBody()  ;
   long playerqianneng = player -> getPotential() ;
   if (qianneng < 1) qianneng =1;
   if ((jingli > spend) && (qixue > spend))
   {
     if (( howmuch*spend )>jingli) howmuch = jingli/spend;
     if (( howmuch*spend )>qixue) howmuch = qixue/spend;
     if ((howmuch*qianneng) > playerqianneng ) howmuch = playerqianneng/qianneng;
     if (howmuch == 0)
     {
       player->display ("你已经没有潜能了");
       return 1;
     }
     else if(howmuch > 100) howmuch =100;
     YString skillexp = player->getAsLong(skillname+"_经验");
     long skilllevel = player->getSkillLevel (skillname);
     if (skilllevel < 250 )//级别太低不能领悟
     {
       player->display ("只有武功高于250级的时候才能领悟");
       return 1;
     }
     //设计领悟升级的公式
     //还没有确定等讨论后决定怎么添加
     flag =0;//标志请空
     YString teshuskills="";//特殊武功名称
     if(skillname=="基本拳法" || skillname=="基本掌法" || skillname=="基本指法"
     || skillname=="基本手法" || skillname=="基本爪法" || skillname=="基本腿法") //根据臂力去去最大
     {
        teshuskills =player->getAsString("空手_攻击武功");
        if ((player->getAsLong(teshuskills)+1) < skilllevel)
        {
          flag =2;
        }
        if (getLingLingWuMax()+(player->getBeginArm()*2) < skilllevel )
        {
           flag = 1;
        }

     }
     if(skillname=="基本剑法" || skillname=="基本刀法" || skillname=="基本棍法"
     || skillname=="基本鞭法" || skillname=="基本枪法" ) //根据臂力去去最大
     {
        YString wuqi = player->getWeaponType();
        if (wuqi == "")
        {
          player -> display("没拿趁手的兵器怎么领悟？");
          return 1;
        }
        wuqi = wuqi+"_攻击武功";
        teshuskills =player->getAsString(wuqi);
        if ((player->getAsLong(teshuskills)+1) < skilllevel)
        {
          flag =2;
        }
        if (getLingLingWuMax()+(player->getBeginArm()*2) < skilllevel )
        {
           flag = 1;
        }

     }
     else if(skillname=="基本内功")  //根据根骨
     {
        if (getLingLingWuMax()+ (player->getBeginArm()*2) < skilllevel )
        {
             flag =1;
        }
     }
     else if(skillname=="基本轻功")  //根据身法
     {
        teshuskills =player->getDodgeUsing();
        if ((player->getAsLong(teshuskills)+1) < skilllevel)
        {
          flag =2;
        }
        else if (getLingLingWuMax()+ ( player->getBeginForce()*2) < skilllevel )
        {
          flag = 1;
        }
     }
     else if(skillname=="基本招架")//根据身法和臂力
     {
        if (getLingLingWuMax()+ player->getBeginForce() + player->getBeginArm() < skilllevel )
        {
          flag = 1;
        }
     }
     if (flag == 1)
     {
       player -> display ("你已经无法领悟到更高深的"+skillname+"了。");
       return 1;
     }
     else if(flag ==2)
     {
       player -> display  ("你的"+ teshuskills+"还需要勤加练习。");
       return 1;
     }
     YString skillsExp = skillname + "_经验";
     long howskillsexp = player->getAsLong (skillsExp);
     //经验的速度
     //根据读书写字来决定
     long learn = player->getLearn() ;
     long inlearn = (rand() % (learn/2)) + learn/6; //1/6~2/3
     inlearn = inlearn * player->getLuck() / 80;
     howskillsexp = howskillsexp + (howmuch * inlearn );

     //升级
     if (inlearn  > 100 )player->display("你眉头一展，只觉脑中豁然开朗。");
     else if (inlearn*3 > 50 )player->display("你凝神沉思，但觉心头似有所悟。");
     else player->display("你只觉的心烦意乱，看来没有什么大的进展");

     //player->display("你领悟了"+(YString)howmuch+"次获得了"+(YString)(inlearn*howmuch)+"点经验。");
     if (howskillsexp >((skilllevel+1) * (skilllevel+1) * 10))
     {
       player->playerlevelup(skillname);
       player->set (skillsExp,0);
       getRoomChat()->talk(NULL,"<font color=red>"+player->getActorName()+"的"+skillname+"升级了！！！</font>",player, NULL);
     }
     else
     {
        player->set(skillsExp,howskillsexp);
     }

     //消耗的气血和潜能
     player->setEnergy(jingli- howmuch*spend);
     player->setBody(qixue - howmuch*spend );
     player->setPotential( playerqianneng - (howmuch*qianneng));
   }
   else
   {
     player->display ("你的气血精力不足，无法领悟。");
   }
   return 1;
}
