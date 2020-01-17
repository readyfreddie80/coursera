
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <assert.h>
using namespace std;

void Reverse(vector<int>& v) {
    for (size_t i = 0; i < v.size() / 2; ++i) {
        swap(v[i], v[v.size() - i - 1]);
    }
}

#define newCmd(cmd) {#cmd, cmd}

enum cmdsNum {
    NEW_BUS,
    BUSES_FOR_STOP,
    STOPS_FOR_BUS,
    ALL_BUSES
};

void PrintVector(const vector<string>& v, char splitter) {
    for (const auto& s : v) {
        cout << s << splitter;
    }
}


int main() {
    int Q = 0;
    cin >> Q;

    map<string, size_t> cmds = {newCmd(NEW_BUS),
                                newCmd(BUSES_FOR_STOP),
                                newCmd(STOPS_FOR_BUS),
                                newCmd(ALL_BUSES)};

    map<string, vector<string>> busesForStops;
    map<string, vector<string>> stopsForBuses;
    string stop, bus;
    while (Q--) {
        string currCmd;
        cin >> currCmd;

        switch (cmds[currCmd]) {
            case NEW_BUS:
                int stopsCnt;
                cin >> stopsCnt >> bus;

                while (stopsCnt--) {
                    cin >> stop;
                    stopsForBuses[bus].push_back(stop);
                    busesForStops[stop].push_back(bus);
                }
                break;
            case BUSES_FOR_STOP:
                cin >> stop;
                if (busesForStops.count(stop) == 0) {
                    cout << "No stop" << endl;
                } else {
                    PrintVector(busesForStops[stop], '\n');
                }
                break;
            case STOPS_FOR_BUS:
                cin >> bus;
                if (stopsForBuses.count(bus) == 0) {
                    cout << "No bus" << endl;
                } else {
                    PrintVector(stopsForBuses[bus], '\n');
                }
                break;
            case ALL_BUSES:
                if (stopsForBuses.empty()) {
                    cout << "No buses" << endl;
                } else {
                    for (const auto &item : stopsForBuses) {
                        cout << item.first << ": ";
                        PrintVector(item.second, ' ');
                    }
                    break;
                    default :
                        break;
                }
        }
    }
}