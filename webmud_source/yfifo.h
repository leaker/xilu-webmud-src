//---------------------------------------------------------------------------
//
// yfifo.h    2000年6月12日
//
// 类YFIFO：提供对FIFO的支持，在WIN32用文件仿真
//
// 作者：叶林   单位：西陆资讯娱乐网
//
//---------------------------------------------------------------------------

#ifndef __WIN32__  //UNIX
  #include <semaphore.h>
  #include "yfifo_shm.h"
#else //windows,不使用共享内存

#ifndef __YFIFO_H__
#define __YFIFO_H__

#include <queue>
#include "ystring.h"

class YFIFO {
  private:
    YString m_name; //FIFO的名字
    YString m_mode;
    YString delim; //消息之间的分割符
    #ifndef __WIN32__  //UNIX
      sem_t *m_sem;
    #endif
    queue<YString> messages;
    int MAXLEN;
  protected:

  public:
		YFIFO(YString name, YString mode, int shm_size=1024);  //构造函数
		~YFIFO(); //析构函数
    YString getName(void) {return m_name;}
    int write(YString s, unsigned long timeout=-1);
    YString read(int maxlen=0, unsigned long timeout=-1);
    void clear(void); //清空缓存
};

#endif //__YFIFO_H__
#endif //windows,不使用共享内存