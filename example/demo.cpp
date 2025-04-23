/* @Copyright Artem Shapovalov 2025-2025
 * Distrubutes under the:
 *
 * Boost Software Licence - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished
 * to do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part,
 * and all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated
 * by a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE. */

#include <boost/independency.hpp>
#include <cstdio>

using namespace boost::independency;

// The legend: imagine you build the dashboard for some vehicle. It may contain
// lots of indicators that should run separately. The classical approach is
// to make it just classes that directly call together, but the program
// rapidly thransforms to the dirty and unmaintainable dependency hell.
// To avoid it you may write your program in service-orientied approach, but
// with inner local bus. You describe the communication format and then
// implement each module separately. Each module depends on the two things
// only: the Independency library and message format. Also this approach
// opens for you a lot of opportunities for integration testing.

// Please, note that it's just an inner bus, there is no any serialization
// and synchronization options, so you can use it inside the same running
// process and all possible thread issues you should resolve yourself.

// These macro definitions provides the structure of the messages.

#define EVENT_KEY              1
#define EVENT_SYSTEM           1
#define EVENT_DATA_RECEIVED    2

#define INIT_KEY               2
#define SYSTEM_INIT            1
#define SPEED_INDICATOR_INITED 2
#define RPM_INDICATOR_INITED   3
#define FUEL_INDICATOR_INITED  4

#define SPEED_KEY              2
#define RPM_KEY                3
#define FUEL_LEVEL_KEY         4

// A single bus instance common for all of the modules.
Bus bus;

// These classes represents the program modules that have no mutual
// dependencies, all of them depends on Independency library and message
// macro definitions only. In other words dependency injection in action.

class speed_indicator: public Handler
{
    public:
    speed_indicator() : Handler(reinterpret_cast<speed_indicator*>(this), hnd)
    {}

    private:
    static void hnd(void* arg, const Message& msg)
    {
        speed_indicator* that = reinterpret_cast<speed_indicator*>(arg);

        if (msg.get_int(EVENT_KEY) == EVENT_SYSTEM &&
            msg.get_int(INIT_KEY)  == SYSTEM_INIT)
        {
            that->init();
        }

        if (msg.get_int(EVENT_KEY) == EVENT_DATA_RECEIVED)
        {
            that->print_speed(msg.get_float(SPEED_KEY));
        }
    }

    void init()
    {
        std::printf("speedometer: ready\n");
        bus.send(Message(Pair(EVENT_KEY, EVENT_SYSTEM))
                    .add(Pair(INIT_KEY,  SPEED_INDICATOR_INITED)));
    }

    void print_speed(float speed)
    {
        std::printf("speedometer: %f km/h\n", speed);
    }
};

class rpm_indicator : public Handler
{
    public:
    rpm_indicator() : Handler(reinterpret_cast<rpm_indicator*>(this), hnd)
    {}

    private:
    static void hnd(void* arg, const Message& msg)
    {
        rpm_indicator* that = reinterpret_cast<rpm_indicator*>(arg);

        if (msg.get_int(EVENT_KEY) == EVENT_SYSTEM &&
            msg.get_int(INIT_KEY)  == SPEED_INDICATOR_INITED)
        {
            that->init();
        }

        if (msg.get_int(EVENT_KEY) == EVENT_DATA_RECEIVED)
        {
            that->print_rpm(msg.get_float(RPM_KEY));
        }
    }

    void init()
    {
        std::printf("RPM: ready\n");
        bus.send(Message(Pair(EVENT_KEY, EVENT_SYSTEM))
                    .add(Pair(INIT_KEY, RPM_INDICATOR_INITED)));
    }

    void print_rpm(float rpm)
    {
        std::printf("RPM: %f\n", rpm);
    }
};

class fuel_indicator : public Handler
{
    public:
    fuel_indicator() : Handler(reinterpret_cast<fuel_indicator*>(this), hnd)
    {}

    private:
    static void hnd(void* arg, const Message& msg)
    {
        fuel_indicator* that = reinterpret_cast<fuel_indicator*>(arg);

        if (msg.get_int(EVENT_KEY) == EVENT_SYSTEM &&
            msg.get_int(INIT_KEY)  == SYSTEM_INIT)
        {
            that->init();
        }

        if (msg.get_int(EVENT_KEY) == EVENT_DATA_RECEIVED)
        {
            that->print_fuel(msg.get_float(FUEL_LEVEL_KEY));
        }
    }

    void init()
    {
        std::printf("fuel level: ready\n");
        bus.send(Message(Pair(EVENT_KEY, EVENT_SYSTEM))
                    .add(Pair(INIT_KEY, FUEL_INDICATOR_INITED)));
    }
    void print_fuel(float fuel)
    {
        std::printf("fuel level: %f\n", fuel);
    }
};

class bus_monitor : public Handler
{
    public:
    bus_monitor() : Handler(hnd) {}

    private:
    static void hnd(const Message& msg)
    {
        std::printf("monitor: ");

        if (msg.get_int(EVENT_KEY) == EVENT_SYSTEM)
        {
            switch (msg.get_int(INIT_KEY))
            {
                case SYSTEM_INIT:           std::printf("system init\n"); break;
                case SPEED_INDICATOR_INITED:std::printf("speed ready\n"); break;
                case RPM_INDICATOR_INITED:  std::printf("RPM ready\n");   break;
                case FUEL_INDICATOR_INITED: std::printf("fuel ready\n");  break;
                default: break;
            }
        }

        if (msg.get_int(EVENT_KEY) == EVENT_DATA_RECEIVED)
        {
            std::printf("speed: %f, rpm: %f, fuel: %f\n",
                msg.get_float(SPEED_KEY),
                msg.get_float(RPM_KEY),
                msg.get_float(FUEL_LEVEL_KEY)
            );
        }
    }
};

int main(int argc, char** argv)
{
    // Here we declare the actors of this demo program.
    bus_monitor bm;
    speed_indicator speedometer;
    rpm_indicator rpm;
    fuel_indicator fuel;

    // Here we connecte all of them to the system bus.
    bus.reg(bm);
    bus.reg(speedometer);
    bus.reg(rpm);
    bus.reg(fuel);

    // This event demonstrates the example of simple ordered initialization.
    // It's a subset of dependency-based initialization, that pretty clear
    // to implement and understand with the event-based bus approach.
    bus.send(Message(Pair(EVENT_KEY, EVENT_SYSTEM))
                .add(Pair(INIT_KEY,  SYSTEM_INIT)));

    // This is mock of working mode, after this message all of the caught data
    // would be printed in console, one data event for all of the modules and
    // each module do it's own part of job.
    bus.send(Message(Pair(EVENT_KEY, EVENT_DATA_RECEIVED))
                .add(Pair(SPEED_KEY,      40.0f))
                .add(Pair(RPM_KEY,        1000.0f))
                .add(Pair(FUEL_LEVEL_KEY, 0.5f)));

    return 0;
}