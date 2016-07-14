#include "ZRDriver.h"

#include "ZeldaInformationHandler.h"

ZRDriver::ZRDriver(std::string title, int width, int height, long nanosPerFrame, int maxSkippedDrawSteps) : GameDriver(title, width, height, nanosPerFrame, maxSkippedDrawSteps) {

}

void ZRDriver::Run() {
  GameDriver::Run();
  //Game ended
  ZeldaInformationHandler::SetIsRunning(false);
}

