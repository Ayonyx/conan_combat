#ifndef _logline_h
#define _logline_h
#include "parser.h"
using namespace std;

class LogLine {
    public:
        LogLine(string);
        ~LogLine();
        void ParseLine();

        int GetExperience();
        int GetDamage();
        int GetHealing();
        string MatchLine(string);
        double GetValue();
        bool IsHealingOrDamage();
        string GetTarget();
        string GetPlayer();

    private:
        string oldline;
        vector<string> loglines;
        string player;
        string target;
        string ability;
        string value;
        bool damage_healing;
        int exp;
        int dmg;
        int heal;
};

#endif
