#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>

using namespace std;

class Customer{
public:
    int id;
    int x;
    int y;
    int demand;
    int ready_time;
    int due_date;
    int service_duration;
};

//Zmienne globalne
int vehiclesNumber;
int capacity;

vector<Customer> customersVector;

//Funkcja wypisujaca kontrolnie vector
void print_customersVector(vector<Customer>& v) {
    vector<Customer>::iterator it;
    for(it = v.begin(); it != v.end(); ++it) {
        cout<<(*it).id<<" "<<(*it).x<<" "<<(*it).y<<" "<<(*it).demand<<" "<<(*it).ready_time<<" "<<(*it).due_date<<" "<<(*it).service_duration<<endl;
    }
}


//Funkcja wczytujaca dane z pliku
void data_input(char * filename){

    string truncLine;
    fstream file;
    file.open(filename, fstream::in);

    Customer temp;

    //Omijanie wstepnych linii bez danych
    for(int i=0;i<4;i++)
        getline(file,truncLine);

    file>>vehiclesNumber>>capacity;

    //Wlaczenie wypisywania kontrolnego liczby ciezarowek i ich ladownosci
    //printf("%d %d\n", vehiclesNumber,capacity);

    //Omijanie kolejnych linii bez danych
    for(int i=0;i<4;i++)
        getline(file,truncLine);

    while(!file.eof())
    {
        file>>temp.id>>temp.x>>temp.y>>temp.demand>>temp.ready_time>>temp.due_date>>temp.service_duration;
        customersVector.push_back(temp);
    }

    //Wyrzucam ostatni element z vectora, bo ze wzgledu na pusta ostatnia linie pliku powtarza sie ostatni customer
    customersVector.pop_back();

    //Wlaczanie kontrolnego wypisywania vectora
    //print_customersVector(customersVector);

    file.close();
}

int main(int args, char* argv[]) {


    //Wczytywanie danych
    data_input(argv[1]);


    
    return 0;
}