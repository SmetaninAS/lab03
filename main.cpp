
#include <curl/curl.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include "histogram.h"
using namespace std;

vector<double> input_numbers(istream& in,size_t count)
{
    vector<double> result(count);
    for (size_t i = 0; i < count; i++) {
        in >> result[i];
    }
    return result;
}
Input read_input(istream& in,bool prompt)
{
    Input data;
    if (prompt==true)
    cerr << "Enter number count: ";
    size_t number_count;
    cin >> number_count;
    if (prompt==true)
    cerr << "Enter numbers: ";
    data.numbers = input_numbers(in, number_count);
    size_t bin_count;
    if (prompt==true)
    cerr << "Enter column count: ";
    cin >> data.bin_count;

    return data;
}

vector <size_t>  make_histogram(struct Input bindata)
    {

        double min;
        double max;
        find_minmax(bindata.numbers,min,max);
        vector<size_t> bins(bindata.bin_count,0);
        for (double number : bindata.numbers)
            {
        size_t bin;
        bin = (number - min) / (max - min) * bindata.bin_count;
        if (bin == bindata.bin_count)
            {
            bin--;
            }
        bins[bin]++;
            }
           return bins;
    }
void show_histogram_text(const vector<size_t> &bins)
    {

    const size_t SCREEN_WIDTH = 80;
    const size_t MAX_ASTERISK = SCREEN_WIDTH - 4 - 1;

    size_t max_count = 0;
    for (size_t count : bins) {
        if (count > max_count) {
            max_count = count;
        }
    }
    const bool scaling_needed = max_count > MAX_ASTERISK;

    for (size_t bin : bins) {
        if (bin < 100) {
            cout << ' ';
        }
        if (bin < 10) {
            cout << ' ';
        }
        cout << bin << "|";

        size_t height = bin;
        if (scaling_needed) {
            const double scaling_factor = (double)MAX_ASTERISK / max_count;
            height = (size_t)(bin * scaling_factor);
        }

        for (size_t i = 0; i < height; i++) {
            cout << '*';
        }
        cout << '\n';
    }
    }



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
void svg_rect(double x, double y, double width, double height,string stroke = "black" ,string fill = "black")
{
    cout << "<rect x='"<<x<< "' y='" <<y<<"' width='" <<width <<"' height='" <<height <<"' stroke='"<<stroke<<"' fill='"<<fill<<"'/>";
}
void show_histogram_svg(const vector<size_t>& bins)
{
const auto IMAGE_WIDTH = 400;
const auto IMAGE_HEIGHT = 300;
const auto TEXT_LEFT = 20;
const auto TEXT_BASELINE = 20;
const auto TEXT_WIDTH = 50;
const auto BIN_HEIGHT = 30;
const auto BLOCK_WIDTH = 10;
double top = 0;
  svg_begin(400, 300);
for (size_t bin : bins)
    {
    const double bin_width = BLOCK_WIDTH * bin;
    svg_text(TEXT_LEFT, top + TEXT_BASELINE, to_string(bin));
    svg_rect(TEXT_WIDTH, top, bin_width, BIN_HEIGHT,"blue","#aaffaa");
    top += BIN_HEIGHT;
    }
    svg_end();
}

size_t write_data(void* items, size_t item_size, size_t item_count, void* ctx)
{
 stringstream* buffer = reinterpret_cast<stringstream*>(ctx);
    const char* itemsch = reinterpret_cast<const char*>(items);
    size_t data_size;
    data_size=item_size* item_count;
    buffer->write(itemsch, data_size);
    return data_size;
}

Input download(const string& address)
{
    stringstream buffer;
     curl_global_init(CURL_GLOBAL_ALL);
    CURL* curl = curl_easy_init();
    if (curl)
        {
  CURLcode res;
  curl_easy_setopt(curl, CURLOPT_URL, address.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
  res = curl_easy_perform(curl);
  if(res!=0)
    {
    cout<<curl_easy_strerror(curl_easy_perform(curl));
    exit(1);
    }
    curl_easy_cleanup(curl);
    }
    return read_input(buffer, false);
}


int main(int argc, char* argv[])
{
    Input input;
    if (argc > 1) {
        input = download(argv[1]);
    } else {
        input = read_input(cin, true);
    }

    const auto bins = make_histogram(input);
    show_histogram_svg(bins);
    return 0;
}
