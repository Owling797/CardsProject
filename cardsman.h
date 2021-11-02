#ifndef CARDSMAN
#define CARDSMAN
#include <iostream>
#include <string>
#include <math.h>

class GamersDeck;
class GameRules;
class Game;

class Card // класс карты
{
protected:
    char suit;          //  масть карты в формате [S C H D]
    int rank;           // номинал карты в формате [6-10 11(J) 12(Q) 13 K 14 A]
    int inGameFlag = 0; // находится ли карта в текущей игре
    Card *next;         //следующая в колоде карта
    Card *prev;         //предыдущая в колоде карта

public:
    Card(){};
    Card(char, int);

    void changeInGameFlag() { inGameFlag = abs(inGameFlag - 1); }; // меняет флаг в игре->вне игры и наоборот
    int getInGameFlag() { return inGameFlag; }
    int getRank() { return rank; }  //возвращает номинал карты
    char getSuit() { return suit; } // возвращает масть карты
    void setNext(Card *p) { next = p; }
    void setPrev(Card *p) { prev = p; }
    void setRank(int r) { rank = r; }
    void setSuit(char s) { suit = s; }
    Card *getPrev() { return prev; }
    Card *getNext() { return next; }
    friend char translate(int, char); // перевод номинала в текстовую нотацию (J, Q, K, A)
    friend void printSuit(char);      // вывод значка и цвета вместо буквы
    friend GamersDeck;
    friend Game;
};

class Deck //класс базовой колоды
{
protected:
    int number;

public:
    Deck() { number = 0; };
    int getNumber() { return number; }
};

class MainDeck : public Deck //
{
protected:
    int **deckTable;
    int jokers;

public:
    MainDeck() : Deck(){};
    void createTable(int);      // создание таблицы единиц по количеству карт в основной колоде
    void changeTable(int, int); // обнуление ячейки таблицы

    friend GameRules;
};

class GamersDeck : public Deck //колода игрока
{
protected:
    Card *head;
    Card *tail;
    int cardsNumber = 0; // количество кард в колоде

public:
    GamersDeck() : Deck(){};
    Card *getHead() { return head; };
    Card *getTail() { return tail; };
    void setHead(Card *p) { head = p; };
    void setTail(Card *p) { tail = p; };
    void addCard(char, int); // добавить в колоду карту по масти и номиналу
    void addCard(Card &);    // добавить в колоду карту по ссылке на карту
    void addCard(Card *);    //добавить в колоду карту по укаазателю на карту
    int getNumber() { return cardsNumber; };
    void increaseNumber() { cardsNumber++; };
    void decreaseNumber() { cardsNumber--; };
    void showDeck(); // вывести колоду на экран
};

class Player // игрок
{
protected:
    int index;
    GamersDeck curDeck; // agregation

public:
    Player(){};
    Player(int);
    int submitted = 0;
    Player *next = NULL;

    int getIndex() { return index; };
    GamersDeck *getDeck() { return &curDeck; }
    Card *recognizeCard(char *);                       // распознать карту
    int put(Card *);                                   // проверка существования такой карты на руке и изменение в колоде игрока
    void submit() { submitted = abs(submitted - 1); }; //подтвердить конец хода
    void pass();                                       // пропустить ход и забрать карты в игре себе
    void setNext(Player *A) { next = A; }
    friend GameRules;
    // friend UserInterface;
};

class List // связный однонаправленный список игроков
{
private:
    Player *head;
    Player *tail;

public:
    List() { head = NULL, tail = NULL; }
    Player *getHead() { return head; };
    Player *getTail() { return tail; };
    Player *addPlayer(int); // добавить в список игрока под следующим номером
};

class GameRules // основные правила игры
{
protected:
    int deckType;
    int gamers;
    MainDeck main_Deck;
    int givemore = 1; // 0 - не подкидн; 1 - подкидн
    int pass = 1;     // 0 - не переводн; 1 - переводн
    int jokers = 1;
    int addView = 0; // 1 - следующий, 2 - все
    //int specTrump = 0;

public:
    GameRules(int type, int n)
    {
        deckType = type;
        gamers = n;
        main_Deck = MainDeck();
    };

    List playersList;   // связный список игроков
    char trumpSuit;     //козырная масть
    int winnersID = -1; // порядковый номер победителя

    void setMainDeck()
    {
        main_Deck.jokers = jokers;
        main_Deck.createTable(deckType);
    }                                    // создать основную колоду (банк) для заданного количества карт
    Player *setPlayers();                // задать игроков и раздать им карты
    int getType() { return deckType; }   // получить количество карт в исходной колоде
    void setTrumpSuit();                 // установить случайную козырную масть
    void giveMore(Player *);             // додать игроку до 6 карт
    void decreasePlayers() { gamers--; } // уменьшить на 1 текущее количество игроков в игре
    int getPlayers() { return gamers; }  // получить текущее количество игроков в игре
    void removePlayer(Player *);         // удалить из списка игрока
};

class UserInterface
{
public:
    void showOpponentNext(Player *);
    void showOpponentAll(Player *);
    void showInfoAttack(Player *, char);
    void showInfoDefence(Player *, GamersDeck, char);
    friend Game;
};

class Game : public GameRules // игра
{
public:
    Game(int t, int n) : GameRules(t, n) {}
    GamersDeck attackCards;   // список карт, введенных отбивающимся игроком
    GamersDeck InGame;        // список карт, которых нужно побить (карты на отбой)
                              // int cycleMembers = 0;
    void giveAll(Player *);   // отдать игроку все карты, находящиеся в игре
    void sort();              // отсортировать карты на отбой
    Player *Action(Player *); // основная функция игры
    int check_attack(Card *); // проверка, может ли карта побить какую-либо из карт на отбой
    UserInterface UI;
    //int passToNext();
};

class GeneralClass
{
private:
    int setDeck;
    void setPlayers();

public:
    void simpleMode();
};

#endif
