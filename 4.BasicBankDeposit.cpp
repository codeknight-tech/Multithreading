#include<iostream>
#include<thread>
#include<atomic>
#include<chrono>
#include<vector>
using namespace std;
 
atomic<int> TotalDeposited = 0;
thread_local int amount = 0;
 
void DepositMoney(int money, int times){
    for(int i = 0; i < times; i++){
        TotalDeposited += money;
        amount += money;
        this_thread::sleep_for(chrono::seconds(2));
    }
    cout<<"The thread "<< this_thread::get_id()<<" deposited the amount Rs. "<<amount<<endl;
}
 
 
int main(){
    vector<thread> threads;
    threads.emplace_back(thread(DepositMoney, 400, 2));   // 2 * 400 => 800. This thread deposits Rs.800.
    threads.emplace_back(thread(DepositMoney, 1000, 7));  // 7 * 1000 => 7000. This thread deposits Rs.7000.
    threads.emplace_back(thread(DepositMoney, 50, 5));    // 2 * 50 => 250. This thread deposits Rs.250.
                                                        // Total Deposited Rs.8050.
    for(auto it = threads.begin(); it != threads.end(); ++it){
        if((*it).joinable()) (*it).join();
    }
 
    cout<<"Total Deposited amount : "<< TotalDeposited<<endl;
 
}