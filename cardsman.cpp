#include "cardsmanNEW.h"
#include <ctime>
#include <random>
#include <string>

using namespace std;

void printSuit(char s) // черный: 30 текст, белый: 47 фон, красный: 31 текст \033[...m
{
    switch (s)
    {
    case 'H':
        cout << "\033[31;47m" /*это цвета*/
             << "\03";        // а это значок масти
        break;
    case 'C':
        cout << "\033[30;47m"
             << "\05";
        break;
    case 'D':
        cout << "\033[31;47m"
             << "\04";
        break;
    case 'S':
        cout << "\033[30;47m"
             << "\06";
        break;
    case 'R':
        cout << "\033[31;47m"
             << "\01";
        break;
    case 'B':
        cout << "\033[30;47m"
             << "\02";
        break;
    }
    //cout << s << "\033[0m";
}

Card::Card(char _suit, int _rank)
{ // конструктор инициализации
    suit = _suit;
    rank = _rank;
}

char translate(int n, char suit) // перевод номинала в текстовую нотацию (J, Q, K, A)
{
    char ans;
    switch (n)
    {
    case 11:
        return 'J';
    case 12:
        return 'Q';
    case 13:
        return 'K';
    case 14:
        return 'A';
    default:
        ans = 'N';
    }
    if (suit == 'R')
        return 'R';
    if (suit == 'B')
        return 'B';
    return ans;
}

/////////

void MainDeck::createTable(int k) // создание таблицы единиц по количеству карт в основной колоде
{
    deckTable = new int *[4];
    int i, j, n;
    n = k / 4;
    if (this->jokers)
        n++;
    for (i = 0; i < 4; i++)
    {
        deckTable[i] = new int[n]; // строк ровно по картам в каждой масти +1 для джокеров
    }
    for (i = 0; i < 4; i++) // заполнение единицами всех, кроме джокеров
    {
        for (j = 0; j < k / 4; j++)
        {
            deckTable[i][j] = 1;
            //            cout << " 1 ";
        }
        //      cout << endl;
    }
    this->number = k;
    if (this->jokers)
    {
        deckTable[0][k / 4] = 1;
        deckTable[1][k / 4] = 0;
        deckTable[2][k / 4] = 1;
        deckTable[3][k / 4] = 0;
        this->number += 2;
        //cout << deckTable[0][k / 4] << deckTable[1][k / 4] << deckTable[2][k / 4] << deckTable[3][k / 4] << endl;
    }
}

void MainDeck::changeTable(int s, int r) // обнуление ячейки таблицы единиц
{
    deckTable[s][r]--;
    this->number = this->number - 1;
}

//////////////

void GamersDeck::showDeck() // вывод колоды на экран
{
    Card *temp = this->head;
    while (temp != NULL)
    {
        if (temp->inGameFlag)
        {
            temp = temp->next;
            continue;
        }
        printSuit(temp->getSuit());
        cout << temp->getSuit();
        if (temp->getRank() > 10)
        {
            cout << translate(temp->getRank(), temp->getSuit()) << "\033[0m"
                 << " ";
        }
        else
            cout << temp->getRank() << "\033[0m"
                 << " ";
        temp = temp->next;
    }
    cout << "\033[0m";
    cout << endl;
}

void GamersDeck::addCard(char s, int r) // добавить в колоду карту по масти и номиналу (только next, prev)
{
    Card *temp = new Card(s, r);

    int n = this->getNumber();

    if (n == 0)
    {
        temp->prev = NULL;
        temp->next = NULL;
        this->head = temp;
        this->tail = this->head;
    }
    else
    {
        this->tail->next = temp;
        temp->next = NULL;
        temp->prev = this->tail;
        this->tail = temp;
    }
    this->increaseNumber();
}

void GamersDeck::addCard(Card &A) // добавить в колоду карту по ссылке на карту
{
    this->addCard(A.getSuit(), A.getRank());
}

void GamersDeck::addCard(Card *A) // добавить в колоду карту по указателю на карту
{
    this->addCard(A->getSuit(), A->getRank());
}

