//
//  Card.h
//  Hearts
//
//  Created by Kundan Chintamaneni on 2/9/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#ifndef __Hearts__Card__
#define __Hearts__Card__

#include <string>

class PlayingCard {
public:
    enum class Suit {HEARTS, SPADES, CLUBS, DIAMONDS};
    enum class Rank {TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE};
    const static Suit suitCollection[4];
    const static Rank rankCollection[13];
    
    PlayingCard(Rank rank, Suit suit);
    
    bool operator==(const PlayingCard& card) const;
    
    //inlined for performance gain
    inline Suit Suit() const {
        return suit;
    }
    inline Rank Rank() const {
        return rank;
    }
    std::string ToString() const;
    void Print() const;
private:
    std::string ToString(enum Suit suit) const;
    std::string ToString(enum Rank rank) const;
    
    enum Suit suit;
    enum Rank rank;
};



#endif /* defined(__Hearts__Card__) */
