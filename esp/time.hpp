#pragma once

#include <chrono>
#include <esp_err.h>
#include <string>

namespace Time {
esp_err_t initSNTP();
const std::chrono::system_clock::time_point timePointNow();
int64_t epochMillis();
std::string timeNowAscii();
const std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>
iSO8601StringToTimePoint(const std::string &iso8601);
}; // namespace Time
