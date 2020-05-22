#ifndef SVG_H_INCLUDED
#define SVG_H_INCLUDED
#include <string>
#include <vector>
using namespace std;
void svg_begin(double width, double height);
  void svg_end();
  void svg_text(double left, double baseline, string text);
  void svg_rect(double x, double y, double width, double height,string stroke = "black" ,string fill = "black");
  void svg_line(double x1,double y1,double x2,double y2,string stroke_1,struct Input bindata);
  void show_histogram_svg(const vector<size_t>& bins,struct Input bindata,string name);

#endif // SVG_H_INCLUDED
