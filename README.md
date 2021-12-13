# CardsProject
Cards Game “Durak” VERSION 2.0

GENERAL USAGE NOTES
-----------------------------------------------------
This programm is a card game imitator.
At the start of the game you have your own 6-cards deck from the main deck, which contains 4 suits, each suit contains 9 (6...A) or 13(2...A) cards. You also know a special trump suit. 
If you’re the first to take a step, to put a card down you need to choose the card and tap it’s suit and rank into the console. If there is already a player who admitted his turn and you need to defend some cards that the first player(s) put down, choose one of your cards, which can defend one of your opponent(s)’s put-down-cards,  and enter it into the console. When you've beaten every card from the “cards to beat” list, the initiative will go to you (you will be the first to take a step).  If it’s not possible to beat them, you need to enter ‘P’ and take cards to your deck.
*
Card (1) is able to defend another (2) if they’re the same suit and card’s (1) rank is bigger then card’s (2) or if card’s (1) suit is trump
*
The game can be played on the basic or additional settings.
The basic rules provide for the possibility to take a step with only one card.
The additional rules can be plugged in by adding some flags while starting a programm:

	m	“more” 
(You’ll be able to take a step with more than 1 card, but those cards must be the same rank)

	p	“pass”
(Despite beating cards-to-beat, you can enter some card from your deck that is the same rank as cards-to-beat. The fight-back-initiative is passed to the next player and you don’t need to defend cards anymore)

	j	“jokers”
(Starting the game with 2 additional cards BB (black joker) and RR(red joker). BB can defend any black suitted card, RR - any red suitted)

	v	“view”
(You’ll see your next opponent’s deck in each step)

	a	“view all”
(You’ll see decks of all of your opponents in each step)

	c	“computer”
(You will play one-on-one with the computer. Another additional rules are cancelled in this mode)




