#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
#include <list>
#include <cmath>
#include <cstdlib>
#include <algorithm>

using namespace std;

class Customer {
public:
    int id;
    int x;
    int y;
    int demand;
    int ready_time;
    int due_date;
    int service_duration;
};

bool operator==(const Customer &me, const Customer &another) {
    return me.id == another.id;
}

class Saving {
public:
    Customer i;
    Customer j;
    double saving;
};

//Zmienne globalne
int vehiclesNumber;
int vehiclesCapacity;

vector<Customer> customersVector;

//Funkcja wypisujaca kontrolnie vector
void print_customersVector(vector<Customer> &v) {
    vector<Customer>::iterator it;
    for (it = v.begin(); it != v.end(); ++it) {
        cout << (*it).id << " " << (*it).x << " " << (*it).y << " " << (*it).demand << " " << (*it).ready_time << " " <<
        (*it).due_date << " " << (*it).service_duration << endl;
    }
}

//Funkcja wypisujaca tablice savingsow
void print_savingsArray(Saving savingsArray[], unsigned long savingsamount) {
    unsigned long i;
    for (i = 0; i < savingsamount; ++i) {
        cout << savingsArray[i].saving << endl;
    }
}

bool sortcomparison(Saving i, Saving j) { return (i.saving > j.saving); }


//Funkcja wczytujaca dane z pliku w formacie solomona
void data_input(char *filename) {

    string truncLine;
    fstream file;
    file.open(filename, fstream::in);

    Customer temp;

    //Omijanie wstepnych linii bez danych
    for (int i = 0; i < 4; i++)
        getline(file, truncLine);

    file >> vehiclesNumber >> vehiclesCapacity;

    //Wlaczenie wypisywania kontrolnego liczby ciezarowek i ich ladownosci
    //printf("%d %d\n", vehiclesNumber,vehiclesCapacity);

    //Omijanie kolejnych linii bez danych
    for (int i = 0; i < 4; i++)
        getline(file, truncLine);

    while (!file.eof()) {
        file >> temp.id >> temp.x >> temp.y >> temp.demand >> temp.ready_time >> temp.due_date >> temp.service_duration;
        customersVector.push_back(temp);
    }

    //Wyrzucam ostatni element z vectora, bo ze wzgledu na pusta ostatnia linie pliku powtarza sie ostatni customer
    customersVector.pop_back();


    file.close();
}

//Funkcja wczytywania okreslonej liczby danych
void data_input_n(char *filename, char *data_quantity) {
    string truncLine;
    fstream file;
    file.open(filename, fstream::in);
    int n;

    Customer temp;

    //Omijanie wstepnych linii bez danych
    for (int i = 0; i < 4; i++)
        getline(file, truncLine);

    file >> vehiclesNumber >> vehiclesCapacity;

    //Wlaczenie wypisywania kontrolnego liczby ciezarowek i ich ladownosci
    //printf("%d %d\n", vehiclesNumber,vehiclesCapacity);

    //Omijanie kolejnych linii bez danych
    for (int i = 0; i < 4; i++)
        getline(file, truncLine);

    n = atoi(data_quantity);

    //Wczytywanie danych magazynu i n klientow

    for (int i = 0; i <= n && !file.eof(); i++) {
        file >> temp.id >> temp.x >> temp.y >> temp.demand >> temp.ready_time >> temp.due_date >> temp.service_duration;
        customersVector.push_back(temp);
    }


    //Wyrzucanie ostatniego elementu z vectora, gdy dotrzemy do konca pliku (ostatni element jest taki sam jak przedostatni)
    if (file.eof()) {
        customersVector.pop_back();
    }

    file.close();
}







/*
//Funkcja sprawdzajaca poprawnosc trasy
int computeRoute(list<Customer> routeNodesList) {
    list<Customer>:: iterator it1=routeNodesList.begin();
    list<Customer>::iterator it2=routeNodesList.begin();
    it2++;
    int distanceSum=0;
    int capacitySum=0;

    for(it2; it2!=routeNodesList.end();++it2) {
        if(distanceSum+distance(it1,it2)<=routeNodesList[it2].due_time){
            distanceSum+=distance(it1,it2);
            if(distanceSum<it2.ready_time){
                distanceSum=it2.ready_time;
            }
            distanceSum+=it2.service_duration;

        }
        else
            return -1;
        if(capacitySum+it2.demand<=vehiclesCapacity)
            capacitySum+=it2.demand;
        else
            return -1;

        it1++;
    }
    return distanceSum;
}
*/

