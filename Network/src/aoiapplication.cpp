#include "aoiapplication.h"
#include "math.h"

AoIApplication::AoIApplication(MobileTerminal * const tx, MobileTerminal * const rx, const int ts_ms, const int sampling_offset)
    : Application(tx, rx)
{
    this->ts_ms = ts_ms;
    this->t_until_next_sample_ms = sampling_offset + ts_ms;
    age = 0;
    k = 0;
    l = -1;
}

void AoIApplication::update()
{
    if(t_until_next_sample_ms-- == 0)
    {
        t_until_next_sample_ms = ts_ms;
        k++;
        age = k - l;
    }
}

int AoIApplication::getAge() const
{
    return age;
}

void AoIApplication::setAge(int newAge)
{
    age = newAge;
}

int AoIApplication::getLatestSeenTimestamp()
{
    return l;
}

int AoIApplication::getAgeInXSlots(int x) const
{
    if (x == 0)
        return age;

    int periods = static_cast<int>(floor(double(x) / double(ts_ms)));
    int remain = x % ts_ms;

    if(remain <= t_until_next_sample_ms)
        return age + periods + 1;
    else
        return age + periods;
}
