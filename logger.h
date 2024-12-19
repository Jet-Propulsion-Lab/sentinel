#include <iostream>
#include <ostream>
#include <data.h>
using namespace std;

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