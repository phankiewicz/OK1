#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <cmath>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;

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
double routesLength = 0;

vector<Customer> customersVector;

/*//Funkcja wypisujaca kontrolnie vector
void print_customersVector(vector<Customer> &v) {
    vector<Customer>::iterator it;
    for (it = v.begin(); it != v.end(); ++it) {
        cout << (*it).id << " " << (*it).x << " " << (*it).y << " " << (*it).demand << " " << (*it).ready_time << " " <<
        (*it).due_date << " " << (*it).service_duration << endl;
    }
}

void print_Customer(Customer v) {
    cout << v.id << " " << v.x << " " << v.y << " " << v.demand << " " << v.ready_time << " " << v.due_date << " " <<
    endl;
}

//Funkcja wypisujaca tablice savingsow
void print_savingsArray(Saving savingsArray[], unsigned long savingsamount) {
    unsigned long i;
    for (i = 0; i < savingsamount; ++i) {
        cout << savingsArray[i].saving << endl;
    }
}*/

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


//Funkcja obliczajaca dlugosc trasy miedzy dwoma punktami
double distance1(Customer point1, Customer point2) {

    //cout<<point1.x<<" "<<point1.y<<"  "<<point2.x<<" "<<point2.y<<endl;

    return sqrt(((point1.x - point2.x) * (point1.x - point2.x)) + ((point1.y - point2.y) * (point1.y - point2.y)));

}


vector<Saving> compute_savings() {

    Saving temp;
    Customer magazyn = customersVector.at(0);

    vector<Saving> savingsArray;

    for (unsigned i = 1; i < customersVector.size(); i++) {
        for (unsigned j = i + 1; j < customersVector.size(); j++) {
            //cout<<i<<"  "<<j<<endl;
            temp.i = customersVector.at(i);
            temp.j = customersVector.at(j);
            temp.saving = distance1(magazyn, temp.i) + distance1(magazyn, temp.j) - distance1(temp.i, temp.j);

            savingsArray.push_back(temp);
        }

    }
    return savingsArray;
}

/*void printRoute(vector<Customer> route) {
    for (Customer a:route) {
        printf("%i -> ", a.id);
    }
    printf("\n");

}

void printRoutes(vector<vector<Customer>> routes) {
    if (routes.empty())
        cout << "-1";
    for (auto r:routes) {
        printRoute(r);
    }
}*/


void printRoute2(vector<Customer> route) {
    for (unsigned long i = 1; i < route.size() - 1; ++i) {
        printf("%i ", route.at(i).id);
    }
    printf("\n");
}

void printRoutes2(vector<vector<Customer>> routes) {
    if (routes.empty())
        cout << "-1";
    for (auto r:routes) {
        printRoute2(r);
    }
}

vector<Customer> mergeRoute(vector<Customer> a, vector<Customer> b) {
    a.erase(a.end() - 1);
    b.erase(b.begin());

    vector<Customer> AB;
    //AB.reserve(a.size() + b.size());
    AB.insert(AB.end(), a.begin(), a.end()); // ♫ Really don't care
    AB.insert(AB.end(), b.begin(), b.end()); //…believe me, it's OK
    return AB;
}

//Funkcja sprawdzajaca poprawnosc trasy, gdy jest poprawna zwraca jej dlugosc
double isConnectionFeasible(vector<Customer> route) {
    int distanceSum = 0;
    int capacitySum = 0;

    //printRoute(route);

    for (unsigned long i = 1; i < route.size(); ++i) {
        //cout<<distanceSum<<endl;
        Customer it1 = route.at(i - 1);
        Customer it2 = route.at(i);
        //print_Customer(it1);
        //print_Customer(it2);
        //cout<<endl;
        if (distanceSum + distance1(it1, it2) <= it2.due_date) {
            distanceSum += distance1(it1, it2);
            if (distanceSum < it2.ready_time) {
                distanceSum = it2.ready_time;
            }
            distanceSum += it2.service_duration;
        }
        else
            return -1;
        if (capacitySum + it2.demand <= vehiclesCapacity)
            capacitySum += it2.demand;
        else
            return -1;

    }
    routesLength += distanceSum;
    //cout<<distanceSum<<endl;
    return distanceSum;

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
        vector<vector<Customer>>::iterator i = routes.begin(), j = routes.begin();
        for (vector<Customer> pre: routes) {
            for (vector<Customer> post: routes) {
                if (pre == post || pre.empty() || post.empty()) continue;
                if ((pre.at(pre.size() - 2) == s.i && post.at(1) == s.j) ||
                    (pre.at(pre.size() - 2) == s.j && post.at(1) == s.i)) {
                    vector<Customer> route = mergeRoute(pre, post);
                    if (isConnectionFeasible(route) != -1) {
                        if (i > j) {
                            routes.erase(i);
                            routes.erase(j);
                        } else {
                            routes.erase(j);
                            routes.erase(i);
                        }
                        routes.push_back(route);
                    }
                }
                ++j;
            }
            ++i;
        }
    }
    return routes;
}

high_resolution_clock::time_point startClock() {
    return high_resolution_clock::now();
}

long double endClock(high_resolution_clock::time_point timePoint) {
    auto timePoint2 = high_resolution_clock::now();
    nanoseconds span = duration_cast<nanoseconds>(timePoint2 - timePoint);
    return span.count();
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
    //print_customersVector(customersVector);


    //Uruchomienie procedury obliczania savingsow
    vector<Saving> savings = compute_savings();

    //Kontrolne wypisywanie tablicy savingsow
    //print_savingsArray(savingsArray,savingsamount);

    //Sortowanie tablicy savingsow
    sort(savings.begin(), savings.end(), sortcomparison);

    //Kontrolne wypisywanie tablicy savingsow
    //print_savingsArray(savingsArray,savingsamount);

    vector<vector<Customer>> routes = createNaiveRoutes();

    //printRoutes(routes);

    routes = performSavings(routes, savings);

    try {
        for (vector<Customer> i: routes) {
            if (isConnectionFeasible(i) == -1) {
                throw -1;
            }
        }
    } catch (int e) {
        printf("-1\n");
        return 0;
    }

    int sum = 0;
    for (vector<Customer> i: routes)
        sum += isConnectionFeasible(i);
    printf("%lu %i\n", routes.size(), sum);

    printRoutes2(routes);


    return 0;
}