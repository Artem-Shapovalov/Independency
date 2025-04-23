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

class test_consumer : public Handler
{
    public:
    test_consumer() : Handler(reinterpret_cast<void*>(this), hnd), received(0)
    {}

    static void hnd(void* arg, const Message& mess)
    {
        test_consumer* that = reinterpret_cast<test_consumer*>(arg);
        that->received = mess.get_int(1);
    }

    int received;
};

int main(int argc, char** argv)
{
    {
        // These tests checks out the message creation and reading each type
        // of data. It's not a clear unit test, we check out several calls.

        Pair void_pointer_pair(1, reinterpret_cast<void*>(1));
        Pair string_pair(2, reinterpret_cast<const char*>(2));
        Pair char_pair(3, static_cast<char>(3));
        Pair unsigned_char_pair(4, static_cast<unsigned char>(4));
        Pair short_pair(5, static_cast<short>(5));
        Pair unsigned_short_pair(6, static_cast<unsigned short>(6));
        Pair int_pair(7, static_cast<int>(7));
        Pair unsigned_int_pair(8, static_cast<unsigned int>(8));
        Pair long_pair(9, static_cast<long>(9));
        Pair unsigned_long_pair(10, static_cast<unsigned long>(10));
        Pair float_pair(11, static_cast<float>(11));
        Pair double_pair(12, static_cast<double>(12));

        Message mess(void_pointer_pair);
        mess.add(string_pair);
        mess.add(char_pair);
        mess.add(unsigned_char_pair);
        mess.add(short_pair);
        mess.add(unsigned_short_pair);
        mess.add(int_pair);
        mess.add(unsigned_int_pair);
        mess.add(long_pair);
        mess.add(unsigned_long_pair);
        mess.add(float_pair);
        mess.add(double_pair);

        if (mess.get_void_pointer(1) != reinterpret_cast<void*>(1))
        {
            std::printf("get_void_pointer failed\n");
            return -1;
        }

        if (mess.get_string(2) != reinterpret_cast<const char*>(2))
        {
            std::printf("get_string failed\n");
            return -1;
        }

        if (mess.get_char(3) != static_cast<char>(3))
        {
            std::printf("get_char failed\n");
            return -1;
        }

        if (mess.get_unsigned_char(4) != static_cast<unsigned char>(4))
        {
            std::printf("get_unsigned_char failed\n");
            return -1;
        }

        if (mess.get_short(5) != static_cast<short>(5))
        {
            std::printf("get_short failed\n");
            return -1;
        }

        if (mess.get_unsigned_short(6) != static_cast<unsigned short>(6))
        {
            std::printf("get_unsigned_short failed\n");
            return -1;
        }

        if (mess.get_int(7) != static_cast<int>(7))
        {
            std::printf("get_int failed\n");
            return -1;
        }

        if (mess.get_unsigned_int(8) != static_cast<unsigned int>(8))
        {
            std::printf("get_unsigned_int failed\n");
            return -1;
        }

        if (mess.get_long(9) != static_cast<long>(9))
        {
            std::printf("get_long failed\n");
            return -1;
        }

        if (mess.get_unsigned_long(10) != static_cast<unsigned long>(10))
        {
            std::printf("get_unsigned_long failed\n");
            return -1;
        }

        if (mess.get_float(11) != static_cast<float>(11))
        {
            std::printf("get_float failed\n");
            return -1;
        }

        if (mess.get_double(12) != static_cast<double>(12))
        {
            std::printf("get_double failed\n");
            return -1;
        }
    }

    {
        // This test check the bus message propagation and building

        Bus bus;
        test_consumer cons;

        bus.reg(cons);
        bus.send(Message(Pair(1, static_cast<int>(10))));

        if (cons.received != 10)
        {
            std::printf("bus propagation test failed\n");
            return -1;
        }
    }

    return 0;
}