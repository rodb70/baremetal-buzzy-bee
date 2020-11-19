/*
 * gba.c
 *
 */

uint16_t *videoBuffer = (uint16_t*) 0x6000000;

// ***=========================================================

void setPixel(int row, int col, uint16_t color)
{
    videoBuffer[row * 240 + col] = color;
}
void waitForVblank()
{
    while( SCANLINECOUNTER > 160 )
        ;
    while( SCANLINECOUNTER < 161 )
        ;
}

void drawBuzz(int y, int x, const uint16_t *buzz)
{
    for( int r = 0; r < 20; r++ )
    {
        DMA[3].src = buzz + (r * 20);
        DMA[3].dst = (videoBuffer + OFFSET( x + r, y, 240 ));
        DMA[3].cnt = (20) | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT;
    }
}

void drawRectangle(int col, int row, int width, int height, volatile uint16_t color)
{
    for( int r = 0; r < height; r++ )
    {
        DMA[3].src = &color;
        DMA[3].dst = &videoBuffer[OFFSET( row + r, col, 240 )];
        DMA[3].cnt = (width) | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED;
    }
}

void drawImage(int row, int col, int width, int height, const uint16_t *image)
{
    for( int r = 0; r < height; r++ )
    {
        // pointer to first pixel of image + (position of each row)
        DMA[3].src = image + (r * width);
        DMA[3].dst = (videoBuffer + OFFSET( row + r, col, 240 ));
        DMA[3].cnt = (width) | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT;
    }
}

