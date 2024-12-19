#include <string>
using namespace std;

struct GPS
{
    float lat, lon;
};

struct log
{
    string date;
    string time;
    int bearing;
    GPS pos;   // GPS coordinates
    float vel; // volcity
    log *next, *prev;

    log()
    {
        next = prev = nullptr;
    }
};