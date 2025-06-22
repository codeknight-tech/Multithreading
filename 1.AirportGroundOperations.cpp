/*
------------------------------------------------
Airport Ground Operations Simulation 
------------------------------------------------

Scenario: You're modeling a simplified airport ground control system. The Air Traffic Controller (main thread) coordinates actions by: 

Maintenance Crew - handles technical checks (non-blocking). 
Refueling Crew - refuels the airplane (blocking). 
Baggage Handlers - load and unload baggage (blocking). 

Commands: 
1 - Send maintenance crew (non-blocking). 
2 - Refuel the airplane (controller must wait). 
3 - Handle baggage (controller must wait). 
100 - Exit the system. 
Any other input - Display “Invalid operation”. 
*/

#include<iostream>
#include<thread>
using namespace std;

void Maintainance(){
    cout<<"The maintainance crew is arriving."<<endl;
    this_thread::sleep_for(chrono::seconds(2));
    cout<<"The maintainance crew has arrived"<<endl;
    this_thread::sleep_for(chrono::seconds(2));
    cout<<"Checked the technicals and clear to go.."<<endl;
}

void refuel(){
    cout<<"The Crew is refueling the airplane."<<endl;
    this_thread::sleep_for(chrono::seconds(2));
    cout<<"The Crew has refueled."<<endl;
}

void handling(){
    cout<<"The Crew is loading/Unloading the baggage. Please wait"<<endl;
    this_thread::sleep_for(chrono::seconds(2));
    cout<<"Handled and Good to go.."<<endl;
}

int main(){
    int command = 0;
    cout<<"1 - Send maintenance crew (non-blocking).\n2 - Refuel the airplane (controller must wait).\n3 - Handle baggage (controller must wait).\n100 - Exit the system. "<<endl;
    while(true){
        cin>>command;
        if(command == 1){
            thread mc(Maintainance);
            mc.detach();
        }else if(command == 2){
            thread rc(refuel);
            rc.join();
        }else if(command == 3){
            thread hc(handling);
            hc.join();
        }else if(command == 100){
            cout<<"Thank you. Have a nice day!!."<<endl;
            exit(0);
        }else{
            cout<<"Invalid Operation."<<endl;
        }
    }
}