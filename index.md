## Card Game

Part of educational and technological practice at BMSTU

This programm is a card game "Durak" imitator.

### >> About
"Durak" is a game where you and your opponent(s) have some decks with cards from the main deck. You need to "beat" all cards which are your opponent(s) put down while their turn. If you were able to do it, your turn starts. You need to put down some of your cards according to rules, which your opponent(s) will try to beat. The goal is to get rid of all of your cards first.

### >> The game process

At the start of the game you have your own 6-cards deck from the main deck.

Main deck contains 4 suits, each suit contains 9 (6...A) or 13(2...A) cards.

You also know a special trump suit.

#### If you’re the first to take a step:

To put a card down you need to choose the card and tap it’s suit and rank into the console. 

#### If you’re the defending one:

...then there is already a player who admitted his turn and you need to defend some cards that the first player(s) put down

1. Choose one of your cards, which can beat one of your opponent(s)’s put-down-cards, and enter it into the console. 
2. When you've beaten every card from the “cards to beat” list, the initiative will go to you (you will be the first to take a step).  
3. If it’s not possible to beat them, you need to enter ‘P’ and take cards to your deck.

### >> Defence rules

Card (1) is able to beat another (2) if *they’re the same suit* and **card’s (1) rank is bigger then card’s (2)** or if **card’s (1) suit is trump**

### >> Features

- The game can be played on the basic or additional settings.
- The basic rules provide for the possibility to take a step with only one card.
- The additional rules can be plugged in by adding some flags while starting a programm:

	 m	“more” 
*You’ll be able to take a step with more than 1 card, but those cards must be the same rank*

	 p	“pass”
*Despite beating cards-to-beat, you can enter some card from your deck that is the same rank as cards-to-beat. The fight-back-initiative is passed to the next player and you don’t need to defend cards anymore*

	 j	“jokers”
*Starting the game with 2 additional cards BB (black joker) and RR(red joker). BB can defend any black suitted card, RR - any red suitted*

	 v	“view”
*You’ll see your next opponent’s deck in each step*

	 a	“view all”
*You’ll see decks of all of your opponents in each step*

	 c	“computer”
*You will play one-on-one with the computer. Other additional rules are cancelled in this mode (it will be fixed soon)*

### Requirements:
The following must be installed on the device:

- C++14

[Link](url) and ![Image](src)
