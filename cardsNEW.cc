#include "cardsmanNEW.h"
using namespace std;

int main()
{
    Player *cur_player, *next_player;
    int type, num;
    cout << "Enter deck type and number of players" << endl;

    cin >> type >> num;
    /* while (!(type == 36 && type >= 6 * num || type == 52 && type >= 6 * num))
    {
        cout << "Try again" << endl;
        cin >> type >> num;
    }*/

    Game CurGame(type, num);           // создали объект класса правил
    CurGame.setMainDeck();             // создание таблицы-колоды
    cur_player = CurGame.setPlayers(); // создали циклично связанный список игроков (c кoлодами), получили первого
    CurGame.setTrumpSuit();            //задали козырную масть
                                       //закончилось создание игроков//

    while (cur_player->next != NULL && CurGame.getPlayers() > 1)
    {
        next_player = CurGame.Action(cur_player);
        cur_player = next_player;
    }
    cout << "\033[2J"; // очистка терминала
    cout << "RESULTS:" << endl
         << "Winner is: Player #" << CurGame.winnersID << endl;
    cout << "Looser is: #" << cur_player->getIndex();

    return 0;
}