int GamersDeck::checkSort(int a = 0)
{
    int start = this->getHead()->getRank();
    char curSuit;
    Card *curCard = this->getHead();
    if (a)
    {
        curCard = this->getTail();
        start = curCard->getRank();
        curSuit = curCard->getSuit();
        while (curCard->getSuit() == curSuit)
        {
            if (curCard->getRank() > start)
                return 0;
            start = curCard->getRank();
            curCard = curCard->prev;
        }
        return 1;
    }

    while (curCard != NULL)
    {
        if (start > curCard->getRank())
            return 0;
        start = curCard->getRank();
        curCard = curCard->next;
    }
    return 1;
}
void GamersDeck::sortNum(char trumpSuit)
{
    if (this->cardsNumber < 4)
        return;
    int i = 0, notTrump, j;
    int tRank, tRank2, maxRank = 0, flag = 1;
    char tSuit, tSuit2, curSuit;
    Card *curCard = this->getHead();
    Card *tempCard = curCard->next;
    Card *tCard;

    curSuit = curCard->getSuit();
    while (curCard != NULL) //нашли наибольший
    {
        if (curCard->getRank() > maxRank)
        {
            maxRank = curCard->getRank();
            tempCard = curCard;
        }
        curCard = curCard->next;
    }

    // меняем с последним
    curCard = this->getTail();
    tRank = tempCard->getRank();
    tSuit = tempCard->getSuit();
    tempCard->setRank(curCard->getRank());
    tempCard->setSuit(curCard->getSuit());
    curCard->setRank(tRank);
    curCard->setSuit(tSuit);

    curCard = curCard->prev;
    while (curCard->prev->getRank() == curCard->getRank())
        curCard = curCard->prev;
    tempCard = this->getHead();
    while (curCard != this->getHead()) // сортировка по рангу
    {
        tempCard = this->getHead();
        flag = 1;
        while (!(tempCard->getSuit() == curCard->getSuit() && tempCard->getRank() == curCard->getRank()))
        {
            if (this->checkSort())
            {
                curCard = this->getHead();
                break;
            }
            if (tempCard->getRank() == maxRank)
            {
                flag = 0;
                if (tempCard->getRank() < curCard->getRank())
                {
                    while (curCard->prev->getRank() == curCard->getRank())
                        curCard = curCard->prev;
                    curCard = curCard->prev;
                }
                tRank = tempCard->getRank();
                tSuit = tempCard->getSuit();
                tempCard->setRank(curCard->getRank());
                tempCard->setSuit(curCard->getSuit());
                curCard->setRank(tRank);
                curCard->setSuit(tSuit);

                curCard = curCard->prev;
            }
            else
            {
                tempCard = tempCard->next;
            }
        }
        maxRank--;
    }

    // выталкиваем козыри в конец и по порядку
    tempCard = this->getHead();
    curCard = tempCard;
    tRank2 = 0;
    flag = 0;
    tCard = this->getTail();
    while (curCard != NULL)
    {
        if (curCard->getSuit() == trumpSuit)
        {
            if (curCard == tCard)
                flag = 2;
            if (flag == 2 && checkSort(1))
                break;
            if (!flag)
                tRank2 = curCard->getSuit();
            tCard = tCard->prev;

            tempCard = curCard;
            if (curCard->getRank() == tRank2)
                break;
            while (tempCard->next != NULL)
            {
                flag = 1;
                tSuit = tempCard->next->getSuit();
                tRank = tempCard->next->getRank();
                tempCard->next->setSuit(tempCard->getSuit());
                tempCard->next->setRank(tempCard->getRank());
                tempCard->setSuit(tSuit);
                tempCard->setRank(tRank);
                tempCard = tempCard->next;
            }
        }
        curCard = curCard->next;
    }
}

//////////////

Player::Player(int i) { this->index = i; };

int Player::put(Card *x, int comp = 0) // проверка существования. Если есть - удалить карту, вернуть 0
{
    int t = 1;
    Card *temp = this->curDeck.getHead();

    while (temp != NULL && t && !comp)
    {
        if (temp->getRank() == x->getRank() && temp->getSuit() == x->getSuit())
        {
            t = 0;
            break;
        }
        temp = temp->getNext();
    }
    if (t && !comp)
    {
        cout << "put:: You don't have this one" << endl;
        return 1;
    }

    if (temp->getRank() == this->curDeck.getHead()->getRank() && temp->getSuit() == this->curDeck.getHead()->getSuit())
    {
        if (this->curDeck.getNumber() > 1)
        {
            this->curDeck.setHead(temp->getNext());
            this->curDeck.getHead()->setPrev(NULL);
        }
        else
            this->curDeck.setHead(NULL);
    }
    else if (temp->getRank() == this->curDeck.getTail()->getRank() && temp->getSuit() == this->curDeck.getTail()->getSuit())
    {
        this->curDeck.setTail(temp->getPrev());
        this->curDeck.getTail()->setNext(NULL);
    }
    else
    {
        temp->getPrev()->setNext(temp->getNext());
        temp->getNext()->setPrev(temp->getPrev());
    }
    this->curDeck.decreaseNumber();

    return 0;
}

