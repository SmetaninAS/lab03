#include <curl/curl.h>
#include <iostream>
#include <sstream>
#include <windows.h>
#include <curl/curl.h>
#include <iostream>
#include <sstream>
#include <tchar.h>
#include <string>
#include <vector>
#include <iomanip>
#include "histogram.h"
#include "svg.h"
using namespace std;
  void svg_begin(double width, double height)
{
    cout << "<?xml version='1.0' encoding='UTF-8'?>\n";
    cout << "<svg ";
    cout << "width='" << width << "' ";
    cout << "height='" << height << "' ";
    cout << "viewBox='0 0 " << width << " " << height << "' ";
    cout << "xmlns='http://www.w3.org/2000/svg'>\n";
}

  void svg_end()
    {
    cout << "</svg>\n";

    }
void svg_text(double left, double baseline, string text)
{
cout << "<text x='" << left << "' y='" << baseline <<"'>" <<text <<"</text>";

}
void svg_rect(double x, double y, double width, double height,string stroke ,string fill)
{
    cout << "<rect x='"<<x<< "' y='" <<y<<"' width='" <<width <<"' height='" <<height <<"' stroke='"<<stroke<<"' fill='"<<fill<<"'/>";

}
void svg_line(double x1,double y1,double x2,double y2,string stroke_1,struct Input bindata)
{
    cout << "<line x1='"<<x1<<"' y1='"<<y1<<"' x2='"<<x2<<"' y2='"<<y2<<"' stroke='"<<stroke_1<<"' stroke-dasharray='"<<bindata.length_ch<<" "<<bindata.length_pr<<"' />";
}

void show_histogram_svg(const vector<size_t>& bins,struct Input bindata,string name)
{
const auto IMAGE_WIDTH = 600;
const auto IMAGE_HEIGHT = 300;
const auto TEXT_LEFT = 20;
const auto TEXT_BASELINE = 20;
const auto TEXT_WIDTH = 50;
const auto BIN_HEIGHT = 30;
const auto BLOCK_WIDTH = 10;
const size_t SCREEN_WIDTH = 80;
const size_t MAX_ASTERISK = SCREEN_WIDTH - 4 - 1;
double top = 0;
  svg_begin(600, 300);
  size_t max_count = 0;
         for (size_t count : bins)
    {
        if (count > max_count)
        {
            max_count = count;
        }
    }
    const bool scaling_needed = max_count > MAX_ASTERISK;
     for (size_t bin : bins)
        {
            size_t height = bin;
            if (scaling_needed)
            {
                const double scaling_factor = (double)MAX_ASTERISK / max_count;
                height = (size_t)(bin * scaling_factor);
            }
            const double bin_width = BLOCK_WIDTH * height;
            svg_text(TEXT_LEFT, top + TEXT_BASELINE, to_string(bin));
            svg_rect(TEXT_WIDTH, top, bin_width, BIN_HEIGHT,"blue","#aaffaa");
            if (top !=(bindata.bin_count-1)*BIN_HEIGHT)
                svg_line(BLOCK_WIDTH,top+BIN_HEIGHT,IMAGE_WIDTH,top+BIN_HEIGHT,"red",bindata);
            top += BIN_HEIGHT;
        }
    svg_text(TEXT_LEFT, top + TEXT_BASELINE, name);

    svg_end();
}
