#include "parser.h"


//constructor for the class
Parser::Parser()
{
    //eventually I should get this from the registry
    char buffer[256], secondary[256];
    HKEY reg_default, reg_value;
    //64bit first
    if(!RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Funcom\\Age of Conan", 0, KEY_QUERY_VALUE, &reg_default) == ERROR_SUCCESS) {
        //then default to 32bit
        RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Funcom\\Age of Conan", 0, KEY_QUERY_VALUE, &reg_default);
    }

    DWORD size = 255;
    RegQueryValueEx(reg_default, "LastInstalledClient", NULL, NULL, (LPBYTE)secondary, &size);

    directory = secondary;


    //get time for log finding!
    time_t rawtime;
    tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    year = (timeinfo->tm_year + 1900); //year
    month = (timeinfo->tm_mon + 1); //month
    day = timeinfo->tm_mday; //day
    hour = timeinfo->tm_hour; //hour
    minute = timeinfo->tm_min; //minute

    logfile = GetNewLog();
}

Parser::~Parser()
{
    delete &directory;
    delete &logfile;
}

string Parser::GetNewLog()
{
    DIR * dir = opendir(directory.c_str());
    struct dirent * ent; //= readdir(dir);

    vector<string> dir_list;
    string t_name;
    //CombatLog-2010-02-13_0127
    stringstream compare;
    if(month < 10)
        compare << "CombatLog-" << year << "-0" << month << "-" << day;
    else if(month < 10 && day < 10)
        compare << "CombatLog-" << year << "-0" << month << "-0" << day;
    else if(day < 10)
        compare << "CombatLog-" << year << "-" << month << "-0" << day;
    else
        compare << "CombatLog-" << year << "-" << month << "-" << day;

    while(ent = readdir(dir)) {
        t_name = ent->d_name;

        if(t_name.substr(0,20) == compare.str()) {
            dir_list.push_back(t_name);
        }
    }

    string first;
    int winner;
    for(int i = 0; i < dir_list.size(); i++) {
        if(i == 0) { first = dir_list[i].substr(22,25); winner = i; };

        if(dir_list[i].substr(22,25) > first) { first = dir_list[i].substr(22,25); winner = i; };
    }

    closedir(dir);

    delete ent;
    delete dir;
    return(dir_list[winner]);
}


void Parser::OpenLogFile()
{
    int temp = 0, loop = 0;
    int end[2];
    string line;

    while(true) {
        combatlog.open((directory+"/"+logfile).c_str());
        combatlog.seekg(0, ios::end);
        temp = combatlog.tellg();

        end[loop % 2] = temp;

        if(loop > 0) {
            if(loop % 2 == 0) {
                combatlog.seekg(end[1]);
            } else if(loop % 2 == 1) {
                combatlog.seekg(end[0]);
            }
        } else {
            combatlog.seekg(0, ios::beg);
        }

        while(getline(combatlog, line)) {
            cout << line << endl;
        }
        combatlog.clear();
        combatlog.close();

        //cout << loop << endl;
        loop++;
    }

}