Card *Player::recognizeCard(char *card) // распознать карту
{
    int length = sprintf(card, "%s", card);

    if (length < 2 || length > 3)
    {
        cout << "incorrect input" << endl;
        Card *A = new Card('N', 0);
        return A;
    }
    char suit = card[0];
    if (!(suit == 'C' || suit == 'D' || suit == 'H' || suit == 'S'))
        suit = 'N';

    int rank = 0;
    if (isalpha(card[1]))
    {
        switch (card[1])
        {
        case 'J':
            rank = 11;
            break;
        case 'Q':
            rank = 12;
            break;
        case 'K':
            rank = 13;
            break;
        case 'A':
            rank = 14;
            break;
        default:
            rank = 0;
        }
    }
    else
    {
        //rank = static_cast<int>(card[1]); // работает ес че
        string temp_card(card);
        temp_card = temp_card.substr(1);
        rank = stoi(temp_card);

        if (length == 3 && temp_card != "10")
            rank = 0;
    }

    if (card[0] == 'R' && card[1] == 'R')
    {
        //cout << "\nJoker red\n";
        suit = 'R';
        rank = 15;
    }
    if (card[0] == 'B' && card[1] == 'B')
    {
        //cout << "\nJoker black\n";
        suit = 'B';
        rank = 15;
    }

    Card *A = new Card(suit, rank);
    return A;
}

/////////

Player *List::addPlayer(int d) // добавить в список игрока под следующим номером
{
    Player *temp = new Player(d);
    temp->next = NULL;
    if (this->head == NULL) //если создаем первый узел
    {
        this->head = temp;
        this->tail = head;
    }
    else //если узел уже не первый
    {
        this->tail->next = temp;
        temp->next = NULL;
        this->tail = temp;
    }
    return this->tail;
}

void GameRules::setTrumpSuit() // установить случайную козырную масть
{
    srand(time(0));
    char arr[4] = {'H', 'S', 'C', 'D'};
    this->trumpSuit = arr[rand() % 4];
}

void GameRules::giveMore(Player *x) // додать игроку до 6 карт
{
    int i, j, k, n;
    char arr[4] = {'C', 'D', 'H', 'S'};
    char s;
    int r;
    int cardsInSuit = (this->deckType) / 4;
    if (jokers)
        cardsInSuit++;
    n = x->getDeck()->getNumber();
    srand(time(0));
    while (x->curDeck.getNumber() < 6 /*this->deckType / this->gamers*/ && main_Deck.number)
    {
        while (this->main_Deck.deckTable[j = rand() % 4][k = rand() % (cardsInSuit)] == 0 && this->main_Deck.number) // находим ненулевую ячейку главной колоды
            ;
        if (this->main_Deck.number)
        {
            s = arr[j];                        // масть карты (в случае джокеров будут только arr[0] или arr[2])
            r = k + 15 - (this->deckType) / 4; //номер карты (6...14 или 2...14) или с джокерами (6...15 или 2...15)
            this->main_Deck.changeTable(j, k); // обнулить ячейку в главной колоде
            if (r == 15)
            {
                if (s == 'H')
                    s = 'R';
                if (s == 'C') //||s=='S')
                    s = 'B';
            }

            x->getDeck()->addCard(s, r);
        }
    }
    x->getDeck()->sortNum(this->trumpSuit);
}

Player *GameRules ::setPlayers() // задать игроков и раздать им карты
{
    this->playersList = List();
    //создание первого:
    Player *curPlayer = this->playersList.addPlayer(1); // вернули tail (последнего добавленного только что)

    int i;
    this->giveMore(curPlayer); // раздать карты

    // создание второго и последующих:
    for (i = 2; i <= this->gamers; i++)
    {
        curPlayer = curPlayer->next;
        curPlayer = playersList.addPlayer(i);
        this->giveMore(curPlayer); // раздать карты
    }

    curPlayer->next = playersList.getHead(); // зациклили
    return playersList.getHead();
}

