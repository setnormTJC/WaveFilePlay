#include "PlotImage.h"



PlotImage::PlotImage(unsigned int imageWidth, unsigned int imageHeight, const Color& fillColor)
    :BMPImage(imageWidth, imageHeight, fillColor)
    , imageWidth(imageWidth), imageHeight(imageHeight)
{

    plotWidth = imageWidth - PADDING; //100 is arbitary padding value
    plotHeight = imageHeight - PADDING;


    drawXAxis(origin.first, origin.second, plotWidth, ColorEnum::Magenta);
    drawYAxis(origin.first, origin.second, plotHeight, ColorEnum::Green);

}



void PlotImage::drawXAxis(unsigned int x0, unsigned int y0, unsigned int xf, const ColorEnum& color)
{
    const int TICK_SPACING = plotWidth / 4;

    for (unsigned int x = x0; x < xf; ++x)
    {
        pixelData.pixelMatrix[y0][x] = Color{ color };

        //display "tick" every 25% of the way: 
        if ((x - x0) % TICK_SPACING == 0)
        {
            //add extra vertical pixels:
            const int yPixelCountForTick = 10;

            for (int y = y0 - yPixelCountForTick; y < y0 + yPixelCountForTick; ++y)
            {
                pixelData.pixelMatrix[y][x] = Color{ color };
            }
        }
    }
}

void PlotImage::drawYAxis(unsigned int x0, unsigned int y0, unsigned int yf, const ColorEnum& color)
{
    if (x0 < 0 || y0 < 0 || yf > imageHeight)
    {
        throw std::runtime_error("attempting to drawLine outside of imageWidth or imageHeight");
    }

    const int TICK_SPACING = plotHeight / 4;

    for (unsigned int y = y0; y < yf; ++y)
    {
        pixelData.pixelMatrix[y][x0] = Color{ color }; //note the "swapped" order of x and y here

        //display "tick" every 25% of the way: 
        if ((y - y0) % TICK_SPACING == 0)
        {
            //add extra vertical pixels:
            const int xPixelCountForTick = 10;

            for (int x = x0 - xPixelCountForTick; x < x0 + xPixelCountForTick; ++x)
            {
                pixelData.pixelMatrix[y][x] = Color{ color };
            }
        }
    }
}

void PlotImage::plotData(const map<int, int>& elementCountsToExecutionTimes, const ColorEnum& color)
{
    // Get the maximum element count (last key in the map) for SCALING 
    int maxElementCount = elementCountsToExecutionTimes.rbegin()->first;
    //also maxExecutionTime, for scaling (POSSIBLE that this won't be rbegin()->second (if "unlucky" algo)
    int maxExecutionTime = 0;
    for (const auto& currentPair : elementCountsToExecutionTimes)
    {
        if (currentPair.second > maxExecutionTime)
        {
            maxExecutionTime = currentPair.second;
        }
    }

    int pairCounter = 0;

    for (const auto& currentPair : elementCountsToExecutionTimes)
    {
        pairCounter++;
        //int x = origin.first + currentPair.first / plotWidth;  
        //int y = origin.second + currentPair.second / plotHeight; 
        int x = origin.first + (currentPair.first * (plotWidth - origin.first)) / maxElementCount;
        int y = origin.second + (currentPair.second * (plotHeight - origin.second)) / maxExecutionTime;

        setPixelToColor_withThickness(x, y, Color{ color }, 3);

     
    }
}

