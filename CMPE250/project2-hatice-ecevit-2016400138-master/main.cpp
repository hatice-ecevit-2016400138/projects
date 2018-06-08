#include <iostream>
using namespace std;

#include <queue>

#include <sstream>
#include <algorithm>
#include <fstream>
#include <iterator>

#include <vector>
#include <iomanip>

//this is the barista class
class Barista{
public:
    bool is_busy=false;
    double utilization_time;

public: Barista(){
        this->is_busy=false;
        this->utilization_time=0;
    }

};
//this is the customer class
struct Customer{

    //0:arrived

    //1:at the cashier

    //2:at the barista

    int currentSit=0;
    double arrival_time;
    double order_time;
    double brew_time;
    double price;
    double turnaround_time;
   //current time is the ending time of the process the customer is in
    double current_time;
    //where is for determining in which cashier or barista the customer is
    int where=-1;
    Customer(double arrivalTime, double orderTime, double brewTime, double price){
        this->arrival_time=arrivalTime;
        this->order_time=orderTime;
        this->brew_time=brewTime;
        this->price=price;
        this->current_time=arrivalTime;
        this->currentSit=0;
    }
    Customer(){

    }


};
//this is the cashie class
class Cashier{
public:
    bool is_busy=false;
    double utilization_time;

public: Cashier(){
        this->is_busy=false;
        this->utilization_time=0;
    }

};
//this for the time priority que
struct CompareToTime{
    bool operator()(Customer& cust1, Customer& cust2){
        return cust1.current_time>cust2.current_time;
    }
};
//this is for the barista priority que
struct CompareToPrice{
    bool operator()(Customer& cust1, Customer& cust2){
        return cust1.price<cust2.price;
    }
};


template <class Container>
void split1(const string& str, Container& cont)
{
    istringstream iss(str);
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(cont));
}
//this method returns the first cashier who's empty if no one is empty, returns -1
static int whichCashierEmpty(Cashier cashiers[],int N){
    for(int i=0; i<N; i++){
        if(cashiers[i].is_busy== false)
            return i;
    }
    return -1;
}
//this method returns the first barista who's empty if no one is empty, returns -1
static int whichBaristaEmpty(Barista baristas[],int N){
    for(int i=0;i<N;i++){
        if(baristas[i].is_busy==false){
            return i;
        }
    }
    return -1;
}