//Funkcja obliczajaca dlugosc trasy miedzy dwoma punktami
double distance1(Customer point1, Customer point2) {

    //cout<<point1.x<<" "<<point1.y<<"  "<<point2.x<<" "<<point2.y<<endl;

    return sqrt(((point1.x - point2.x) * (point1.x - point2.x)) + ((point1.y - point2.y) * (point1.y - point2.y)));

}


void compute_savings(Saving savingsArray[]) {

    Saving temp;
    Customer magazyn = customersVector.at(0);

    unsigned long savingsposition = 0;

    for (unsigned i = 1; i < customersVector.size(); i++) {
        for (unsigned j = i + 1; j < customersVector.size(); j++) {
            //cout<<i<<"  "<<j<<endl;
            temp.i = customersVector.at(i);
            temp.j = customersVector.at(j);
            temp.saving = distance1(magazyn, temp.i) + distance1(magazyn, temp.j) - distance1(temp.i, temp.j);

            savingsArray[savingsposition] = temp;
            savingsposition++;
        }

    }

}

void printRoute(vector<Customer> route) {
    for (Customer a:route) {
        printf("%i -> ", a.id);
    }
    printf("\n");

}

void printRoutes(vector<vector<Customer>> routes) {
    for (auto r:routes) {
        printRoute(r);
    }
}

vector<Customer> mergeRoute(vector<Customer> a, vector<Customer> b) {
    a.erase(a.end() - 1);
    b.erase(b.begin());

    vector<Customer> AB;
    AB.reserve(a.size() + b.size());
    AB.insert(AB.end(), a.begin(), a.end()); // ♫ Really don't care
    AB.insert(AB.end(), b.begin(), b.end()); //…believe me, it's OK
    return AB;
}

bool isConnectionFeasible(vector<Customer> pre, vector<Customer> post) {
    return true; //TODO czy się opłaca i czy można połączyć pre + post
}


vector<vector<Customer>> createNaiveRoutes() {
    vector<vector<Customer>> routes;
    for (Customer c:customersVector) {
        if (c.id != 0) {
            vector<Customer> tmpRoute = {customersVector.at(0), c, customersVector.at(0)};
            routes.push_back(tmpRoute);
        }
    }
    return routes;
}

vector<vector<Customer>> performSavings(vector<vector<Customer>> routes, vector<Saving> savingsArray) {
    for (Saving s: savingsArray) {
        vector<Customer> pre, post;
        vector<vector<Customer>>::iterator preIndex, postIndex;
        vector<vector<Customer>>::iterator i = routes.begin();
        for (vector<Customer> route: routes) {
            if (route.at(1) == s.j) {
                post = route;
                postIndex = i;
            } else if (route.at(route.size() - 2) == s.i) {
                pre = route;
                preIndex = i;
            }
            if (!pre.empty() && !post.empty() && isConnectionFeasible(pre, post)) break;
            ++i;
        }
        if (!pre.empty() && !post.empty()) {
            vector<Customer> newRoute = mergeRoute(pre, post);
            if (preIndex > postIndex) {
                routes.erase(preIndex);
                routes.erase(postIndex);
            } else {
                routes.erase(postIndex);
                routes.erase(preIndex);
            }
            routes.push_back(newRoute);
        }
    }
    return routes;
}

vector<Saving> convert2vector(Saving savingArr[], unsigned long n) {
    vector<Saving> savings;
    for (int i = 0; i < n; ++i) {
        savings.push_back(savingArr[i]);
    }
    return savings;
}

int main(int argc, char *argv[]) {

    if (argc == 2) {
        //Wczytywanie danych
        data_input(argv[1]);

    }
    else if (argc == 3) {
        //Wczytywanie okreslonej ilosci danych
        data_input_n(argv[1], argv[2]);
    }
    else {
        printf("Niewlasciwa liczba parametrow\n");
        return 1;
    }

    //Wlaczanie kontrolnego wypisywania vectora
    print_customersVector(customersVector);

    //Wyliczenie dlugosci tablicy savingsow
    unsigned long savingsamount = (customersVector.size() * (customersVector.size() - 1)) / 2;

    //Deklaracja tablicy savingsow
    Saving savingsArray[savingsamount];

    //Uruchomienie procedury obliczania savingsow
    compute_savings(savingsArray);

    //Kontrolne wypisywanie tablicy savingsow
    //print_savingsArray(savingsArray,savingsamount);

    //Sortowanie tablicy savingsow
    sort(savingsArray, savingsArray + savingsamount, sortcomparison);

    //Kontrolne wypisywanie tablicy savingsow
    //print_savingsArray(savingsArray,savingsamount);

    vector<vector<Customer>> routes = createNaiveRoutes();

    //printRoutes(routes);

    vector<Saving> savings = convert2vector(savingsArray, savingsamount);

    routes = performSavings(routes, savings);

    //printRoutes(routes);
    return 0;
}