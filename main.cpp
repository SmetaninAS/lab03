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
#define INFO_BUFFER_SIZE 32767
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
    cerr << "enter length_ch";
    cin >> data.length_ch;
    cerr << "enter length_pr";
    cin >> data.length_pr;


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

int printf(const char* format, ...);
 DWORD WINAPI GetVersion(void);
 BOOL GetComputerNameA
    (
    LPSTR   lpBuffer,
    LPDWORD nSize
    );
string
make_info_text() {
    stringstream buffer;
    DWORD info = GetVersion();
    DWORD mask = 0x0000ffff;
    DWORD version = info & mask;
    DWORD platform = info>>16;
    DWORD mask_major = 0b00000000'00000000'00000000'11111111;
    char system_dir[MAX_PATH];
    GetSystemDirectory(system_dir, MAX_PATH);
    cerr <<endl;
    TCHAR  infoBuf[INFO_BUFFER_SIZE];
    DWORD  bufCharCount = INFO_BUFFER_SIZE;
    GetComputerName( infoBuf, &bufCharCount );
     cerr<<endl;
     if ((info&0x80000000)==0)
     {
      DWORD version_major = version & mask_major;
      DWORD version_minor = version >> 8;
      DWORD build = platform;

     buffer <<"System name : "<< infoBuf <<"  "<<"directory : "<< system_dir<<"  " <<"version : "<<version_major<<"."<<version_minor;
      }
    return buffer.str();
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
    double ulspeed_curl=0;
    if (CURLE_OK != curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE,&ulspeed_curl))
        cerr <<curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE,&ulspeed_curl);
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
    string name = make_info_text();
    const auto bins = make_histogram(input);
    show_histogram_svg(bins,input,name);
    return 0;
}