void GameRules::removePlayer(Player *A) // удалить из списка игрока
{
    Player *temp = this->playersList.getHead();
    while (temp->next->getIndex() != A->getIndex())
    {
        temp = temp->next;
    }
    temp->next = A->next;
    this->decreasePlayers();
    if (winnersID == -1)
    {
        winnersID = A->getIndex();
        cout << "WINNER IS: #" << winnersID << endl;
    }
    delete A;
}

///////////////
void UserInterface::showInfoAttack(Player *cur_player, char trumpSuit)
{
    cout << endl
         << "Trump suit is:";
    printSuit(trumpSuit);
    cout << trumpSuit << "\033[0m" << endl; // козырь
    cout << "Your cards: ";
    cur_player->getDeck()->showDeck();
    cout << endl
         << "Enter a card or Submit (end) by pressing 'P'" << endl;
}

void UserInterface::showInfoDefence(Player *next_player, GamersDeck InGame, char trumpSuit)
{
    cout << endl
         << "Cards to beat: ";
    InGame.showDeck();
    cout << "Trump suit is:";
    printSuit(trumpSuit);
    cout << trumpSuit << "\033[0m" << endl; // козырь
    cout << "Your cards: ";
    next_player->getDeck()->showDeck();
    cout << "Enter a card or Pass (end) by pressing 'P'" << endl;
}

void UserInterface::showOpponentNext(Player *cur_player)
{
    cout << endl
         << "Next player's deck: ";
    cur_player->next->getDeck()->showDeck();
    //cout << endl;
}

void UserInterface::showOpponentAll(Player *cur_player)
{
    Player *crpl = cur_player->next;
    cout << endl;
    while (cur_player->getIndex() != crpl->getIndex())
    {
        cout << "Player's #" << crpl->getIndex() << " deck: ";
        crpl->getDeck()->showDeck();
        cout << endl;
        crpl = crpl->next;
    }
}

void UserInterface::results(int winner, int looser, int comp)
{
    cout << "\033[2J"; // очистка терминала
    int a = 0;
    //if (comp)
    //a = 2;
    cout << "RESULTS:" << endl
         << "Winner is: Player #" << winner + a << endl;
    cout << "Looser is: #" << looser + a / 2 << endl;
    if (comp)
    {
        if (winner == 1)
            cout << "you won, computer failed this time" << endl;
        else
            cout << "computer won this time" << endl;
    }
}
/////////////

void Game::sort() // отсортировать карты на отбой по масти
{
    if (InGame.getNumber() == 1)
        return;
    Card *curCard = InGame.getTail();
    char s = curCard->getSuit();
    char trSuit = trumpSuit;
    Card temp = Card(InGame.getHead()->getSuit(), InGame.getHead()->getRank());
    if (s == trSuit)
    {
        InGame.getHead()->setSuit(InGame.getTail()->getSuit());
        InGame.getHead()->setRank(InGame.getTail()->getRank());
        InGame.getTail()->setSuit(temp.getSuit());
        InGame.getTail()->setRank(temp.getRank());
    }
}

void Game::giveAll(Player *x) // отдать игроку все карты, находящиеся в игре
{
    Card *temp = this->InGame.getHead();
    Card *temp1;
    while (this->InGame.getNumber() > 1)
    {
        x->getDeck()->addCard(temp);
        this->InGame.setHead(temp->getNext());
        this->InGame.getHead()->setPrev(NULL);
        this->InGame.decreaseNumber();
        temp = temp->next;
    }
    x->getDeck()->addCard(temp);
    this->InGame.setHead(NULL);
    this->InGame.setTail(NULL);

    temp = this->attackCards.getHead();

    while (this->attackCards.getNumber() > 1)
    {
        x->getDeck()->addCard(temp);
        this->attackCards.setHead(temp->getNext());
        this->attackCards.getHead()->setPrev(NULL);
        this->attackCards.decreaseNumber();
        temp = temp->next;
    }
    if (this->attackCards.getNumber())
        x->getDeck()->addCard(temp);
    this->attackCards.setHead(NULL);
    this->attackCards.setTail(NULL);
}

