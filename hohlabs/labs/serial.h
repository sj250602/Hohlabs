#pragma once

#include "util/io.h"

namespace serial{

  static inline bool is_transmitter_ready(io_t baseport){
    //insert your code here
    uint8_t z = io::read8(baseport,5);
    uint8_t p = (1<<5);
    return ((z&p)!=0);
  }

  static inline void writechar(uint8_t c, io_t baseport){
    //insert your code here
    io::write8(baseport,0,c);
  }

} //end serial
