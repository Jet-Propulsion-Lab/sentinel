#include <iostream>
#include <ostream>
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

class dlist
{
public:
    log *head, *tail;

    dlist()
    {
        head = tail = nullptr;
    }

    void insert(log data)
    {
        log *newNode = new log;
        *newNode = data;

        if (head == nullptr)
        {
            head = tail = newNode;
        }
        else
        {
            head->next = newNode;
            newNode->prev = head;
            head = newNode;
        }
    }
};

void readlogs(dlist logs)
{
    log *current = logs.tail;

    while (current != nullptr)
    {
        cout << "<" << current->date << ">" << "<" << current->time << ">" 
        << "<" << current->pos.lat << "," << current->pos.lon << ">" << "<" << current->bearing << ">" << "<" << current->vel << " m/s >" << endl;

        current = current->next;
    }
}

void writelog(dlist data)
{
}