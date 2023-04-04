#include "ystring.h"
#include "yfifo.h"

int main(void)
{
  YFIFO fifo("/etc/test.fifo","w");
  fifo.write("浣犲ソ!");
  return 0;
}
