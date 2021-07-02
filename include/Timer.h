//
// Created by Edge on 2021/7/2.
//

#ifndef RAY_TIMER_H
#define RAY_TIMER_H

#include <chrono>

class Timer {
public:
    void updateCurrentTime();
    float calculateDiffMilliSecondTime() const;
private:
    std::chrono::time_point<std::chrono::system_clock> m_currentTime;
};

#endif //RAY_TIMER_H
