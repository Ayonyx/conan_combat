#include "logline.h"
#include <boost/regex.hpp>

LogLine::LogLine(string str)
{
    oldline = str.substr(11);
    exp = 0;
    dmg = 0;
    heal = 0;

    ParseLine();
}

LogLine::~LogLine()
{
    delete &loglines;
    delete &oldline;
    delete &player;
    delete &target;
    delete &value;
    delete &damage_healing;
    delete &ability;
}

void LogLine::ParseLine()
{
    using namespace boost;
    regex experience("You gained (\\d*) XP.$");
    regex white_damage("You (\\w*|critically\\s\\w*) (\\w*|\\w*\\s\\w*|\\w*\\s\\w*\\s\\w*) for (\\d*).$");
    regex combo_damage("Your (\\w*\\s\\w*\\s\\w{1}|\\w*\\s\\w*\\s\\w{2}|\\w*\\s\\w*\\s\\w{3}|\\w*\\s\\w*) (\\w*|critically\\s\\w*) (\\w*|\\w*\\s\\w*|\\w*\\s\\w*\\s\\w*) for (\\d*).$");
    regex healing_done("Your (\\w*\\s\\w*\\s\\(\\w{4}\\s\\d*\\)|\\w*) (\\w*) (\\w*) for (\\d*).$");
    regex healing("");
    smatch matches;

    int _exp = 0;
    int _dmg = 0;

    //check for experience gain
    if(regex_match(oldline, matches, experience)) {
        _exp = std::atoi((matches[1].str()).c_str()); // << endl;

        //cout << _exp << endl;
        exp = _exp;
    } else if(regex_match(oldline, matches, white_damage)) {
        _dmg = std::atoi((matches[3].str()).c_str());
        dmg = _dmg;
    } else if(regex_match(oldline, matches, combo_damage)) {
        _dmg = std::atoi((matches[4].str()).c_str());
        dmg = _dmg;
    } else if(regex_match(oldline, matches, healing_done)) {
        _dmg = std::atoi((matches[4].str()).c_str());
        heal = _dmg;
    }
}

int LogLine::GetExperience()
{
    return(exp);
}

string LogLine::MatchLine(string test)
{
    using namespace boost;
    smatch matches;
    regex comp(test);

    if(regex_match(oldline, matches, comp)) {
        return matches[1];
    }

    return "";
}

int LogLine::GetDamage()
{
    return(dmg);
}

int LogLine::GetHealing()
{
    return(heal);
}

double LogLine::GetValue()
{
}

bool LogLine::IsHealingOrDamage()
{
}

string LogLine::GetTarget()
{
}

string LogLine::GetPlayer()
{
}
