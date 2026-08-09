#include <iostream>
#include <fstream>
#include <vector>

#include "task.hpp"

using namespace std;

int main() {
    string tasks_filename = getenv("HOME");
    tasks_filename += "/.poutine";
    ifstream tasks_file(tasks_filename);
    if (tasks_file.fail()) {
        cout << "Couldn't read poutine file at `" << tasks_filename << "`." << endl;
        // TODO: add instructions for writing one?
        return 0;
    }

    vector<task> tasks;
    string line;
    while (getline(tasks_file, line)) {
        if (!line.size() || all_of(line.begin(), line.end(), [](char c) { return isspace(c); }) || line.front() == '#')
            continue;
        auto t = task::parse(line);
        if (t.has_value())
            tasks.push_back(*t);
        else {
            cout << "Confusing line :" << line;
            continue;
        }
    }

    auto today = boost::gregorian::day_clock::local_day().day_of_week();
    cout << "Today is " << today.as_long_string() << endl;

    int i = 0;
    for (const auto& task : tasks) {
        if (!task.is_today(today))
            continue;
        cout << ' ' << ++i << ". " << task.name;
        if (task.is_time_based())
            cout << ' ' << task::timestamp(task.days[today]);
        else if (task.noun.has_value())
            cout << ' ' << task.days[today] << ' ' << *task.noun;
        cout << endl;
    }
}
