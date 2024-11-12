#include <iostream>
#include <string>
#include <list>
using namespace std;

struct finMount
{
    bool left;  // 1 for left mounted, 0 for right
    bool front; // 1 for front mounted, 0 for back
};

class gridfin
{
public:
    int num;
    string type;
    float angle;
    finMount mount;

    gridfin(int n, string t, bool l, bool f)
    {
        num = n;
        type = t;
        angle = 0.0;
        mount.left = l;
        mount.front = f;
    }
};

class gridSystem // Graph to connect the grid fins together and control them
{
public:
    int numV; // Number of Grid Fins
    list<gridfin> *adj;

    gridSystem(int V)
    {
        this->numV = V;
        adj = new list<gridfin>[V];
    }

    void addNew()
    {
        int n;
        string t;
        bool l, f;

        cout << "Please set the grid fin number: ";
        cin >> n;
        cout << "Please enter the type of grid fin mesh: ";
        cin >> t;
        cout << "Please enter 1 if fin is left mounted, 0 if right mounted: ";
        cin >> l;
        cout << "Please enter 1 if fin is front mounted, 0 if back mounted: ";
        cin >> f;

        gridfin gnew(n, t, l, f);

        for (int i = 0; i < numV; i++)
        {
            for (auto temp : adj[i])
            {
                if ((temp.mount.left == gnew.mount.left) || (temp.mount.front == gnew.mount.front))
                {
                    adj[i].push_back(gnew);
                    adj[gnew.num].push_back(temp); // Add bidirectional connection
                }
            }
        }
    }

    void printGraphRecursive(int v)
    {
        cout << "-> " << v;

        for (auto x : adj[v])
        {
            printGraphRecursive(x.num);
        }
    }

    void printGraph()
    {
        for (int v = 0; v < numV; ++v)
        {
            cout << "Adjacency list of vertex " << v << ":\n head ";
            printGraphRecursive(v);
            cout << endl;
        }
    }
};

void gkeepvertical(gridSystem &graph, float pitch, float yaw)
{
    for (int i = 0; i < graph.numV; i++)
    {
        for (auto &g : graph.adj[i])
        {
            if (pitch > 0)
            { // Nose Up
                if (g.mount.front == 1)
                {
                    g.angle = ((pitch * 60) / 360);
                }
                else
                {
                    g.angle = -1 * ((pitch * 60) / 360);
                }
            }
            else if (pitch < 0)
            { // Nose Down
                if (g.mount.front == 1)
                {
                    g.angle = -1 * ((pitch * 60) / 360);
                }
                else
                {
                    g.angle = ((pitch * 60) / 360);
                }
            }

            if (yaw > 0)
            { // Veer Right
                if (g.mount.left == 1)
                {
                    g.angle += ((yaw * 60) / 360);
                }
                else
                {
                    g.angle -= ((yaw * 60) / 360);
                }
            }
            else if (yaw < 0)
            { // Veer Left
                if (g.mount.left == 1)
                {
                    g.angle -= ((yaw * 60) / 360);
                }
                else
                {
                    g.angle += ((yaw * 60) / 360);
                }
            }
        }
    }
}

void gyaw(gridfin g[], int gnum, float yaw)
{
    if (yaw > 0) // Nose Up
    {
        for (int i = 0; i < gnum; i++)
        {
            if (g[i].mount.left == 1)
            {
                g[i].angle = ((yaw * 60) / 360);
            }
            if (g[i].mount.left == 0)
            {
                g[i].angle = -1 * ((yaw * 60) / 360);
            }
        }
    }

    if (yaw < 0) // Nose Up
    {
        for (int i = 0; i < gnum; i++)
        {
            if (g[i].mount.left == 1)
            {
                g[i].angle = -1 * ((yaw * 60) / 360);
            }
            if (g[i].mount.front == 0)
            {
                g[i].angle = ((yaw * 60) / 360);
            }
        }
    }
}

void gpitch(gridfin g[], int gnum, float pitch)
{
    if (pitch > 0) // Nose Up
    {
        for (int i = 0; i < gnum; i++)
        {
            if (g[i].mount.front == 1)
            {
                g[i].angle = ((pitch * 60) / 360);
            }
            if (g[i].mount.front == 0)
            {
                g[i].angle = -1 * ((pitch * 60) / 360);
            }
        }
    }

    if (pitch < 0) // Nose Down
    {
        for (int i = 0; i < gnum; i++)
        {
            if (g[i].mount.front == 1)
            {
                g[i].angle = -1 * ((pitch * 60) / 360);
            }
            if (g[i].mount.front == 0)
            {
                g[i].angle = ((pitch * 60) / 360);
            }
        }
    }
}

void airbrake(gridfin g[], int gnum)
{
    for (int i = 0; i < gnum; i++)
    {
        g[i].angle = 60;
    }
}