int Game::check_attack(Card *A) // проверка, может ли карта A побить какую-либо из карт на отбой, найдется самая "сложная" карта из оставшихся
{
    int i = 0;
    Card *temp = this->InGame.getHead();
    while (temp != NULL)
    {
        if (A->getSuit() == 'R') // джокер красный
            if (temp->getSuit() == 'H' || temp->getSuit() == 'D')
            {
                temp->changeInGameFlag(); //inGameFlag = 1; // карта нашлась и побита но пока в игре
                return i + 1;
            }
        if (A->getSuit() == 'B') //джокер белый
            if (temp->getSuit() == 'C' || temp->getSuit() == 'S')
            {
                temp->changeInGameFlag(); //inGameFlag = 1; // карта нашлась и побита но пока в игре
                return i + 1;
            }
        if (A->getSuit() == temp->getSuit() && A->getRank() > temp->getRank() ||
            (A->getSuit() != temp->getSuit() && A->getSuit() == this->trumpSuit))
        {
            temp->changeInGameFlag(); //inGameFlag = 1; // карта нашлась и побита но пока в игре
            return i + 1;
        }
        i++;
        temp = temp->getNext();
    }
    return 0;
}
int Game::mechProtect(Player *next_player, GamersDeck InGame)
{
    Card *cardToBeat = InGame.getHead();
    Card *curCard = next_player->getDeck()->getHead();
    Card *tempCard = curCard;
    int flag = 0;
    int counter = InGame.getNumber();
    cout << counter << endl;
    while (curCard->getNext() != NULL)
    {
        cout << "!!! ";
        if (this->check_attack(curCard))
        {
            cout << "found ";
            counter--;
            flag = 1;
            tempCard = curCard;
            next_player->put(tempCard);
            break;
        }
        curCard = curCard->getNext();
    }

    if (this->check_attack(curCard) && !flag && curCard->getNext() == NULL)
    {
        cout << "!!! ";
        cout << "found add ";
        counter--;
        tempCard = curCard;
        next_player->put(tempCard);
    }

    if (counter)
    {
        return 0;
    }
    cout << counter << endl;
    return 1; //смог
}

int Game::mechAttack(Player *next_player, int opponentCards) // контроль возможности хода (количество карт одного ранга, чтобы бросить)
{
    Card *curCard = next_player->getDeck()->getHead();
    int counter = 1;
    curCard = curCard->next;
    int rank = curCard->getRank();
    while (curCard->next != NULL && counter < opponentCards && this->givemore)
    {
        if (curCard->getRank() == rank)
            counter++;
        else
            break;
        curCard = curCard->next;
    }
    return counter;
}
////////////////////////
GeneralClass::GeneralClass(int _gm, int _ps, int _j, int _v, int _c)
{
    gm = _gm;
    ps = _ps;
    j = _j;
    v = _v;
    comp = _c;
}

void GeneralClass::gamePlay()
{
    Player *cur_player, *next_player;
    int type, num;
    if (this->comp)
    {
        cout << "1 to 1 game with a computer. Enter deck type." << endl;
        cin >> type;
        num = 2;
    }
    else
    {
        cout << "Enter deck type and number of players" << endl;
        cin >> type >> num;
    }
    while (!(type == 36 && type >= 6 * num || type == 52 && type >= 6 * num))
    {
        cout << "Try again" << endl;
        cin >> type >> num;
    }

    Game CurGame(type, num, gm, ps, j, v, comp); // создали объект класса правил (весь)
    CurGame.setMainDeck();                       // создание таблицы-колоды
    CurGame.setTrumpSuit();                      //задали козырную масть
    cur_player = CurGame.setPlayers();           // создали циклично связанный список игроков (c кoлодами), получили первого

    //закончилось создание игроков//
    if (comp)
    {
        next_player = CurGame.Action(cur_player);
        cur_player = next_player;
    }
    while (cur_player->next != NULL && CurGame.getPlayers() > 1)
    {
        next_player = CurGame.Action(cur_player);
        cur_player = next_player;
    }

    UI.results(CurGame.winnersID, cur_player->getIndex(), comp);
}

