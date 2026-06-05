#pragma once

#include <string>
#include <optional>
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace std;

class task {
public:
    string name;
    optional<string> noun; // a value of an empty string indicates task has a timestamp
    unsigned short days[7] = {0};

    static optional<task> parse(const string& line);
    static string timestamp(unsigned short n);

    void print() const;
    bool is_today(boost::gregorian::greg_weekday) const;
    bool is_time_based() const;

    void mark_as_time_based();
};
