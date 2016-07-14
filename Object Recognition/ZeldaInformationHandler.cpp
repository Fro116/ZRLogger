#include "ZeldaInformationHandler.h"

ZeldaInformationHandler::ZeldaInformationHandler() {

}

std::mutex ZeldaInformationHandler::mapLocationMutex;
int ZeldaInformationHandler::mapx = -1;
int ZeldaInformationHandler::mapy = -1;

void ZeldaInformationHandler::SetMapLocation(int x, int y) {
  std::lock_guard<std::mutex> guard(mapLocationMutex);
  mapx = x;
  mapy = y;
}

std::pair<int, int> ZeldaInformationHandler::GetMapLocation() {
  std::lock_guard<std::mutex> guard(mapLocationMutex);
  return std::make_pair(mapx, mapy);
}
