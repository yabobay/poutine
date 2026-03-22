#pragma once

#include <string>
#include <optional>
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace std;

class task {
public:
    string name;
    optional<string> noun;
    unsigned short days[7] = {0};

    static optional<task> parse(const string& line);
    static string timestamp(unsigned short n);

    void print() const;
    bool is_today(boost::gregorian::greg_weekday) const;
    bool is_time_based() const;
};
