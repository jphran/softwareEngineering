#include "tool.h"

Tool::Tool(QColor color, Canvas *canvas)
{
    currentCanvas = canvas;
    brushColor = color;
}

Tool::~Tool()
{

}

void Tool::preformLine(int x1, int y1, int x2, int y2)
{
   // Iterators, counters required by algorithm
   int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;

   // Calculate line deltas
   dx = x2 - x1;
   dy = y2 - y1;

   // Create a positive copy of deltas (makes iterating easier)
   dx1 = abs(dx);
   dy1 = abs(dy);

   // Calculate error intervals for both axis
   px = 2 * dy1 - dx1;
   py = 2 * dx1 - dy1;

   // The line is X-axis dominant
   if (dy1 <= dx1) {

       // Line is drawn left to right
       if (dx >= 0) {
           x = x1; y = y1; xe = x2;
       } else { // Line is drawn right to left (swap ends)
           x = x2; y = y2; xe = x1;
       }

       this->perform(x, y); // Draw first pixel

       // Rasterize the line
       for (i = 0; x < xe; i++) {
           x = x + 1;

           // Deal with octants...
           if (px < 0) {
               px = px + 2 * dy1;
           } else {
               if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
                   y = y + 1;
               } else {
                   y = y - 1;
               }
               px = px + 2 * (dy1 - dx1);
           }

           // Draw pixel from line span at
           // currently rasterized position
           this->perform(x, y);
       }

   } else { // The line is Y-axis dominant

       // Line is drawn bottom to top
       if (dy >= 0) {
           x = x1; y = y1; ye = y2;
       } else { // Line is drawn top to bottom
           x = x2; y = y2; ye = y1;
       }

       this->perform(x, y); // Draw first pixel

       // Rasterize the line
       for (i = 0; y < ye; i++) {
           y = y + 1;

           // Deal with octants...
           if (py <= 0) {
               py = py + 2 * dx1;
           } else {
               if ((dx < 0 && dy<0) || (dx > 0 && dy > 0)) {
                   x = x + 1;
               } else {
                   x = x - 1;
               }
               py = py + 2 * (dx1 - dy1);
           }

           // Draw pixel from line span at
           // currently rasterized position
           this->perform(x, y);
       }
   }
}

void Tool::setCurrentCanvas(Canvas *canvas)
{
    currentCanvas = canvas;
}

void Tool::setBrushColor(QColor color)
{
    brushColor = color;
}

Frame* Tool::getCurrentFrame()
{
    return currentCanvas->getCurrentFrame();
}

QColor Tool::getBrushColor()
{
    return brushColor;
}