int main(int argc, char* argv[]) {
    // below reads the input file
    // in your next projects, you will implement that part as well
    if (argc != 3) {
        cout << "Run the code with the following command: ./project1 [input_file] [output_file]" << endl;
        return 1;
    }




    //  cout << "input file: " << argv[1] << endl;
    //cout << "output file: " << argv[2] << endl;


    ifstream infile(argv[1]);
    int N;
    int M;
    string line;

    // process first line
    getline(infile, line);
   // N is the number of cashiers
    N = stoi(line);
    getline(infile, line);
   //m is the number of customers
    M=stoi(line);
    // cout << "number of input lines: " << N << endl;
   //Customer array
    Customer customersFirst[M];
    //Cahier array
    Cashier cashiers[N];
    //Barista array
    Barista baristas[N/3];
    //time priority que
    priority_queue<Customer,vector<Customer>,CompareToTime> timeLine;
   //cashier que
    queue<Customer> cashierq;
    //barista priority que
    priority_queue<Customer,vector<Customer>,CompareToPrice> baristaq;
    //this for loop is adding the customers to the array
    for (int i=0; i<M; i++) {
        getline(infile, line);
        //  cout << "line: " << line << endl;

        vector<string> words;
        split1(line,words);
        Customer c(atof(words[0].c_str()),atof(words[1].c_str()),atof(words[2].c_str()),atof(words[3].c_str()));
        customersFirst[i]=c;
        timeLine.push(c);
    }
    //time is the total time
    double time=0;
    //this is for the customers who's procces is done
    Customer customers[M];
   //max length of the cashier queue
    int maxCashq=0;
   //max length of the barista queue
    int maxBarq=0;
  //k is the number of customer's who's process is finished
    int k=0;
    //this is the event simulation
    while(!timeLine.empty()){
        Customer current=timeLine.top();

        timeLine.pop();
        //incrementing time
        time=current.current_time;
        //customer is arrived
        if(current.currentSit==0){
            //there is no available cashier, pushing to cashier queue
            if(whichCashierEmpty(cashiers,N)==-1){
                cashierq.push(current);
                //updating the length
                if(cashierq.size()>maxCashq)
                    maxCashq=cashierq.size();
            }
            else{
                //finding the first empty cashier
                int x=whichCashierEmpty(cashiers,N);
                cashiers[x].is_busy=true;
                //updating the utilization time
                cashiers[x].utilization_time=cashiers[x].utilization_time+current.order_time;
                current.where=x;
                //processing the order
                current.current_time=current.current_time+current.order_time;
                current.currentSit=1;
                //adding to time que
                timeLine.push(current);
            }
        }
            //customer is finished at the cashier
        else if(current.currentSit==1){
            //there's a line for cashiers, popping from the line and sending it to cashier who is current leaving
            if(!cashierq.empty()){
                int x=current.where;
                Customer fromLine=cashierq.front();
                cashierq.pop();
                fromLine.where=x;
                fromLine.currentSit=1;
                fromLine.current_time=time+fromLine.order_time;
                cashiers[x].utilization_time+=fromLine.order_time;
                timeLine.push(fromLine);
            }
                //freeing the cashier
            else{
                int x=current.where;
                cashiers[x].is_busy=false;
            }
            //if there's no empty barista, pushing it tobarista queue
            if(whichBaristaEmpty(baristas,N/3)==-1){
                current.where=-1;
                baristaq.push(current);
                //updating length
                if(baristaq.size()>maxBarq)
                    maxBarq=baristaq.size();
            }
            else{
                //finding the first empty barista
                int x=whichBaristaEmpty(baristas,N/3);
                baristas[x].is_busy=true;
                baristas[x].utilization_time+=current.brew_time;
                current.currentSit=2;
                current.where=x;
                //updating time and sending it to time queue
                current.current_time+=current.brew_time;
                timeLine.push(current);
            }
        }
            //customer is leaving barista
        else if(current.currentSit==2){
            //there's a line for barista popping from the line and sending it to current's barista
            if(!baristaq.empty()){
                int x=current.where;
                Customer fromLine =baristaq.top();
                baristaq.pop();
                fromLine.where=x;
                fromLine.currentSit=2;
                fromLine.current_time=time+fromLine.brew_time;
                baristas[x].utilization_time+=fromLine.brew_time;
                timeLine.push(fromLine);
            }
                //freeing the barista if there's no line
            else{
                int x=current.where;
                baristas[x].is_busy=false;
            }
            //adding customer to the array of finished customers
            current.where=-1;
            current.turnaround_time=time-current.arrival_time;
            customers[k]=current;
            k++;
        }
    }


    ofstream myFile(argv[2]);

    myFile<<fixed<<setprecision(2);

    myFile<<time<<endl;

    myFile<<maxCashq<<endl;
    myFile<<maxBarq<<endl;


    for(int i=0;i<N;i++){
        myFile<<cashiers[i].utilization_time/time<<endl;
    }
    for(int i=0;i<N/3;i++){
        myFile<<baristas[i].utilization_time/time<<endl;
    }
    //sorting the finished customers in terms of their arrival time
    for(int i=0;i<M;i++){
        for(int j=i+1;j<M;j++){
            if(customers[i].arrival_time>customers[j].arrival_time){
                Customer temp=customers[i];
                customers[i]=customers[j];
                customers[j]=temp;
            }
        }
    }
    for(int i=0;i<M;i++){
        myFile<<customers[i].turnaround_time<<endl;
    }

    myFile<<""<<endl;

//adding the first customers to the time que again
    for(int i=0;i<M;i++){
        timeLine.push(customersFirst[i]);
    }
    //cashiers for the second case
    Cashier cashiers2[N];
    //baristas for the second case
    Barista baristas2[N/3];
    //vector of n/3 barista queue's
    vector <priority_queue<Customer,vector<Customer>,CompareToPrice>> baristaQ(N/3);
   //max length of cashir queue
    int maxCashQ2=0;
    //max length of each barista queue and setting the elements to zero
    int maxBarQ[N/3];
    for(int i=0;i<N/3;i++){
        maxBarQ[i]=0;
    }
    //total time
    double time2=0;
    //setting k=0 again
    k=0;
    while(!timeLine.empty()){
        Customer current=timeLine.top();

        timeLine.pop();
        time2=current.current_time;
        //customer has arrived
        if(current.currentSit==0){
            //there's no empty cashier, pushing it to cashier queue
            if(whichCashierEmpty(cashiers2,N)==-1){
                cashierq.push(current);
               //updating the length
                if(cashierq.size()>maxCashQ2)
                    maxCashQ2=cashierq.size();
            }
            else{
                //finding the first empty cashier
                int x=whichCashierEmpty(cashiers2,N);
                cashiers2[x].is_busy=true;
                //updating the utilization time
                cashiers2[x].utilization_time=cashiers2[x].utilization_time+current.order_time;
                current.where=x;
                //processing the order and sending it to time queue
                current.current_time=current.current_time+current.order_time;
                current.currentSit=1;
                timeLine.push(current);
            }

        }
            //customer is finished with the cashier
        else if(current.currentSit==1){
           //there's a line for cashier
            if(!cashierq.empty()){
                //popping from the line and sending it to current's cashier
                int x=current.where;
                Customer fromLine=cashierq.front();
                cashierq.pop();
                fromLine.where=x;
                fromLine.currentSit=1;
                fromLine.current_time=time2+fromLine.order_time;
                cashiers2[x].utilization_time=cashiers2[x].utilization_time+fromLine.order_time;
                timeLine.push(fromLine);
            }
            else{
                //setting the cashier free
                int x=current.where;
                cashiers2[x].is_busy=false;
            }
            //checking if it's barista is busy if it is pushing it to that barista's queue if not sending it to barista
            if(baristas2[current.where/3].is_busy){

                baristaQ[current.where/3].push(current);
                //updating length if that barista queue
                if(baristaQ[current.where/3].size()>maxBarQ[current.where/3])
                    maxBarQ[current.where/3]=baristaQ[current.where/3].size();
                //current.where=-1;
            }
            else{
                baristas2[current.where/3].is_busy=true;

                baristas2[current.where/3].utilization_time=current.brew_time+baristas2[current.where/3].utilization_time;
                current.currentSit=2;
                //changing current's where
                current.where=current.where/3;
                //processing the order and sending it to queue again
                current.current_time+=current.brew_time;
                timeLine.push(current);
            }

        }
            //customer is finished with the barista
        else if(current.currentSit==2){
            //checking its barista queue if it is not empty popping from the barista queue and sending it to barista
            if(!baristaQ[current.where].empty()){
                int x=current.where;
                Customer fromLine =baristaQ[x].top();
                baristaQ[x].pop();
                fromLine.where=x;
                fromLine.currentSit=2;
                fromLine.current_time=time2+fromLine.brew_time;
                baristas2[x].utilization_time+=fromLine.brew_time;
                timeLine.push(fromLine);
            }
            else{
                //freeing the barista
                int x=current.where;
                baristas2[x].is_busy=false;
            }
            //adding the finished customer to this array
            current.turnaround_time=time2-current.arrival_time;
            customers[k]=current;
            k++;

        }
    }


    myFile<<time2<<endl;
    myFile<<maxCashQ2<<endl;
    for(int i=0;i<N/3;i++){
        myFile<<maxBarQ[i]<<endl;
    }
    for(int i=0;i<N;i++){
        myFile<<cashiers2[i].utilization_time/time2<<endl;
    }
    for(int i=0;i<N/3;i++){
        myFile<<baristas2[i].utilization_time/time2<<endl;
    }
    //sorting the customers in terms of their arrival times
    for(int i=0;i<M;i++){
        for(int j=i+1;j<M;j++){
            if(customers[i].arrival_time>customers[j].arrival_time){
                Customer temp=customers[i];
                customers[i]=customers[j];
                customers[j]=temp;
            }
        }
    }

    for(int i=0;i<M;i++){
        myFile<<customers[i].turnaround_time<<endl;
    }

};