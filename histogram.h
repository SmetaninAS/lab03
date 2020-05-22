#ifndef HISTOGRAM_H_INCLUDED
#define HISTOGRAM_H_INCLUDED
#include <vector>
using namespace std;
struct Input
{
    vector<double> numbers;
    size_t bin_count;
    size_t length_ch; size_t length_pr;
    };



void find_minmax(const vector<double> &numbers, double& min, double& max);

#endif // HISTOGRAM_H_INCLUDED
