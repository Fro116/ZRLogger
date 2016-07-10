//
//  Card.cpp
//  Hearts
//
//  Created by Kundan Chintamaneni on 2/9/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#include "PlayingCard.h"

#include <iostream>
#include <string>

//enum PlayingCard::Suit PlayingCard::Suit() const {
//    return suit;
//}

//enum PlayingCard::Rank PlayingCard::Rank() const {
//    return rank;
//}

PlayingCard::PlayingCard(enum Rank rank, enum Suit suit) : rank(rank), suit(suit) {
 
}

const enum PlayingCard::Suit PlayingCard::suitCollection[4] = {Suit::HEARTS, Suit::SPADES, Suit::CLUBS, Suit::DIAMONDS};

const enum PlayingCard::Rank PlayingCard::rankCollection[13] = {Rank::TWO, Rank::THREE, Rank::FOUR, Rank::FIVE, Rank::SIX, Rank::SEVEN, Rank::EIGHT, Rank::NINE, Rank::TEN, Rank::JACK, Rank::QUEEN, Rank::KING, Rank::ACE};

std::string PlayingCard::ToString(enum PlayingCard::Suit suit) const {
    switch (suit) {
        case Suit::CLUBS:
            return "c";
            break;
        case Suit::DIAMONDS:
            return "d";
            break;
        case Suit::HEARTS:
            return "h";
            break;
        case Suit::SPADES:
            return "s";
            break;
        default:
            break;
    }
}

std::string PlayingCard::ToString(enum PlayingCard::Rank rank) const {
    switch (rank) {
        case Rank::TWO:
            return "2";
            break;
        case Rank::THREE:
            return "3";
            break;
        case Rank::FOUR:
            return "4";
            break;
        case Rank::FIVE:
            return "5";
            break;
        case Rank::SIX:
            return "6";
            break;
        case Rank::SEVEN:
            return "7";
            break;
        case Rank::EIGHT:
            return "8";
            break;
        case Rank::NINE:
            return "9";
            break;
        case Rank::TEN:
            return "10";
            break;
        case Rank::JACK:
            return "J";
            break;
        case Rank::QUEEN:
            return "Q";
            break;
        case Rank::KING:
            return "K";
            break;
        case Rank::ACE:
            return "A";
            break;
    }
}

std::string PlayingCard::ToString() const {
    return ToString(rank) + ToString(suit);
}

void PlayingCard::Print() const {
    std::cout << ToString() << "\n";
}

bool PlayingCard::operator==(const PlayingCard& card) const {
    return (suit == card.suit) && (rank == card.rank);
}
