/*
 * hardware.h
 *
 */

#ifndef GBA_HARDWARE_H_
#define GBA_HARDWARE_H_

// *** Video =======================================================

/* Mode 3 */
extern u16 *videoBuffer; // 0x6000000
// Need: unsigned short *videoBuffer = (unsigned short *)0x6000000;
#define REG_DISPCNT  *(u16*) 0x4000000
#define SCANLINECOUNTER *(unsigned short *)0x4000006

//Background Enables
#define BG0_ENABLE  (1<<8)
#define BG1_ENABLE  (1<<9)
#define BG2_ENABLE  (1<<10)
#define BG3_ENABLE  (1<<11)

#define MODE_0 0
#define MODE_1 1
#define MODE_2 2
#define MODE_3 3
#define MODE_4 4
#define MODE_5 5
#define MODE_6 6

// *** DMA =======================================================

typedef struct
{
    const volatile void *src;
    const volatile void *dst;
    u32 cnt;
} DMA_CONTROLLER;
#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

// Defines
// DMA channels
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

// Mostly just used for sound.
#define DMA_REPEAT (1 << 25)

// DMA copies 16 bits at a time normally (1 pixel),
// but can be set to copy 32 bits at a time to be
// even faster for mutliples of 32 bits.
#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

// Don't worry about these, the default DMA_NOW is good enough
// for 2110, if you want to go beyond the scope of the class you
// can ask a TA or the internet about the other options.
#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

#define BUTTONS (*(unsigned int *)0x4000130)


extern uint16_t *videoBuffer;
static inline void hardware_video_init( void )
{
    REG_DISPCNT = MODE_3 | BG2_ENABLE;
}

void waitForVblank(void);




#endif /* GBA_HARDWARE_H_ */
