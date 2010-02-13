#ifndef _parser_h
#define _parser_h

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <dir.h>
#include <dirent.h>
#include <sstream>
#include <windows.h>

using namespace std;

class Parser {
    public:
        Parser();
        ~Parser();

        string GetNewLog();
        void OpenLogFile();

    private:
        //director and filename (automatically guessed at)
        string directory;
        string logfile;

        //time information
        int day;
        int month;
        int year;
        int hour;
        int minute;

        //actual filestream
        ifstream combatlog;

        //fun stuff
        int experience;
        double damage;
        double healing;
};

#endif
