//
// Created by Edge on 2021/7/2.
//

#include "Timer.h"

void Timer::updateCurrentTime() {
    m_currentTime = std::chrono::system_clock::now();
}

float Timer::calculateDiffMilliSecondTime() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now() - m_currentTime).count();
}