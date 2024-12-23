#include "Sensing.hpp"
#include <iostream>
#include <cassert> // using this for unit tests 
#include <cstdlib> // using this for system
#include <thread> // using this for sleep
#include <chrono> // for timing
#include <fstream> // for file handling
#include <sstream> // for string streams

#if BUILD_PLATFORM == LINUX
    #define CLEAR_SCREEN "clear"
#elif BUILD_PLATFORM == WINDOWS
    #define CLEAR_SCREEN "cls"
#endif

using namespace std;


/// @brief Sleeps for a certain amount of milliseconds
/// Credits to HighCommander4 on StackOverflow: https://stackoverflow.com/questions/4184468/sleep-for-milliseconds 
/// @param milliseconds time to sleep for in milliseconds
void sleep_for(unsigned int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void ShowFastList(RocketSensors::FastList the_list, string Name){
    RocketSensors::Node NodeBuffer = the_list.ReadSequential();
    cout << endl << Name << " -> ";
    for (int i =0; i<10; i++){
        if(NodeBuffer.SelectedType == RocketSensors::Scalar){
            cout << NodeBuffer.Data.Scalar << " -> ";
        }
        else if(NodeBuffer.SelectedType == RocketSensors::Vect_2D){
            cout << NodeBuffer.Data.Vect2D.GetX() << ", " << NodeBuffer.Data.Vect2D.GetY() << " -> ";
        }
        else {
            cout << NodeBuffer.Data.Vect3D.GetX() << ", " << NodeBuffer.Data.Vect3D.GetY() << ", " << NodeBuffer.Data.Vect3D.GetZ() << " -> ";
        }

        NodeBuffer = the_list.ReadSequential();
    }
}

void ShowDataWithinDeadline(RocketSensors::DeadlineStack the_stack, string Name){
    cout << endl << Name << " (latest ten, if within deadline):";
    for(int i=0; i<10; i++){
        try{
            RocketSensors::Node* Buffer = the_stack.Pop();
            if(Buffer->SelectedType == RocketSensors::Scalar){
                cout << Buffer->Data.Scalar << " <-> ";
            }
            else if(Buffer->SelectedType == RocketSensors::Vect_2D){
                Buffer->Data.Vect2D.ShowCartesian(); cout << " <-> ";
            }
            else {
                Buffer->Data.Vect3D.Show(); cout << " <-> ";
            }
        }
        catch(...){
            break;
        }
    }
}

int main(){
    // Declare example data 
    RocketPhysics::Vector2D ExampleA(22.3f, 31.4f, false, 'F');
    RocketPhysics::Vector2D ExampleB(100.0f, 1.1f, true, 'V');
    RocketPhysics::Vector3D ExampleC(200.0f, 201.0f, 202.0f, 'F');
    RocketPhysics::Vector3D ExampleD(200.0f, 0.2f, 1.2f, 'V');
    
    #ifdef ENABLE_UTEST
        // UNIT TESTS
        assert(abs(ExampleA.GetX() - 22.3f) < TEST_FLOAT_PRECISION);
        assert(abs(ExampleA.GetY() - 31.4f) < TEST_FLOAT_PRECISION);
        assert(ExampleA.GetCartesian()[0] == ExampleA.GetX());
        assert(ExampleA.GetCartesian()[1] == ExampleA.GetY());
        assert(ExampleA.GetRadial()[0] - 38.5129848233f < TEST_FLOAT_PRECISION);
        assert(ExampleA.GetRadial()[1] - 0.953263405f < TEST_FLOAT_PRECISION);
        assert(ExampleA.GetType() == 'F');

        assert(abs(ExampleB.GetX() - 45.3596121426f) < TEST_FLOAT_PRECISION);
        assert(abs(ExampleB.GetY() - 89.1207360061f) < TEST_FLOAT_PRECISION);
        assert(ExampleB.GetCartesian()[0] == ExampleB.GetX());
        assert(ExampleB.GetCartesian()[1] == ExampleB.GetY());
        assert(ExampleB.GetRadial()[0] - 100.0f < TEST_FLOAT_PRECISION);
        assert(ExampleB.GetRadial()[1] - 1.1f < TEST_FLOAT_PRECISION);
        assert(ExampleB.GetType() == 'V');

        assert(abs(ExampleC.GetX() - 200.0f) < TEST_FLOAT_PRECISION);
        assert(abs(ExampleC.GetY() - 201.0f) < TEST_FLOAT_PRECISION);
        assert(abs(ExampleC.GetZ() - 202.0f) < TEST_FLOAT_PRECISION);
        assert(ExampleC.GetAll()[0] == ExampleC.GetX());
        assert(ExampleC.GetAll()[1] == ExampleC.GetY());
        assert(ExampleC.GetAll()[2] == ExampleC.GetZ());
        assert(ExampleC.GetType() == 'F');

        assert(abs(ExampleD.GetX() - 200.0f) < TEST_FLOAT_PRECISION);
        assert(abs(ExampleD.GetY() - 0.2f) < TEST_FLOAT_PRECISION);
        assert(abs(ExampleD.GetZ() - 1.2f) < TEST_FLOAT_PRECISION);
        assert(ExampleD.GetAll()[0] == ExampleD.GetX());
        assert(ExampleD.GetAll()[1] == ExampleD.GetY());
        assert(ExampleD.GetAll()[2] == ExampleD.GetZ());
        assert(ExampleD.GetType() == 'V');

        cout << "ALL TESTS PASSED!" << endl;
    #endif

    // Showing results
    cout << "A: "; ExampleA.ShowCartesian();
    cout << endl << "B: "; ExampleB.ShowRadial();
    cout << endl << "C: "; ExampleC.Show();
    cout << endl << "D: "; ExampleD.Show();

    // Testing some operations
    cout << endl << "A+B: "; (ExampleA + ExampleB).ShowCartesian();
    cout << endl << "A-B: "; (ExampleA - ExampleB).ShowCartesian();
    cout << endl << "A*2: "; (ExampleA * 2.0f).ShowCartesian();
    cout << endl << "A/2: "; (ExampleA / 2.0f).ShowCartesian();
    cout << endl << "A*B: "; cout << ExampleA * ExampleB; // Dot product
    cout << endl << "A^B: "; cout << (ExampleA ^ ExampleB); // Cross product (2D)

    cout << endl << "C+D: "; (ExampleC + ExampleD).Show();
    cout << endl << "C-D: "; (ExampleC - ExampleD).Show();
    cout << endl << "C*2: "; (ExampleC * 2.0f).Show();
    cout << endl << "C/2: "; (ExampleC / 2.0f).Show();
    cout << endl << "C*D: "; cout << ExampleC * ExampleD; // Dot product
    cout << endl << "C^D: "; (ExampleC ^ ExampleD).Show(); // Cross product (3D)


    cout << endl << "Press any key to continue...";
    cin.get();

    std::ifstream file("./Sensing/PreviousLaunch.log"); 

    if (!file.is_open()) {
        std::cerr << "Error: Could not open the log file." << std::endl;
        return 1;
    }

    stringstream Buffer;

    ifstream infile("./Sensing/PreviousLaunch.log");

    if (!infile.is_open()) {
        cerr << "Error: Could not open log file " << endl;
        return 1;
    }
    string line, token, bufferA, bufferB;
    RocketSensors::FastList Information;
    char ScanFor;
    
    RocketSensors::BinarySearchTree Sensors;
    Sensors.create('P'); // Pitch
    Sensors.create('R'); // Roll
    Sensors.create('Y'); // Yaw
    Sensors.create('V'); // Velocity
    Sensors.create('A'); // Acceleration
    Sensors.create('H'); // Height

    // Data loading loop
    while (getline(infile, line)){
        stringstream data(line);

        while(data >> token){
            if(token == "Pitch"){ ScanFor = 'P'; continue; }
            if(token == "Roll"){ ScanFor = 'R'; continue; }
            if(token == "Yaw"){ ScanFor = 'Y'; continue; }
            if(token == "Velocity"){ ScanFor = 'V'; continue; }
            if(token == "Acceleration"){ ScanFor = 'A'; continue; }
            if(token == "Height"){ ScanFor = 'H'; continue;}
            if(token == "TimeStamp") { ScanFor = 'T'; continue; }

            if(ScanFor != 'T'){
                Information.Insert(stof(token));

                RocketSensors::Sensor* Root = Sensors.GetRoot();
                RocketSensors::Sensor* Next = Root;
                while(Next){
                    // cout << Next->getType() << ScanFor << endl;
                    if(Next->getType() == ScanFor){RocketSensors::TransferToDeadlineStack(Information, Next->Raw); break;}
                    Next = Sensors.Traverse(Next);
                    Sensors.Update();
                }
                cout << endl;
            }
        }
        
    }
    
    Information.ResetRead(); ShowFastList(Information, "List Head");

    Sensors.Update();
    RocketSensors::Sensor* Root = Sensors.GetRoot();
    RocketSensors::Sensor* Next = Root;
    while(Next){
        ShowDataWithinDeadline(Next->Raw, string(1, Next->getType()));
        Next = Sensors.Traverse(Next);
    }
    cout << endl;

    cout << endl;   return 0;
}