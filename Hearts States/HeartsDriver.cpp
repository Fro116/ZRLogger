//
//  HeartsDriver.cpp
//  Hearts
//
//  Created by Kundan Chintamaneni on 2/21/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#include "HeartsDriver.h"

#include <iostream>
#include <memory>

#include "GameEngine.h"
#include "HeartsState.h"

#include "OpenGlPlayingCard.h"



HeartsDriver::HeartsDriver(long nanosPerFrame, int maxSkippedDrawSteps) : nanosPerFrame(nanosPerFrame), maxSkippedDrawSteps(maxSkippedDrawSteps), game("Hearts", 800, 600), offset(0), skippedDrawSteps(0) {
    time = std::chrono::steady_clock::now();
    
    PlayingCard card = PlayingCard(PlayingCard::Rank::KING, PlayingCard::Suit::HEARTS);
    OpenGlPlayingCard* cardImage =  new OpenGlPlayingCard(card);
    cardImage->MoveTo(glm::vec3(400,300,0));
    cardImage->ScaleBy(glm::vec3(100,100,0));
    cardImage->RotateTo(glm::angleAxis(glm::pi<float>()/4, glm::vec3(1,1,0)));
    std::shared_ptr<Bindable> cardObject(cardImage);
    
//    PlayingCard card2 = PlayingCard(PlayingCard::Rank::QUEEN, PlayingCard::Suit::SPADES);
//    OpenGlPlayingCard* cardImage2 =  new OpenGlPlayingCard(card2);
//    cardImage->MoveTo(glm::vec3(5,2,0));
//    cardImage2->ScaleBy(glm::vec3(2,2,2));
//    cardImage2->RotateTo(glm::quat(1,0,0.4,0));
//    std::shared_ptr<Bindable> cardObject2(cardImage2);
//    
    std::shared_ptr<HeartsState> state = HeartsState::CreateInstance(game.Window());
    state->BindObject(cardObject);
//    state->BindObject(cardObject2);
    game.PushState(state);
}

void HeartsDriver::Run() {
//    auto staticTime = std::chrono::steady_clock::now(); //Performance Test
    time = std::chrono::steady_clock::now();
    
    while (game.IsRunning()) {
        auto curTime = std::chrono::steady_clock::now();
        auto diffTime = std::chrono::duration_cast<std::chrono::nanoseconds>(curTime - time);
        long difference = diffTime.count();
        
        if ((difference + offset >= nanosPerFrame) && (skippedDrawSteps <= maxSkippedDrawSteps)) {
//            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(curTime-staticTime).count() <<"\n"; //Updates per millisecond
            
            offset = difference + offset - nanosPerFrame;
            time = curTime;

            game.Update();
            
            ++skippedDrawSteps;
        }
        else {
//            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(curTime-staticTime).count() <<"\n"; //Frames per millisecond
            skippedDrawSteps = 0;
            game.Draw(difference + offset);
        }
    }
}
