#ifndef DATA_LOGGER_HPP
#define DATA_LOGGER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
using namespace std;

struct Coordinates{
    double latitude;
    double longitude;
    double altitude;

    Coordinates(): 
        latitude(0), longitude(0), altitude(0) {}
    Coordinates(double lat, double lon, double alt): 
        latitude(lat), longitude(lon), altitude(alt) {}
};

struct Logs{
    double timestamp;
    Coordinates gps;
    double bearing;      // in degrees
    double velocity;     // in m/s
    double acceleration; // in m/s²
    double temperature; // in Celsius
    double pressure;    // in hPa
    
   Logs(): 
        timestamp(0), bearing(0), velocity(0),
        acceleration(0), temperature(0), pressure(0) {}
};

struct DataNode{
    Logs logs;
    DataNode* prev;
    DataNode* next;

    DataNode(const Logs& d):
        logs(d),prev(NULL),next(NULL){};

    DataNode(Logs* d):
        logs(*d),prev(NULL),next(NULL){};
};

class DataLogger{
    private:
    //For doubly linked list

    DataNode* head;
    DataNode* tail;
    long int dataCount;
    static const int CACHE_SIZE = 20;
    string logFilePath;

    public:
    DataLogger(const string& filePath = "rocket_data.txt"): 
        head(nullptr), tail(nullptr), dataCount(0), logFilePath(filePath) {}

    string formatDataForFile(const Logs& logs){
    stringstream ss;
    ss << fixed << setprecision(6);
    
    // Format: timestamp,lat,lon,alt,bearing,velocity,accel,temp,pressure
    ss << logs.timestamp << ","
       << logs.gps.latitude << ","
       << logs.gps.longitude << ","
       << logs.gps.altitude << ","
       << logs.bearing << ","
       << logs.velocity << ","
       << logs.acceleration << ","
       << logs.temperature << ","
       << logs.pressure;
    
    return ss.str();
    }

    void writeNodeToFile(const DataNode* node){
    if(!node)
        return;

    ofstream file(logFilePath, ios::app);
    if (!file) {
        throw runtime_error("Unable to open file for writing: " + logFilePath);
    }

    file << formatDataForFile(node->logs) << endl;
    }

    Logs parseDataFromFile(const string& line) const {
        stringstream ss(line);
        string token;
        Logs logs;       
        try {
            //As the files were formated
            getline(ss, token, ',');
            logs.timestamp = stod(token);
            
            getline(ss, token, ',');
            logs.gps.latitude = stod(token);
            
            getline(ss, token, ',');
            logs.gps.longitude = stod(token);
            
            getline(ss, token, ',');
            logs.gps.altitude = stod(token);
            
            getline(ss, token, ',');
            logs.bearing = stod(token);
            
            getline(ss, token, ',');
            logs.velocity = stod(token);
            
            getline(ss, token, ',');
            logs.acceleration = stod(token);
            
            getline(ss, token, ',');
            logs.temperature = stod(token);
            
            getline(ss, token, ',');
            logs.pressure = stod(token);
            
        } catch (const exception& e) {
            throw runtime_error("Error parsing data line: " + line);
        }
        
        return logs;
    }


    //core functions
    void addDataPoint(const Logs& logs){
        DataNode* newNode = new DataNode(logs);
        if(dataCount >= CACHE_SIZE){
            writeNodeToFile(head);
            DataNode* oldhead = head;
            head = head->next;
            if(head)
                head->prev = NULL;
            delete oldhead;
            dataCount--;
        }

        if(head == NULL)
            head = tail = newNode;
        else{
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        dataCount++;
    }

    // Data retrieval
    Logs* getLatestData() {
        if(tail)
            return &(tail->logs);
        else
            return NULL;
    }

    vector<Logs> getLastNReadings(int n) const {
        vector<Logs> readings;
        DataNode* current = tail;
        int count = 0;

        while (current && count < n) {
            readings.push_back(current->logs);
            current = current->prev;
            count++;
        }

        return readings;
    }

    //Analysis functions
    double AverageVelocity()const{
        if(!head)
            return 0.0;

        DataNode* current = head;
        double sum = 0.0;
        int count = 0;

        while(current){
            sum += current->logs.velocity;
            count++;
            current = current->next;
        }

        if(count == 0)
            return 0.0;
        else
            return sum/count;
    }

    double Distance(const Coordinates& coord1, const Coordinates& coord2) const {
        // Formula for distance between two GPS coordinates
        const double PI = 3.14159;
        const double R = 6371000; // Earth's radius in meters
        double lat1 = coord1.latitude * PI / 180;
        double lat2 = coord2.latitude * PI / 180;
        double deltaLat = (coord2.latitude - coord1.latitude) * PI / 180;
        double deltaLon = (coord2.longitude - coord1.longitude) * PI / 180;

        double a = sin(deltaLat/2) * sin(deltaLat/2) +
                  cos(lat1) * cos(lat2) *
                  sin(deltaLon/2) * sin(deltaLon/2);
        double c = 2 * atan2(sqrt(a), sqrt(1-a));

        return R * c;
    }

    double TotalDistance()const{
        if(!head)
            return 0.0;

        DataNode* current = head;
        double totalDistance = 0.0;

        while(current){
            totalDistance += Distance(current->logs.gps,current->next->logs.gps);
            current = current->next;
        }
        return totalDistance;
    }

    double MaxAltitude() const {
        if (!head) return 0.0;
        
        double maxAlt = head->logs.gps.altitude;
        DataNode* current = head->next;
        
        while (current) {
            if (current->logs.gps.altitude > maxAlt) {
                maxAlt = current->logs.gps.altitude;
            }
            current = current->next;
        }
        
        return maxAlt;
    }

    void clearCache() {
        DataNode* current = head;
        while (current) {
            DataNode* next = current->next;
            delete current;
            current = next;
        }
        head = tail = nullptr;
        dataCount = 0;
    }

    //File Operarions
    void SaveAllDataToFile(){
        ofstream file(logFilePath,ios::app);
        if(!file)
            throw runtime_error("Unable to open file for writing: " + logFilePath);

        DataNode* current = head;
        while(current){
            file << formatDataForFile(current->logs) << endl;
            current->next;
        }
    }

    void ReadFromFile(){
        ifstream file(logFilePath);
        if(!file)
            throw runtime_error("Unable to open file for writing: " + logFilePath);

        clearCache();
        string line;
        
        while (getline(file, line)) {
            try {
                Logs logs = parseDataFromFile(line);
                addDataPoint(logs);
            } catch (const exception& e) {
                cerr << "Error parsing line: " << e.what() << endl;
            }
        }
    }

    //Deconstructor
    ~DataLogger() {
        clearCache();
    }
};

#endif
