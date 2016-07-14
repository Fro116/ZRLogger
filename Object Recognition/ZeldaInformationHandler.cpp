#include "ZeldaInformationHandler.h"

ZeldaInformationHandler::ZeldaInformationHandler() {

}

std::mutex ZeldaInformationHandler::mapLocationMutex;
int ZeldaInformationHandler::mapx = -1;
int ZeldaInformationHandler::mapy = -1;

std::mutex ZeldaInformationHandler::secretsMutex;
std::map<std::pair<int,int>, ZeldaInformationHandler::Secrets> ZeldaInformationHandler::overworldSecrets;

std::mutex ZeldaInformationHandler::isRunningMutex;
bool ZeldaInformationHandler::isRunning = true;

void ZeldaInformationHandler::SetMapLocation(int x, int y) {
  std::lock_guard<std::mutex> guard(mapLocationMutex);
  mapx = x;
  mapy = y;
}

ZeldaInformationHandler::Secrets ZeldaInformationHandler::GetSecret(int x, int y) {
  std::lock_guard<std::mutex> guard(secretsMutex);
  if (overworldSecrets.empty()) {
    for (int a = 0; a < 8; ++a) {
      for (int b = 0; b < 16; ++b) {
	overworldSecrets[std::make_pair(a,b)] = Secrets::UNEXPLORED;
      }
    }
  }  
  return overworldSecrets[std::make_pair(x,y)];
}

void ZeldaInformationHandler::SetSecret(int x, int y, Secrets secret) {
  std::lock_guard<std::mutex> guard(secretsMutex);
  if (overworldSecrets.empty()) {
    for (int a = 0; a < 8; ++a) {
      for (int b = 0; b < 16; ++b) {
	overworldSecrets[std::make_pair(a,b)] = Secrets::UNEXPLORED;
      }
    }
  }  
  overworldSecrets[std::make_pair(x,y)] = secret;
}

std::pair<int, int> ZeldaInformationHandler::GetMapLocation() {
  std::lock_guard<std::mutex> guard(mapLocationMutex);
  return std::make_pair(mapx, mapy);
}

void ZeldaInformationHandler::SetIsRunning(bool running) {
  std::lock_guard<std::mutex> guard(isRunningMutex);
  isRunning = running;
}

bool ZeldaInformationHandler::GetIsRunning() {
  std::lock_guard<std::mutex> guard(isRunningMutex);
  return isRunning;
}
