#include "cardsman.h"
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
        //  default:
        //    return 'N';
    }
    if (suit == 'R')
        return 'R';
    if (suit == 'B')
        return 'B';
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
            cout << " 1 ";
        }
        cout << endl;
    }
    this->number = k;
    if (this->jokers)
    {
        deckTable[0][k / 4] = 1;
        deckTable[1][k / 4] = 0;
        deckTable[2][k / 4] = 1;
        deckTable[3][k / 4] = 0;
        this->number += 2;
        cout << deckTable[0][k / 4] << deckTable[1][k / 4] << deckTable[2][k / 4] << deckTable[3][k / 4] << endl;
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

//////////////

Player::Player(int i) { this->index = i; }; // конструктор инициализации

int Player::put(Card *x) // проверка существования. Если есть - удалить карту, вернуть 0
{
    int t = 1;
    Card *temp = this->curDeck.getHead();

    while (temp != NULL && t)
    {
        if (temp->getRank() == x->getRank() && temp->getSuit() == x->getSuit())
        {
            t = 0;
            break;
        }
        temp = temp->getNext();
    }
    if (t)
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

    delete temp;
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
    while (x->curDeck.getNumber() < this->deckType / this->gamers && main_Deck.getNumber())
    {
        while (this->main_Deck.deckTable[j = rand() % 4][k = rand() % (cardsInSuit)] == 0 && this->main_Deck.getNumber()) // находим ненулевую ячейку главной колоды
            ;
        if (this->main_Deck.getNumber())
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
    cout << " DONE " << endl;
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
    int j, flag_0 = 0, flag_pass = 0; // flag0 - была ли введена хотя бы 1 карта
    int allActive = 0;
    string answer;
    // cout << "\033[2J"; // очистка терминала

    //ход игрока
    while (!cur_player->submitted && allActive < cur_player->next->getDeck()->getNumber()) // пока нет сабмита и карт не больше, чем у противника
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
            inGameRank = curCard->getRank();    // будет подкидывать такие
        flag_0 = 1;                             // была введена карта
        allActive++;                            // количество карт которые положил игрок1
        if (flag_0 == 1 && this->givemore == 0) // версия игры без подкидываний, только 1 карта на ход
            break;
    } // конец хода нападающего

    //this->giveMore(cur_player); // в варианте без докидок оно здесь////////////////////////////

    cur_player->submit();

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
    //передача инициативы
    temp = next_player->getDeck()->getHead();

    //cout << "\033[2J"; // очистка терминала

    //начало хода защищающегося
    while (allActive) // пока не закрыты все карты на отбой
    {
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
            //   cout << "!!! ";
            next_player = next_player->next;               // текущий игрок пропускает ход
            cout << "next is " << next_player->getIndex(); //     cout << "P!!!";
            flag_pass = 1;
            break;
        }

        curCard = next_player->recognizeCard(card);
        // cout << " recognized ";
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
        //  cout << "check attack";
        if (!j)
        {
            cout << "This one can't help you" << endl;
            continue;
        }

        if (next_player->put(curCard)) // если такой на руке нет
        {
            continue; // перейти опять к считыванию карты
        }

        //  cout << " j ";
        if (j) // нашли позицию (индекс+1), которую бьет карта
        {
            // cout << "!!! ";
            allActive--;
            this->attackCards.addCard(curCard); //this->InGame.addCard(curCard);
        }
        // cout << "end it";
    }

    i = 1;
    Player *temp_player = next_player;
    if (flag_pass)
        while (temp_player->next->getIndex() != next_player->getIndex())
            temp_player = temp_player->next;
    while (cur_player->getIndex() != temp_player->getIndex())
    //for (i = 1; i <= this->getPlayers(); i++)
    {
        //cout << "\n er" << i << " FROM " << this->getPlayers();
        this->giveMore(cur_player);
        cur_player->submitted = 0;
        cur_player = cur_player->next;
        //cout << " #" << cur_player->getIndex() << endl;
        i++;
    }
    this->giveMore(temp_player);
    temp_player->submitted = 0;
    //cout << " ALL DONE ";
    //this->giveMore(next_player);

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
        cout << "removed one";
    }
    if (gamers == 1)
    {
        next_player->next = NULL;
        cout << " gamers = 1 ";
    }
    return next_player;
}
