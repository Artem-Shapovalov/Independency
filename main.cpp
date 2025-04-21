#include <cstdio>
#include "independency.hpp"

Bus bus;

#define COUNTER_VAL 0
#define EVENT_TYPE 1

class data_producer
{
    public:
    data_producer() : counter(0) {}
    void tick()
    {
        counter++;
        std::printf("--> %d\n", counter);
        bus.send(Message(Pair(COUNTER_VAL, counter)));
    }

    private:
    int counter;

};

class data_consumer
{
    public:
    explicit data_consumer(int div)
    : divider(div), ctr(0), h(static_cast<void*>(this), func)
    {
        bus.reg(h);
    }

    private:
    int divider;
    int ctr;

    static void func(void* arg, const Message& m)
    {
        data_consumer* that = static_cast<data_consumer*>(arg);
        if (m.get_int(COUNTER_VAL) % that->divider == 0)
        {
            that->ctr++;
            std::printf("div %d:%d\n", that->divider, that->ctr);
        }
    }
    Handler h;
};

int main(int argc, char** argv)
{
    std::printf("Independency V1 demo\n");

    data_producer prod;
    data_consumer cons1(3);
    data_consumer cons2(5);
    data_consumer cons3(7);

    for (int i = 0; i < 10; i++)
    {
        prod.tick();
    }
    
    return 0;
}
