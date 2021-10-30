#include <ctime>
#include <string>
#include <queue>

enum class Period : size_t
{
    Year    = 60 * 60 * 24 * 365,
    Month   = 60 * 60 * 24 * 30,
    Day     = 60 * 60 * 24,
    Hour    = 60 * 60,
    Mintue  = 60,
    Second  = 1,
};

using TimeDiff = std::time_t;

class TimePoint 
{
protected: 
    std::time_t inner_;
    

public: 
    TimePoint(): inner_(std::time(nullptr)) {}
    TimePoint(std::time_t t) : inner_(t) {}
    TimePoint(const TimePoint&) = default;

    std::time_t inner() const;

    size_t year() const;
    size_t month() const;
    size_t day() const;
    size_t hour() const;
    size_t mintue() const;
    size_t second() const;

    void fly(std::time_t offset);
    void fly(Period period, size_t num);

    TimeDiff distance(const TimePoint& point) const;
    std::string as_str() const;

    TimePoint& operator + (const TimeDiff& dif);
    // TimePoint& operator - (const TimeDiff& dif);

    bool operator < (const TimePoint& other) const;
    bool operator == (const TimePoint& other) const;

    static TimePoint* now();
    static TimePoint* when(int year, int month, int day, int hour, int min, int sec);
};

class Timer : public TimePoint 
{
private:
    bool repeat_;
    std::time_t freq_;

public:
    Timer() : TimePoint(), repeat_(false), freq_((time_t)Period::Year) {}
    Timer(const Timer&) = default;
    Timer(std::time_t t, bool rep = false, time_t freq = (time_t)Period::Year) : TimePoint(t), repeat_(rep), freq_((time_t)freq) {}
    
    bool is_ready(const TimePoint& tp) const;
    bool is_out(const TimePoint& tp) const;
    bool is_repeat() const;
    time_t next_time() const;
    bool operator > (const Timer& other) const;
};

using TimerQueue = std::priority_queue<Timer, std::vector<Timer>, std::greater<Timer>>;
//

class ClockDial : public TimePoint
{
private:
    TimerQueue timers_;

    void check();
    void update();

public:
    ClockDial(): TimePoint(), timers_() {};
    ClockDial(std::time_t t): TimePoint(t), timers_() {};
    ClockDial(const ClockDial&);

    void insert_timer(const Timer& timer);
    void display() const;
    void tick();
    
    static void draw(const Timer& timer);
    // void remove_timer(size_t which);
};
