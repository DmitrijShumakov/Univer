#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <conio.h>
#include <cstdlib>
#include <windows.h>

using namespace std;

char waitForKeypress() {
    char key;
    while (true) {
        if (_kbhit()) { 
            key = _getch(); 
            break;
        }
    }
    return key;
}

class Card {
public:
    string rank;
    string suit;

    Card(string r, string s) : rank(r), suit(s) {}

    int getValue(){
        if (rank == "Ace") {
            return 11;
        }
        else if (rank == "King" || rank == "Queen" || rank == "Jack") {
            return 10;
        }
        else {
            return stoi(rank);
        }
    }

    void display(){
        cout << rank << " of " << suit;
    }
};

class Deck {
public:
    vector<Card> cards;

    Deck() {
        string ranks[] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace" };
        string suits[] = { "Hearts", "Diamonds", "Clubs", "Spades" };

        for (int i = 0; i<4; i++) {
            for (int j = 0; j < 13; j++) {
                cards.push_back(Card(ranks[j], suits[i]));
            }
        }
        shuffle();
    }

    void shuffle() {
        srand(time(0));
        for (int i = 0; i < cards.size(); i++) 
        {
            int j = rand() % cards.size();
            swap(cards[i], cards[j]);
        }
    }

    Card pickCardFromDeck()
    {
        Card currentCard = cards.back();
        cards.pop_back();
        return currentCard;
    }
    
};

class User {
public:
    void setBalance() {
        cout << "Set your balance: ";
        
        while (cin >> balance)
        {
            if (balance > 0)
                break;
            else cout << "The balance is too small\nSet your balance: ";
        }
        cout << "\n";
    }
    void changeBalance(double coefficient) {
        balance += bid*coefficient;
    }
    void setBid() {
        cout << "Set your bid: ";
        while (cin >> bid)
        {
            if (bid <= balance && bid > 0)
                break;
            else if (bid > 0) cout << "Not enought money\nSet your bid: ";
            else cout << "The bid is too small\nSet your bid: ";
        }
        cout << "\n";
    }
    void showBalance() {
        cout << "Your balance: " << balance << "\n";
    }
    void showBid() {
        cout << "Your bid: " << bid << "\n";
    }
    void doubleBid() {
        bid *= 2;
    }
    
private:
    double balance;
    double bid;
};

class BlackjackGame {
public:

    double showInterface(User user) {
        system("cls");
        user.showBalance();
        user.showBid();
        cout << "Dealer's face-up card: ";
        dealerHand[0].display();
        cout << "\n\n";

        cout << "Player's hand: ";
        displayHand(playerHand);
        cout << "Total: " << getHandValue(playerHand) << "\n\n";


        if (getHandValue(playerHand) == 21) {
            cout << "Blackjack! You win!" << endl;
            system("pause");
            return 1.5;
        }
        else if (getHandValue(playerHand) > 21) {
            cout << "Bust! You lose." << endl;
            system("pause");
            return -1;
        }
        return 0;
    };

    double play(User user) {
        Deck deck;
        double multiple(0);
        playerHand.push_back(deck.pickCardFromDeck());
        playerHand.push_back(deck.pickCardFromDeck());
        dealerHand.push_back(deck.pickCardFromDeck());

        while (true) {
            multiple = showInterface(user);
            if (multiple != 0)
                return multiple;
            cout << "\tPress (s) to Hit (d) to Double or (w) to Stand.\n\n";
            char choice = waitForKeypress();
            switch (choice) {
            case 's':
                playerHand.push_back(deck.pickCardFromDeck());
                break;
            case 'd':
                user.doubleBid();
                playerHand.push_back(deck.pickCardFromDeck());
                multiple = showInterface(user);
                if (multiple != 0)
                    return multiple;
            case 'w':
                while (getHandValue(dealerHand) < 17) {
                    dealerHand.push_back(deck.pickCardFromDeck());
                }
                cout << "Dealer's hand: ";
                displayHand(dealerHand);
                cout << "Total: " << getHandValue(dealerHand) << endl;

                if (getHandValue(dealerHand) > 21 || getHandValue(dealerHand) < getHandValue(playerHand)) {
                    cout << "You win." << endl;
                    system("pause");
                    return 1;
                }
                else if (getHandValue(dealerHand) > getHandValue(playerHand)) {
                    cout << "Dealer wins." << endl;
                    system("pause");
                    return -1;
                }
                else {
                    cout << "It's a push." << endl;
                    return 0;
                }
                system("pause");
                break;
            default:
                break;
            }
        }
    }

private:
    vector<Card> playerHand;
    vector<Card> dealerHand;

    int getHandValue(vector<Card> hand) {
        int value = 0;
        int numOfAces = 0;
        for (int i = 0; i < hand.size(); i++) {
            value += hand[i].getValue();
            if (hand[i].getValue() == 11) {
                numOfAces++;
            }
        }

        while (value > 21 && numOfAces != 0) {
            value -= 10;
            numOfAces--;
        }
        return value;
    }

    void displayHand(vector<Card> hand) {
            hand[0].display();
            for (int i = 1; i < hand.size(); i++) {
                cout << ", ";
                hand[i].display();
        }
        cout << "\n";
    }

};

int main() {
    cout << "\tWelcome to Blackjack!\n" << endl;
    User user;
    user.setBalance();
    while (true) {
        system("cls");
        user.showBalance();
        user.setBid();
        BlackjackGame game;
        user.changeBalance(game.play(user));
    }
    return 0;
}
