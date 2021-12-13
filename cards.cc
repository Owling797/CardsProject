#include "cardsmanNEW.h"
using namespace std;

int main(int argc, char *argv[]) // -m - giveMore; -p - pass; -j - jokers; -v - addView; -va - addView (all)
{
    int giveMoreFlag = 0, passFlag = 0, jokersFlag = 0, addViewFlag = 0, compFlag = 0;
    int i;
    for (i = 0; i < argc; i++)
    {
        if (argv[i][0] == 'm')
            giveMoreFlag = 1;
        if (argv[i][0] == 'p')
            passFlag = 1;
        if (argv[i][0] == 'j')
            jokersFlag = 1;
        if (argv[i][0] == 'v') // view
            addViewFlag = 1;
        if (argv[i][0] == 'a') // all
            addViewFlag = 2;
        if (argv[i][0] == 'c') // with computer
            compFlag = 1;
    }
    if (compFlag)
    {
        giveMoreFlag = 0;
        passFlag = 0;
        jokersFlag = 0;
        addViewFlag = 0;
    }
    GeneralClass GenCl(giveMoreFlag, passFlag, jokersFlag, addViewFlag, compFlag);
    GenCl.gamePlay();

    return 0;
}
