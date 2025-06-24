/*
Implement a multithreaded program in C++ that simulates two types of maintenance crews—an engine crew and a cleaning crew. 

Each crew should be represented by an independent thread and should operate based on work items provided through their respective task queues.

The program must satisfy the following conditions: A done flag should be shared across threads and can be triggered by the user entering 100 to stop the program.

Each thread runs continuously in a loop, checking its queue: If a task is available, the thread should process it and sleep for 1 second.

If no task is available, the thread should sleep for 2 seconds before checking again. The main thread acts as a "captain," continuously accepting user input and adding new work items to the appropriate queues every second without waiting for current tasks to complete.
*/


#include<iostream>
#include<string>
#include<thread>
#include<queue>
using namespace std;

volatile bool running = true;
volatile bool cleaningCrewAvailable = true;
volatile bool engineCrewAvailable = true;
queue<string> engineQueue;
queue<string> cleaningQueue;

void cleaningCrew(){
    while(running){
        if(cleaningCrewAvailable && !cleaningQueue.empty()){
            auto task = cleaningQueue.front();
            cout<<"Cleaning crew is working on the "<<task<<endl;
            this_thread::sleep_for(chrono::seconds(2));
            cout<<task<<" Cleaned."<<endl;
            cleaningQueue.pop();
        }else{
            this_thread::sleep_for(chrono::seconds(1));
        }
    }
}

void engineCrew(){
    while(running){
        if(engineCrewAvailable && !engineQueue.empty()){
            auto task = engineQueue.front();
            cout<<"Engine crew is working on the "<<task<<endl;
            this_thread::sleep_for(chrono::seconds(2));
            cout<<"Engine crew has worked on the "<<task<<" and good to go."<<endl;
            engineQueue.pop();
        }else{
            this_thread::sleep_for(chrono::seconds(1));
        }
    }
}

int main(){
    int command = 0;
    thread cleanthread(cleaningCrew);
    thread enginethread(engineCrew);

    cout<<"Command : \n"
        <<"1 for Cleaning Task\n2"
        <<" for Engine related Task\n"
        <<"100 to stop."
        <<endl;
    while(command != 100){
        cin>>command;
        if(command == 1){
            string cleaningTask;
            cout<<"Which area do you wanna clean ? "<<endl;
            cin>>cleaningTask;
            cleaningQueue.push(cleaningTask);
        }else if(command == 2){
            string engineTask;
            cout<<"Which task do you wanna do in engine ? "<<endl;
            cin>>engineTask;
            engineQueue.push(engineTask);
        }else if(command == 100){
            running = false;
            this_thread::sleep_for(chrono::seconds(4)); // By adding this pause if other threads were running we will give time to exit slowly.
            break;
        }else{
            cout<<"Invalid Operation"<<endl;
        }
    }
    
    cleanthread.join(); 
    enginethread.join();
}


/*
    Simple Program might not be the efficient way just understanding the flow.
    Future Ref to me:
        Here volatile is used because we dont want our thread to see the cached or lets say optimised version of the data in that variable so we have used volatile keyword.(Not the efficient but without mutex or anyother things for now its good. 

        These join statements at the end were needed because if someone exits the program thread might be running so unless the functions were throwing errors, normal join statements should be fine and good.If there is some exceptions throwing lets think about guards.
*/