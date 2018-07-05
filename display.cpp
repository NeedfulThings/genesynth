#include <U8x8lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

#include "display.h"

namespace thea {
namespace display {

U8X8_SH1106_128X64_NONAME_4W_HW_SPI display(/* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);

void init(void) {
  display.begin();
  display.setFlipMode(0);
  display.setPowerSave(0);
  display.setFont(u8x8_font_chroma48medium8_r);
}

} // namespace thea
} // namespace display