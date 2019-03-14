#include <avr/io.h>
#include <util/delay.h>
#include <util/setbaud.h>

#include "i2c.c"
#include "MPR121_Addrs.h"
#include "custom_MPR121.h"
#define MPR121_ADDR 0x5A

void init_MPR121() {
    uint8_t USL = 200;
    uint8_t startAll = 0x8F;
    uint8_t ACE_ARE = 0x03;
    uint8_t LSL = 180;
    uint8_t TL = 100;

    //reset everything back to default
    writeByte(MPR121_ADDR, ELE_CONFIGURATION, 0x0);

    //set touch and release threshholds
    for (uint8_t i = 0; i < 12; i++) 
    {
        writeByte(MPR121_ADDR, ELE0_TOUCH_THRESHOLD + (i * 2), 12);
        writeByte(MPR121_ADDR, ELE0_RELEASE_THRESHOLD + 1 + (i * 2), 6);
    }

    writeByte(MPR121_ADDR, SOFT_RESET, 0x63);
    writeByte(MPR121_ADDR, UPSIDE_LIMIT, USL);
    writeByte(MPR121_ADDR, AUTO_CONFIG_REG_0, ACE_ARE);
    writeByte(MPR121_ADDR, LOWSIDE_LIMIT, LSL);
    writeByte(MPR121_ADDR, TARGET_LEVEL, TL);


    writeByte(MPR121_ADDR, NHDR, 0x01);
    writeByte(MPR121_ADDR, MHDR, 0x01);
    writeByte(MPR121_ADDR, NCLR, 0x0E);
    writeByte(MPR121_ADDR, FDLR, 0x00);

    writeByte(MPR121_ADDR, MHDF, 0x01);
    writeByte(MPR121_ADDR, NHDF, 0x05);
    writeByte(MPR121_ADDR, NCLF, 0x01);
    writeByte(MPR121_ADDR, FDLF, 0x00);

    writeByte(MPR121_ADDR, NHDT, 0x00);
    writeByte(MPR121_ADDR, NCLT, 0x00);
    writeByte(MPR121_ADDR, FDLT, 0x00);

    writeByte(MPR121_ADDR, ELE_DEBOUNCE, 2);

    //start
    writeByte(MPR121_ADDR, ELE_CONFIGURATION, startAll);
}

int16_t touched_status(void) {
    int8_t lower = readByte(MPR121_ADDR, TOUCH_STATUS_0);
    int8_t upper = readByte(MPR121_ADDR, TOUCH_STATUS_1);
    int16_t combined = lower | (upper << 8);
    return combined;
}

