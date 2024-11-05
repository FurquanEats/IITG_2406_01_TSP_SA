#include <iostream>
#include <fstream>
#include <sstream> 
#include <vector>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <utility>
#include <numeric>

using namespace std;

struct City {
    int x, y;
};

double calculateDistance(const City &a, const City &b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

vector<City> loadCities(const string &filename) {
    ifstream infile(filename);
    vector<City> cities;
    
    if (!infile) {
        cerr << "Error: Unable to open file " << filename << endl;
        return cities;
    }

    string line;
    bool readCoordinates = false;
    while (getline(infile, line)) {
        if (line == "NODE_COORD_SECTION") {
            readCoordinates = true;
            continue;
        }
        if (line == "EOF") break;

        if (readCoordinates) {
            istringstream iss(line);
            int index, x, y;
            if (iss >> index >> x >> y) {
                cities.push_back({x, y});
            }
        }
    }
    infile.close();

    if (cities.empty()) {
        cerr << "Error: No cities were loaded from the file." << endl;
    } else {
        cout << "Loaded " << cities.size() << " cities from " << filename << endl;
    }
    
    return cities;
}

double calculateTourCost(const vector<int> &tour, const vector<City> &cities) {
    double totalCost = 0.0;
    for (size_t i = 0; i < tour.size() - 1; i++) {
        totalCost += calculateDistance(cities[tour[i]], cities[tour[i + 1]]);
    }
    totalCost += calculateDistance(cities[tour.back()], cities[tour[0]]);
    return totalCost;
}

vector<int> simulatedAnnealing(vector<int> &initialTour, const vector<City> &cities) {
    const int maxIterations = 10000;
    double temperature = 10000.0;
    double coolingRate = 0.999;

    vector<int> bestTour = initialTour;
    double bestCost = calculateTourCost(bestTour, cities);

    for (int i = 0; i < maxIterations; ++i) {
        vector<int> newTour = bestTour;
        int city1 = rand() % newTour.size();
        int city2 = (city1 + 1 + rand() % (newTour.size() - 1)) % newTour.size();
        swap(newTour[city1], newTour[city2]);

        double currentCost = calculateTourCost(newTour, cities);
        double delta = currentCost - bestCost;

        if (delta < 0 || (exp(-delta / temperature) > ((double)rand() / RAND_MAX))) {
            bestTour = newTour;
            bestCost = currentCost;
        }

        temperature *= coolingRate;
    }
    return bestTour;
}

int main() {
    srand(time(nullptr));

    string filename = "xqf131.tsp";
    vector<City> cities = loadCities(filename);

    if (cities.empty()) {
        cerr << "Exiting program as no cities were loaded." << endl;
        return 1;
    }

    vector<int> tour(cities.size());
    iota(tour.begin(), tour.end(), 0);
    random_shuffle(tour.begin(), tour.end());

    vector<int> bestTour = simulatedAnnealing(tour, cities);
    double bestCost = calculateTourCost(bestTour, cities);

    cout << "Final achieved cost: " << bestCost << "\n";
    cout << "Solution [sequence of cities]: ";
    for (int city : bestTour) {
        cout << city << " ";
    }
    cout << endl;

    return 0;
}
