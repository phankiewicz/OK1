#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <unordered_map>

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
    int atPosition;
};

bool operator==(const Customer &me, const Customer &another) {
    return me.id == another.id;
}

class Saving {
public:
    int i, j;
    double saving;
};

class Route {
public:
    vector<Customer> route;
    int idL, idR;
    double len;
    int time;
    int cap;
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
        temp.atPosition = temp.id;
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
        temp.atPosition = temp.id;
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
    Customer magazyn = customersVector.at(0);
    Saving temp;
    vector<Saving> savingsArray;

    for (unsigned i = 1; i < customersVector.size(); i++) {
        for (unsigned j = i + 1; j < customersVector.size(); j++) {
            //cout<<i<<"  "<<j<<endl;
            temp.i = i;
            temp.j = j;
            temp.saving = distance1(magazyn, customersVector[i]) + distance1(magazyn, customersVector[j]) -
                          distance1(customersVector[i], customersVector[j]);

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


void printRoute2(pair<int, Route> route) {
    vector<Customer> r = route.second.route;
    for (unsigned long i = 1; i < r.size() - 1; ++i) {
        printf("%i ", r.at(i).id);
    }
    printf("\n");
}

void printRoute2(Route route) {
    vector<Customer> r = route.route;
    for (unsigned long i = 1; i < r.size() - 1; ++i) {
        printf("%i ", r.at(i).id);
    }
    printf("\n");
}

void printRoutes2(unordered_map<int, Route> routes) {
    if (routes.empty())
        cout << "-1";
    for (auto r:routes) {
        printRoute2(r);
    }
}

Route mergeRoute(Route a, Route b, Saving s) {
    vector<Customer> A = a.route;
    vector<Customer> B = b.route;
    A.erase(A.end() - 1);
    B.erase(B.begin());

    vector<Customer> AB;
    //AB.reserve(a.size() + b.size());
    AB.insert(AB.end(), A.begin(), A.end()); // ♫ Really don't care
    AB.insert(AB.end(), B.begin(), B.end()); //…believe me, it's OK

    Route result;
    result.route = AB;
    result.idR = b.idR;
    result.len = a.len + b.len - s.saving;
    result.cap = a.cap + b.cap;
    return result;
}

//Funkcja sprawdzajaca poprawnosc trasy, gdy jest poprawna zwraca jej dlugosc
double isConnectionFeasible(Route route) {
    int distanceSum = 0;
    int capacitySum = 0;

    /*printRoute2(route);
    distanceSum = (int) route.len;
    capacitySum = route.cap;

    printf("d=%i, c=%i\n", distanceSum, capacitySum);
    Customer cLast = route.route.at(route.route.size() - 2);

    printf("due-2=%i\n", cLast.due_date);
    if (distanceSum > cLast.due_date)
        return -1;

    cLast = route.route.at(route.route.size() - 1);
    printf("due-1=%i\n", cLast.due_date);
    if (distanceSum > cLast.due_date)
        return -1;

    printf("vCap=%i\n-------\n", vehiclesCapacity);
    if (capacitySum > vehiclesCapacity)
        return -1;
    /*/

    //printRoute(route);
    unsigned long size = route.route.size();

    for (unsigned long i = 1; i < size; ++i) {
        //cout<<distanceSum<<endl;
        Customer it1 = route.route.at(i - 1);
        Customer it2 = route.route.at(i);
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

unordered_map<int, Route> createNaiveRoutes() {
    unordered_map<int, Route> routes;
    for (Customer c:customersVector) {
        if (c.id != 0) {
            Route tmpRoute;
            c.atPosition = c.id;
            tmpRoute.route = {customersVector.at(0), c, customersVector.at(0)};
            tmpRoute.len = distance1(customersVector.at(0), c) + distance1(c, customersVector.at(0));
            tmpRoute.cap = c.demand;
            tmpRoute.idL = c.id;
            tmpRoute.idR = c.id;
            routes[c.id] = tmpRoute;
        }
    }
    return routes;
}

unordered_map<int, Route> performSavings(unordered_map<int, Route> routes,
                                         vector<Saving> savingsArray) {
    unsigned long size = savingsArray.size();
    for (int j = 0; j < size; ++j) {
        Saving s = savingsArray[j];
        Customer si = customersVector[s.i], sj = customersVector[s.j];
        if (si.atPosition < 0 || sj.atPosition < 0 || si.atPosition == sj.atPosition)
            continue;
        Route newRoute;
        long pocz = -1, kon = -1;
        if (si.atPosition == si.id) {
            Route r = routes[sj.atPosition];
            if (sj.atPosition != sj.id || r.route.size() == 3) {
                newRoute = mergeRoute(routes[sj.atPosition], routes[si.id], s);
                pocz = sj.atPosition;
                kon = si.id;
            }
        }
        else if (sj.atPosition == sj.id) {
            Route r = routes[si.atPosition];
            if (si.atPosition != si.id || r.route.size() == 3) {
                newRoute = mergeRoute(routes[si.atPosition], routes[sj.id], s);
                pocz = si.atPosition;
                kon = sj.id;
            }
        }
        if (isConnectionFeasible(newRoute) != -1 && pocz != -1 && kon != -1) {
            int last = newRoute.route[newRoute.route.size() - 2].id;
            int d = newRoute.route[1].id;
            customersVector.at((unsigned long) kon).atPosition = -1;
            customersVector.at((unsigned long) pocz).atPosition = -1;
            customersVector.at((unsigned long) last).atPosition = d;
            routes[pocz] = newRoute;
            routes.erase(kon);
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

void save(char *name, unordered_map<int, Route> routes) {
    //string filename=name;
    ofstream o;
    o.open(name, ofstream::out | ofstream::trunc);
    if (routes.empty())
        o << "-1\n";
    else {
        int sum = 0;
        for (pair<int, Route> i: routes) {
            Route x = i.second;
            sum += isConnectionFeasible(x);
        }
        printf("%lu %i\n", routes.size(), sum);

        printRoutes2(routes);
    }
}

int main(int argc, char *argv[]) {

    char *ile = nullptr, *fileName = nullptr, *outFileName = nullptr;

    if (argc < 2) {
        printf("Niewlasciwa liczba parametrow\n");
        return 1;
    }
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-n") {
            ile = argv[++i];
            continue;
        }
        if (arg == "-o") {
            outFileName = argv[++i];
            continue;
        }
        else
            fileName = argv[i];
    }

    if (ile == nullptr)
        data_input(fileName);
    else
        data_input_n(fileName, ile);

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

    unordered_map<int, Route> routes = createNaiveRoutes();

    //printRoutes(routes);

    routes = performSavings(routes, savings);

    try {
        for (pair<int, Route> i: routes) {
            Route x = i.second;
            if (isConnectionFeasible(x) == -1) {
                throw -1;
            }
        }
    } catch (int e) {
        printf("-1\n");
        routes.clear();
        if (outFileName != nullptr)
            save(outFileName, routes);
        return 0;
    }

    int sum = 0;
    for (pair<int, Route> i: routes) {
        Route x = i.second;
        sum += isConnectionFeasible(x);
    }
    printf("%lu %i\n", routes.size(), sum);

    printRoutes2(routes);

    if (outFileName != nullptr)
        save(outFileName, routes);
    return 0;
}
