#include "ZeldaInformationHandler.h"

ZeldaInformationHandler::ZeldaInformationHandler() {

}

std::mutex ZeldaInformationHandler::mapLocationMutex;
int ZeldaInformationHandler::mapx = -1;
int ZeldaInformationHandler::mapy = -1;
bool ZeldaInformationHandler::inSecretCave;

std::mutex ZeldaInformationHandler::isRunningMutex;
bool ZeldaInformationHandler::isRunning = true;

void ZeldaInformationHandler::SetMapLocation(int x, int y, bool inCave) {
  std::lock_guard<std::mutex> guard(mapLocationMutex);
  mapx = x;
  mapy = y;
  inSecretCave = inCave;
}

std::tuple<int, int, bool> ZeldaInformationHandler::GetMapLocation() {
  std::lock_guard<std::mutex> guard(mapLocationMutex);
  return std::make_tuple(mapx, mapy, inSecretCave);
}

void ZeldaInformationHandler::SetIsRunning(bool running) {
  std::lock_guard<std::mutex> guard(isRunningMutex);
  isRunning = running;
}

bool ZeldaInformationHandler::GetIsRunning() {
  std::lock_guard<std::mutex> guard(isRunningMutex);
  return isRunning;
}
