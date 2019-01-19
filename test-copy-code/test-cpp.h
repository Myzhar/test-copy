#include <chrono>

using namespace std;
using namespace chrono;

class Timer
{
public:
    Timer(){}

    inline void tic() ///< Start timer
    {
        mStart = std::chrono::high_resolution_clock::now();
    }

    inline double toc() ///< Returns msec elapsed from last tic
    {
        auto end = high_resolution_clock::now();
        auto dur = duration_cast<microseconds>(end - mStart);

        double elapsed = static_cast<double>(dur.count())/1000.;

        return elapsed;
    }

private:
    high_resolution_clock::time_point mStart;
};

