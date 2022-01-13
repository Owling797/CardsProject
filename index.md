## Card Game

Part of educational and technological practice at BMSTU

This programm is a card game "Durak" imitator.


### >> About
"Durak" is a game where you and your opponent(s) have some decks with cards from the main deck. You need to "beat" all cards which are your opponent(s) put down while their turn was. If you were able to do it, your turn starts. You need to put down some of your cards according to rules, which your opponent(s) will try to beat. The goal is to get rid of all of your cards first.
![AnimationMP](https://user-images.githubusercontent.com/62678416/149420610-04d0353d-2268-4996-9f88-4991acb701ee.gif)


### >> The game process

First you have to make a choice of settings when starting up. Then will be an offer to make a choice about the number of cards and players.

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

#### Defence rules

Card (1) is able to beat another (2) if *they’re the same suit* and *card’s (1) rank is bigger then card’s (2)* or if *card’s (1) suit is trump*

### >> Features

- The game can be played on the basic or additional settings.
- The basic rules provide for the possibility to take a step with only one card.
![Basic1](https://user-images.githubusercontent.com/62678416/149420794-416c9238-ff72-4e4b-91c9-8c86c8bb1a20.gif)

- The additional rules can be plugged in by adding some flags while starting a programm:

**m**	“more” 	 
*-You’ll be able to take a step with more than 1 card, but those cards must be the same rank:*

![AnimationM](https://user-images.githubusercontent.com/62678416/149419654-987b2df5-c639-493b-93e1-ae6072e75984.gif)


	 
**p**	“pass”	 
*-Despite beating cards-to-beat, you can enter some card from your deck that is the same rank as cards-to-beat. The fight-back-initiative is passed to the next player and you don’t need to defend cards anymore:*

![AnimationMP](https://user-images.githubusercontent.com/62678416/149420610-04d0353d-2268-4996-9f88-4991acb701ee.gif)



**j**   “jokers”	 
*-Starting the game with 2 additional cards BB (black joker) and RR(red joker) in the main deck.* 

*BB can defend any black suitted card, RR - any red suitted*

*That's how joker appears:*

![AnimationMPJ](https://user-images.githubusercontent.com/62678416/149419691-fe4925b7-96bb-4e38-91f1-533d82bd4bda.gif)



**v**	“view”	 
*-You’ll see your next opponent’s deck in each step:*

![AnimationView](https://user-images.githubusercontent.com/62678416/149421251-aeb23cee-cd66-47ea-88b9-f2d81bdf3489.gif)



**a**	“view all”	 
*-You’ll see decks of all of your opponents in each step:*

![AnimationViewAll](https://user-images.githubusercontent.com/62678416/149421271-70194469-022b-453f-bafa-4f501a1758d2.gif)



**c**	“computer”
	 
*You will play one-on-one with the computer. Other additional rules are cancelled in this mode.*


### Requirements:
The following must be installed on the device:

- C++14
