#include "parser.h"

int main()
{
    /*ifstream logfile;

    string line;

    int loop = 0;

    int end[2];

    int temp = 0;

    char * dir = "C:/Games/Age of Conan/";

    while(true) {
        logfile.open("C:/Games/Age of Conan/CombatLog-2010-02-12_1928.txt");
        logfile.seekg(0, ios::end);
        temp = logfile.tellg();

        end[loop % 2] = temp;

        if(loop > 0) {
            if(loop % 2 == 0) {
                logfile.seekg(end[1]);
            } else if(loop % 2 == 1) {
                logfile.seekg(end[0]);
            }
        } else {
            logfile.seekg(0, ios::beg);
        }

        while(getline(logfile, line)) {
            cout << line << endl;
        }
        logfile.close();
        logfile.clear();

        //cout << loop << endl;
        loop++;
    }*/

    Parser * base = new Parser();
    base->OpenLogFile();
}
