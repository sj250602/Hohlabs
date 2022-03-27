#include "util/io.h"

namespace vgatext{

   static inline void writechar(int loc, uint8_t c, uint8_t bg, uint8_t fg, addr_t base){
     //your code goes here
     uint16_t d = uint16_t((bg<<4)|(fg));
     mmio::write16(base,uint32_t(2*loc),(d<<8)|c);
   }

}//namespace vgatext
