#include "cardsmanNEW.h"
#include <ctime>
#include <random>
#include <string>

using namespace std;

Card::Card(char _suit, int _rank)
{ // конструктор инициализации
    suit = _suit;
    rank = _rank;
}

char translate(int n) // перевод номинала в текстовую нотацию (J, Q, K, A)
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
    default:
        return 'N';
    }
}

/////////

void MainDeck::createTable(int k) // создание таблицы единиц по количеству карт в основной колоде
{
    deckTable = new int *[4];
    int i, j;
    for (i = 0; i < 4; i++)
    {
        deckTable[i] = new int[k / 4];
    }
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < k / 4; j++)
        {
            deckTable[i][j] = 1;
        }
    }
    this->number = k;
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
        cout << temp->getSuit();
        if (temp->getRank() > 10)
        {
            cout << translate(temp->getRank()) << " ";
        }
        else
            cout << temp->getRank() << " ";
        temp = temp->next;
    }
    cout << endl;
}

void GamersDeck::addCard(char s, int r) // добавить в колоду карту по масти и номиналу
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
    n = x->getDeck()->getNumber();
    srand(time(0));
    while (x->curDeck.getNumber() < 6 && main_Deck.getNumber())
    {
        while (this->main_Deck.deckTable[j = rand() % 4][k = rand() % ((this->deckType) / 4)] == 0 && this->main_Deck.getNumber()) // находим ненулевую ячейку главной колоды
            ;
        if (this->main_Deck.getNumber())
        {
            s = arr[j];                        // масть карты
            r = k + 15 - (this->deckType) / 4; //номер карты (6...14 или 2...14)
            this->main_Deck.changeTable(j, k); // обнулить ячейку в главной колоде

            x->getDeck()->addCard(s, r);
        }
    }
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

int Game::check_attack(Card *A) // проверка, может ли карта побить какую-либо из карт на отбой, найдется самая "сложная" карта из оставшихся
{
    int i = 0;
    Card *temp = this->InGame.getHead();
    while (temp != NULL)
    {
        if (A->getSuit() == temp->getSuit() && A->getRank() > temp->getRank() ||
            (A->getSuit() != temp->getSuit() && A->getSuit() == this->trumpSuit))
        {
            temp->changeInGameFlag(); //inGameFlag = 1; // карта побита но пока в игре
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
    char inGameRank;                        // пушта правила
    Player *next_player = cur_player->next; // с этим есть возможность вернуться к атакующему для докидки
    string temp_card;
    char *card;
    Card *curCard;
    Card *temp = cur_player->getDeck()->getHead();
    Player *temp1 = cur_player;
    Card *cardsToAttack;
    int j, flag_0 = 0; // flag0 - была ли введена хотя бы 1 карта
    int allActive = 0;
    // cout << "\033[2J"; // очистка терминала

    //ход игрока
    while (!cur_player->submitted && allActive < cur_player->next->getDeck()->getNumber()) // пока нет сабмита и карт не больше, чем у противника
    {
        cout << endl
             << "Player# " << cur_player->getIndex() << endl
             << "Trump suit is:" << this->trumpSuit << endl; // козырь
        cout << "Your cards: ";
        cur_player->getDeck()->showDeck();
        cout << endl
             << "Enter a card or Submit (end) by pressing 'P'" << endl;

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
    }                                        // конец хода нападающего

    this->giveMore(cur_player); // в варианте без докидок оно здесь
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

    //передача инициативы
    temp = next_player->getDeck()->getHead();

    //cout << "\033[2J"; // очистка терминала

    //начало хода защищающегося
    while (allActive) // пока не закрыты все карты на отбой
    {
        cout << endl
             << "Player# " << next_player->getIndex() << endl
             << "Cards to beat: ";
        this->InGame.showDeck();
        cout << "Trump suit is:" << this->trumpSuit << endl; // козырь
        cout << "Your cards: ";
        next_player->getDeck()->showDeck();
        cout << "Enter a card or Pass (end) by pressing 'P'" << endl;
        cin >> temp_card;
        card = (char *)temp_card.c_str();
        if (temp_card == "P") // Pass
        {
            this->giveAll(next_player);
            next_player = next_player->next; // текущий игрок пропускает ход
            break;
        }

        curCard = next_player->recognizeCard(card);
        if (curCard->getRank() == 0 || curCard->getSuit() == 'N')
        {
            cout << "Enter a card" << endl;
            continue;
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
            this->attackCards.addCard(curCard); //this->InGame.addCard(curCard);
        }
    }
    this->giveMore(next_player);

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
        next_player->next = NULL;
    return next_player;
}