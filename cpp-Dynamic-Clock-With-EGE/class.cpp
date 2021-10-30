#include "class.h"
#include "graphics.h"
#include <math.h>

size_t TimePoint::second() const
{
    return this->inner_ % (size_t)Period::Mintue;
}

size_t TimePoint::mintue() const
{
    return ((this->inner_ % (size_t)Period::Hour) - this->second()) / (size_t)Period::Mintue;
}

size_t TimePoint::hour() const
{
    return ((this->inner_ % (size_t)Period::Day) - (this->mintue() * (size_t)Period::Mintue) - this->second()) / (size_t)Period::Hour + 8;
    // convert GMT to local time by hour + 8;
	// CN in east eight area  
}

/* wrong code 
size_t TimePoint::day() const
{
    return this->inner_ % (size_t)Period::Day;
}

size_t TimePoint::month() const
{
    return this->inner_ % (size_t)Period::Month;
}

size_t TimePoint::year() const
{
    return this->inner_ % (size_t)Period::Year + 1970;
}
*/

std::time_t TimePoint::inner() const
{
    return this->inner_;
}


void TimePoint::fly(std::time_t offset)
{
    this->inner_ += offset;
}

void TimePoint::fly(Period period, size_t num)
{
    this->inner_ += (size_t)period * num;
}

TimeDiff TimePoint::distance(const TimePoint &point) const
{
    return this->inner_ > point.inner_
               ? this->inner_ - point.inner_
               : point.inner_ - this->inner_;
}

std::string TimePoint::as_str() const
{
    return std::asctime(std::localtime(&this->inner_));
}

TimePoint &TimePoint::operator+(const TimeDiff &dif)
{
    this->fly(dif);
    return *this;
}

bool TimePoint::operator<(const TimePoint &other) const
{
    return this->inner_ < other.inner_;
}

bool TimePoint::operator==(const TimePoint &other) const
{
    return this->inner_ == other.inner_;
}

TimePoint *TimePoint::now()
{
    return new TimePoint(std::time(nullptr));
}

TimePoint *TimePoint::when(int year, int month, int day, int hour, int min, int sec)
{
    return new TimePoint((time_t)(year * (size_t)Period::Year + month * (size_t)Period::Month + day * (size_t)Period::Day + hour * (size_t)Period::Hour + min * (size_t)Period::Mintue + sec * (size_t)Period::Second));
}

bool Timer::is_ready(const TimePoint &tp) const
{
    return this->inner_ == tp.inner();
}

bool Timer::is_out(const TimePoint &tp) const
{
    return this->inner_ < tp.inner();
}

bool Timer::is_repeat() const
{
    return this->repeat_;
}
time_t Timer::next_time() const
{
	return this->freq_;
}

bool Timer::operator > (const Timer& other) const
{
	return this->inner_ > other.inner();
}

ClockDial::ClockDial(const ClockDial &other) : TimePoint(other)
{
    this->timers_ = other.timers_;
}

void ClockDial::check()
{
	while (!this->timers_.empty() && this->timers_.top().is_out(this->inner_))
		    this->timers_.pop();
}

void ClockDial::insert_timer(const Timer& timer) 
{
	this->timers_.push(timer);
	
	// ege::setcolor(GREEN);
	// xyprintf(200, 850, info.append(timer.as_str().append(" insert timer ")).c_str());
}

void ClockDial::update() 
{
	if(!this->timers_.empty())
	{
		const Timer& timer = this->timers_.top();
		if (timer.is_ready(this->inner()) || timer.is_out(this->inner()))
		{
			if(timer.is_repeat())
			{
				this->insert_timer(Timer(this->inner()  + (size_t)timer.next_time(), true, timer.next_time()));
			}
		}
	}
}

void ClockDial::tick()
{
	this->update(); // update timer queue
    this->fly(1); 	// current time +1s
    this->check(); 	// remove timer which out of time 
}

void ClockDial::display() const
{
    // if contains canvas to draw
    if (is_run())
    {
        // draw timer if necessary
        if (!this->timers_.empty())
        {
            const Timer &timer = this->timers_.top();
            if (timer.is_ready(this->inner_))
                this->draw(timer);
        }

        // define some config args
        static const int DialSize = 300;
        static const ege::ege_point center{500, 500};

        // define some helper function
        auto getpos = [=](float rad, float r) -> ege::ege_point
        {
            ege::ege_point pos;
            pos.x = sin(rad) * r + center.x;
            pos.y = -cos(rad) * r + center.y;
            return pos;
        };

        // draw background dial
        ege::setlinewidth(2.0f);
        ege::setcolor(WHITE);
        circle(center.x, center.y, DialSize);
        {
            float r = 0;                 // radius
            for (int i = 0; i < 12; i++) // hour
            {
                ege::ege_point start = getpos(r, DialSize);
                ege::ege_point stop = getpos(r, DialSize - 50);
                line(start.x, start.y, stop.x, stop.y);
                r += 2 * PI / 12;
            }

            for (int i = 0; i < 60; i++) // mintue
            {
                ege::ege_point start = getpos(r, DialSize);
                ege::ege_point stop = getpos(r, DialSize - 15);
                line(start.x, start.y, stop.x, stop.y);
                r += 2 * PI / 60;
            }
        }

        // draw three pointers
        {
            // hour
            ege::setlinewidth(10.0f);
            ege::setcolor(BLUE);

            double rad = 2 * PI / 12 * this->hour();
            ege::ege_point stop = getpos(rad, DialSize - 150);
            line(center.x, center.y, stop.x, stop.y);
        }
        
        {
		    // mintue
		    ege::setlinewidth(5.0f);
	        ege::setcolor(GREEN);
		
	        double rad = 2 * PI / 60 * this->mintue();
	        ege::ege_point stop = getpos(rad, DialSize - 100);
		    line(center.x, center.y, stop.x, stop.y);
		}
		
		{
			// second
			ege::setlinewidth(1.0f);
			ege::setcolor(YELLOW);
				
			double rad = 2 * PI / 60 * this->second();
			ege::ege_point stop = getpos(rad, DialSize - 50);
			line(center.x, center.y, stop.x, stop.y);
		}

		// debug variable 'test'  
        // std::string test;
        // test.reserve(50);
        // test.append(std::to_string(this->hour())).append(" ").append(std::to_string(this->mintue())).append(" ").append(std::to_string(this->second()));
		
        // display current time string
        ege::setcolor(WHITE);
        xyprintf(center.x - 100, center.y + 70, this->as_str().c_str());
        // xyprintf(100, 600, test.c_str());
    }
}

void ClockDial::draw(const Timer &timer)
{
	static auto paintstar = [](double x, double y, double r, double a)
	{
	    int pt[10];
	    for (int n = 0; n < 5; ++n)
	    {
	        pt[n*2] = (int)( -cos( PI * 4 / 5 * n + a ) * r + x );
	        pt[n*2+1] = (int)( sin( PI * 4 / 5 * n + a) * r + y );
	    }
	    fillpoly(5, pt);
	};
	
    if (is_run())
    {
    	std::string info = "Timer: ";
    	xyprintf(100, 850, info.append(timer.as_str().append(" has alarmed!!!!!!")).c_str());
    	paintstar(600, 850, 100, 0);
    }
}
