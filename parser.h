#ifndef _parser_h
#define _parser_h

#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <time.h>
#include <dir.h>
#include <dirent.h>
#include <sstream>
#include <windows.h>
#include <boost/regex.hpp>
#include <boost/timer.hpp>
#include <conio.h>
#include "inc/fmod.hpp"
#include "logline.h"

using namespace std;

class LogLine;

class Parser {
    public:
        Parser();
        ~Parser();

        string GetNewLog();
        void OpenLogFile();
        void UpdateDisplay();

    private:
        //director and filename (automatically guessed at)
        string directory;
        string logfile;
        int combatLength;
        bool inCombat;
        int combatTotal;
        int healingTotal;
        int starttime;

        //time information
        int day;
        int month;
        int year;
        int hour;
        int minute;

        boost::timer base;
        boost::timer runtime;

        FMOD::System *  s_system;
        FMOD::Sound  *  sound;
        FMOD::Channel * channel;
        FMOD_RESULT     result;

        //actual filestream
        ifstream combatlog;

        //fun stuff
        int experience;
        int damage;
        int healing;
};

#endif
