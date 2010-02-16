#include "parser.h"

int g_sec  = 1;
int g_min  = 60 * g_sec;
int g_hour = 60 * g_min;
int g_day  = 24 * g_hour;

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

    experience = 0;
    damage = 0;
    healing = 0;
    combatLength = 0;
    combatTotal = 0;
    healingTotal = 0;

    //result = FMOD::System_Create(&system);
    //system->setOutput(FMOD_OUTPUTTYPE_DSOUND);
    //channel = 0;

    //result = system->init(32, FMOD_INIT_NORMAL, 0);
    //result = system->createSound("Alarm.mp3", FMOD_HARDWARE, 0, &sound);

    //get time for log finding!
    time_t rawtime;
    tm * timeinfo;

    inCombat = false;

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
    int temp = 0, pausetime = 0;
    bool first = true;
    int old = 0;
    string line;
    int experience_gain = 0;
    starttime = 0;

    string miasma;

    while(true) {
        if(base.elapsed() >= 0.5) {
            combatlog.open((directory+"/"+logfile).c_str());
            combatlog.seekg(0, ios::end);
            temp = combatlog.tellg();

            if(temp == old && runtime.elapsed() - pausetime > 5.0) {
                inCombat = false;
                combatLength = 0;
                combatTotal = 0;
                healingTotal = 0;
            }

            if(temp == old) {
                continue;
            }

            if(!first) {
                combatlog.seekg(old);
            } else {
                combatlog.seekg(0, ios::beg);
            }

            old = temp;

            while(getline(combatlog, line)) {
                if(line.length() > 11) {
                    LogLine * current = new LogLine(line);

                    if(current->GetExperience() > 0) {
                        experience += current->GetExperience();
                    }

                    if(current->GetDamage() > 0) {
                        damage += current->GetDamage();
                        if(inCombat == false) {
                            inCombat = true;
                            starttime = runtime.elapsed();
                            combatLength += base.elapsed();
                            combatTotal += current->GetDamage();
                        } else {
                            pausetime = runtime.elapsed();
                            combatTotal += current->GetDamage();
                        }
                    }

                    if(current->GetHealing() > 0) {
                        healing += current->GetHealing();
                        if(inCombat) {
                            healingTotal += current->GetHealing();
                        }
                    }

                    /*miasma = current->MatchLine("Hathor-Ka's Miasma hits (\\w*) for (\\d*) poison damage.$");
                    if(miasma != "") {
                        cout << "============================================" << endl;
                        cout << miasma << endl;
                        cout << "============================================" << endl;
                    }*/

                    delete &current;
                }
            }
            combatlog.clear();
            combatlog.close();
            first = false;

            UpdateDisplay();
            base.restart();
        }
    }
}

void Parser::UpdateDisplay()
{
    system("CLS");

    cout << "Experience: " << experience << endl;
    cout << "Damage:     " << damage << endl;
    cout << "Healing:    " << healing << endl;
    cout << "---------------------------------------------------------------------------" << endl;
    if(inCombat) {
        printf("Combat:  %0.0f\n", runtime.elapsed() - starttime);
        printf("DPS: %0.0f\n", combatTotal / (runtime.elapsed() - starttime));
        printf("HPS: %0.0f\n", healingTotal / (runtime.elapsed() - starttime));
    } else {
        cout << "Out of Combat" << endl;
    }
    cout << "---------------------------------------------------------------------------" << endl;

    //int _seconds = runtime.elapsed() % g_min;
    //int _minutes =  runtime.elapsed() / 60;
    //int _hours   = runtime.elapsed() / 60*60;
    //printf("Runtime:    %0.0f:%0.0f:%0.0f\n", _hours, _minutes, _seconds);
}
