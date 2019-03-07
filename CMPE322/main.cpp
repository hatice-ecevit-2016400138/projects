#include <iostream>
#include <vector>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdlib>



using namespace std;

void *client(void *param);
void *server(void *param);


vector<int> available_seats;
pthread_mutex_t available_seats_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t reserved_seats_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t file_lock = PTHREAD_MUTEX_INITIALIZER;
ofstream myFile("output.txt");

//This data structure is used in reserved seats to keep in track of which client asked for which seat.

struct client_seat{
    int client_id;
    int seat_num;
    client_seat(int c_id, int s_id){
        this->client_id = c_id;
        this->seat_num = s_id;
    }
};
vector <client_seat> reserved_seats;

//This method is used by server thread to find reserved seat for a certain client.

int findInReservedSeats(int client_id){
    int index = -1;
    for (int i = 0; i < reserved_seats.size(); i++){
        // taking the lock for reserved seats to search certain client id.
        pthread_mutex_lock(&reserved_seats_mutex);
        if (reserved_seats.at(i).client_id == client_id)
            index = i;
        pthread_mutex_unlock(&reserved_seats_mutex);
    }
    return index;
}

int main(int argc, char * argv[]) {
    int num_seats = atoi(argv[1]);

    //creating the seats vector.
    for (int i = 1; i <= num_seats; i++)
    {
        available_seats.push_back(i);
    }

    //creating the client number array for passing as parameter to client thread.
    int c_id[num_seats];
    for (int i = 1; i <= num_seats; i++){
        c_id[i-1] = i;
    }

    //creating thread id's for client threads.
    pthread_t clients[num_seats];
    myFile << "Number of total seats: " << num_seats << endl;

    //creating the client threads, giving the client number as parameter.
    for(int i = 0; i < num_seats; i++)
    {
        pthread_t tid;
        pthread_create(&tid,0,client,&c_id[i]);
        clients[i] = tid;
    }

    //joining all of the client threads using thread id array.
    for (int i = 0; i < num_seats; i++){
        pthread_join(clients[i],NULL);
    }
    myFile << "All seats are reserved." << endl;
    return 0;
}

void *client(void *param)
{
    int client_num = *(int *)param;
    int sleep_time = rand()%150 + 50;
    usleep(sleep_time);

    //creating the server thread
    pthread_t s_id;
    pthread_create(&s_id,0,server,&client_num);

    //taking the lock for available_seats since we use it on every line in this part.
    pthread_mutex_lock(&available_seats_mutex);
    int seat_num = rand()%available_seats.size();
    int seat = available_seats.at(seat_num);
    // deleting the selected seat from the available seats.
    available_seats.erase(available_seats.begin() + seat_num);
    pthread_mutex_unlock(&available_seats_mutex);

    // creating a pair to add it onto reserved seats.
    client_seat pair(client_num, seat);
    // taking the lock on reserved seats.
    pthread_mutex_lock(&reserved_seats_mutex);
    reserved_seats.push_back(pair);
    pthread_mutex_unlock(&reserved_seats_mutex);
    //client thread waits for server thread.
    pthread_join(s_id,NULL);
    pthread_exit(0);
}

void *server(void *param)
{
    //taking the client id as parameter.
    int client_id = *(int *)param;
    // idle until there's a request from the client thread.
    while (findInReservedSeats(client_id) == -1){
        //do nothing
    }
    // finding the selected seat and the pair.
    int index = findInReservedSeats(client_id);
    client_seat pair = reserved_seats.at(index);
    //getting the lock for file.
    pthread_mutex_lock(&file_lock);
    myFile << "Client" << pair.client_id << " reserves Seat" << pair.seat_num << endl;
    pthread_mutex_unlock(&file_lock);
    pthread_exit(0);
}