Player *Game::Action(Player *cur_player) // основная функция игры
{
    int type, n, i, t;
    int cycleMembers = 0;                   // количество людей, переведших ход (<= 4)
    char inGameRank;                        // пушта правила
    Player *next_player = cur_player->next; // с этим есть возможность вернуться к атакующему для докидки
    string temp_card;
    char *card;
    Card *curCard;
    Card *temp = cur_player->getDeck()->getHead();
    Player *temp1 = cur_player;
    Card *cardsToAttack;
    int j, flag_0 = 0, flag_pass = 0; // flag0 - была ли введена хотя бы 1 картаи
    int allActive = 0;
    string answer;

    Card *tempMech;
    int numMech, flagC = 0;
    cout << "\033[2J"; // очистка терминала
    if (this->comp && next_player->getIndex() == 2)
        next_player->setC();
    //ход игрока
    while (!cur_player->submitted && allActive < cur_player->next->getDeck()->getNumber() && cur_player->getC() == 0) //(cur_player->getIndex() == 1 && !cur_player->getC() == 0 || !cur_player->getC() && !next_player->getC())) // пока нет сабмита и карт не больше, чем у противника
    {
        cout << endl
             << "Player# " << cur_player->getIndex();
        if (this->addView == 1) // крапленые карты
            UI.showOpponentNext(cur_player);
        if (this->addView == 2)
            UI.showOpponentAll(cur_player);
        this->UI.showInfoAttack(cur_player, trumpSuit);

        cin >> temp_card;
        card = (char *)temp_card.c_str();
        if (temp_card == "P")
        {
            if (flag_0 || cur_player->getDeck()->getNumber() == 0)
            {
                cur_player->submit();
                cout << "end of your turn" << endl;
                break;
            }
            else // была обработана ситуация пустого ввода (сабмит сразу, без карт)
            {
                cout << "Enter a card" << endl;
                continue;
            }
        }

        curCard = cur_player->recognizeCard(card);
        if (curCard->getRank() == 0 || curCard->getSuit() == 'N')
        {
            cout << "Try one more time" << endl;
            continue;
        }
        if (curCard->getRank() != inGameRank && flag_0)
        {
            cout << "Needed to be the same rank" << endl;
            continue;
        }
        if (cur_player->put(curCard))
        {
            continue;
        }
        this->InGame.addCard(curCard);
        this->sort();

        if (!flag_0)
            inGameRank = curCard->getRank(); // будет подкидывать такие
        flag_0 = 1;                          // была введена карта
        allActive++;                         // количество карт которые положил игрок1

        if (flag_0 == 1 && this->givemore == 0) // версия игры без подкидываний, только 1 карта на ход
            break;

    } // конец хода нападающего (человека)

    cout << cur_player->getIndex();
    if (cur_player->getC() && cur_player->getDeck()->getNumber() > 1)
    {
        i = 0;
        cur_player->getDeck()->sortNum(this->trumpSuit);
        numMech = mechAttack(cur_player, next_player->getDeck()->getNumber());
        cout << numMech;
        cur_player->getDeck()->showDeck();
        while (i < numMech) // комп ходит
        {
            cout << numMech << endl;
            flagC = 1;

            tempMech = cur_player->getDeck()->getHead();
            cout << cur_player->getDeck()->getHead()->getSuit() << cur_player->getDeck()->getHead()->getRank() << endl;
            cur_player->put(tempMech, 1);
            InGame.addCard(tempMech);
            cout << cur_player->getDeck()->getHead()->getSuit() << cur_player->getDeck()->getHead()->getRank() << "--->" << tempMech->getSuit() << tempMech->getRank() << "--->" << InGame.getHead()->getSuit() << InGame.getHead()->getRank() << endl;
            allActive++;
            tempMech = tempMech->next;
            i++;
        }
    }
    giveMore(next_player);
    cur_player->submit();
    if (cur_player->getDeck()->getNumber() == 1 && cur_player->getC() && !flagC)
    {
        next_player->next = NULL;
        return next_player;
    }
    if (cur_player->getDeck()->getNumber() == 0)
    {
        removePlayer(cur_player);
    }
    if (gamers == 1)
    {
        cout << "END OF THE GAME" << endl
             << "LOOSER IS: #" << next_player->getIndex() << endl;
        next_player->next = NULL;
        return next_player;
    }
    flag_0 = 0;

    temp = next_player->getDeck()->getHead();

    cout << "\033[2J"; // очистка терминала

    //начало хода защищающегося
    while (allActive) // пока не закрыты все карты на отбой
    {
        if (next_player->getC() && next_player->getIndex() == 2)
        {
            next_player->getDeck()->sortNum(this->trumpSuit);
            if (mechProtect(next_player, InGame)) // комп отбивается
            //отбился
            {
                giveMore(cur_player);
                giveMore(next_player);
                next_player->getDeck()->sortNum(this->trumpSuit);
                /**/ next_player->getDeck()->showDeck();
                allActive = 0;
                i = 0;
            }
            else // комп не отбился
            {
                giveAll(next_player);
                giveMore(next_player);
                allActive = 0;
                next_player = cur_player; // будет ходить человек
                break;
            }
            if (next_player->getDeck()->getNumber() == 1)
            {
                cout << "END OF THE GAME" << endl
                     << "LOOSER IS: MAN, #" << next_player->getIndex() << endl;
                cur_player->next = NULL;
                return cur_player;
            }
            break;
        }
        cout << endl
             << "Player# " << next_player->getIndex();
        if (this->addView == 1) // крапленые карты
            UI.showOpponentNext(next_player);
        if (this->addView == 2)
            UI.showOpponentAll(next_player);
        UI.showInfoDefence(next_player, InGame, trumpSuit);

        cin >> temp_card;
        card = (char *)temp_card.c_str();

        if (temp_card == "P") // Pass
        {
            this->giveAll(next_player);
            next_player = next_player->next;               // текущий игрок пропускает ход
            cout << "next is " << next_player->getIndex(); //     cout << "P!!!";
            flag_pass = 1;
            break;
        }

        curCard = next_player->recognizeCard(card);
        if (curCard->getRank() == 0 || curCard->getSuit() == 'N')
        {
            cout << "Enter a card" << endl;
            continue;
        }

        if (curCard->getRank() == InGame.getTail()->getRank() && this->pass) // возможен перевод хода
        {
            if (!flag_0 && next_player->next->getDeck()->getNumber() > InGame.getNumber())
            {
                cout << "Do you wanna pass? Press Y/N" << endl;
                cin >> answer;
                if (answer == "Y")
                {
                    if (next_player->put(curCard)) // если такой на руке нет
                    {
                        continue; // перейти опять к считыванию карты
                    }
                    next_player = next_player->next;
                    cycleMembers++;
                    allActive++;
                    InGame.addCard(curCard); // добавили подходящую на перевод
                    this->sort();
                    continue;
                }
            }
        }

        j = this->check_attack(curCard);
        if (!j)
        {
            cout << "This one can't help you" << endl;
            continue;
        }

        if (next_player->put(curCard)) // если такой на руке нет
        {
            continue; // перейти опять к считыванию карты
        }

        if (j) // нашли позицию (индекс+1), которую бьет карта
        {
            allActive--;
            this->attackCards.addCard(curCard);
        }
    }

    i = 1;
    Player *temp_player = next_player;
    if (flag_pass)
        while (temp_player->next->getIndex() != next_player->getIndex())
            temp_player = temp_player->next;
    while (cur_player->getIndex() != temp_player->getIndex())
    {
        this->giveMore(cur_player);
        cur_player->submitted = 0;
        cur_player = cur_player->next;
        i++;
    }
    this->giveMore(temp_player);
    temp_player->submitted = 0;

    while (InGame.getNumber() > 1)
    {
        Card *temp1 = InGame.getHead();
        InGame.setHead(InGame.getHead()->getNext()); //head = head->next
        InGame.getHead()->setPrev(NULL);
        InGame.decreaseNumber();
        delete temp1;
    }
    InGame.setHead(NULL);
    InGame.setTail(NULL);
    InGame.decreaseNumber();
    while (attackCards.getNumber() > 1)
    {
        Card *temp1 = attackCards.getHead();
        attackCards.setHead(attackCards.getHead()->getNext()); //head = head->next
        attackCards.getHead()->setPrev(NULL);
        attackCards.decreaseNumber();
        delete temp1;
    }
    if (attackCards.getNumber() == 1)
        attackCards.decreaseNumber();
    attackCards.setHead(NULL);
    attackCards.setTail(NULL);

    if (next_player->getDeck()->getNumber() == 0)
    {
        Player *tempPl = next_player;
        next_player = next_player->next;
        removePlayer(tempPl);
    }
    if (gamers == 1)
    {
        next_player->next = NULL;
    }

    return next_player;
}
