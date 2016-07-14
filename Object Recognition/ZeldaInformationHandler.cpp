#include "ZeldaInformationHandler.h"

ZeldaInformationHandler::ZeldaInformationHandler() {

}

void ZeldaInformationHandler::SetMapLocation(int x, int y) {
  std::lock_guard<std::mutex> guard(mapLocationMutex);
  mapx = x;
  mapy = y;
}

std::pair<int, int> ZeldaInformationHandler::GetMapLocation() {
  std::lock_guard<std::mutex> guard(mapLocationMutex);
  return std::make_pair(mapx, mapy);
}
