/* © Copyright Artem Shapovalov 2025
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

#ifndef INDEPENDENCY_HPP
#define INDEPENDENCY_HPP

#include <cstddef>
#include <cstring>

namespace boost { namespace independency {

class Message;

/// \brief The main data structure for the messages.
/// \details Designed to be one of the temporary items of chain.
class Pair {
    public:

    /// \brief Constructor for void-pointer key-value pair.
    /// \param k Key.
    /// \param v Value.
    Pair(unsigned long k, void* v) : next(static_cast<Pair*>(0))
    {
        key = k;
        val._void_pointer = v;
        type = _void_pointer;
    }

    /// \brief Constructor for string literal key-value pair.
    /// \param k Key.
    /// \param v Value.
    Pair(unsigned long k, const char* v) : next(static_cast<Pair*>(0))
    {
        key = k;
        val._string = v;
        type = _string;
    }

    /// \brief Constructor for character key-value pair.
    /// \param k Key.
    /// \param v Value.
    Pair(unsigned long k, char v) : next(static_cast<Pair*>(0))
    {
        key = k;
        val._char = v;
        type = _char;
    }

    /// \brief Constructor for unsigned character key-value pair.
    /// \param k Key.
    /// \param v Value.
    Pair(unsigned long k, unsigned char v) : next(static_cast<Pair*>(0))
    {
        key = k;
        val._unsigned_char = v;
        type = _unsigned_char;
    }

    /// \brief Constructor for short integer key-value pair.
    /// \param k Key.
    /// \param v Value.
    Pair(unsigned long k, short v) : next(static_cast<Pair*>(0))
    {
        key = k;
        val._short = v;
        type = _short;
    }

    /// \brief Constructor for unsigned short integer key-value pair.
    /// \param k Key.
    /// \param v Value.
    Pair(unsigned long k, unsigned short v) : next(static_cast<Pair*>(0))
    {
        key = k;
        val._unsigned_short = v;
        type = _unsigned_short;
    }

    /// \brief Constructor for integer key-value pair.
    /// \param k Key.
    /// \param v Value.
    Pair(unsigned long k, int v) : next(static_cast<Pair*>(0))
    {
        key = k;
        val._int = v;
        type = _int;
    }

    /// \brief Constructor for unsigned integer key-value pair.
    /// \param k Key.
    /// \param v Value.
    Pair(unsigned long k, unsigned int v) : next(static_cast<Pair*>(0))
    {
        key = k;
        val._unsigned_int = v;
        type = _unsigned_int;
    }

    /// \brief Constructor for long integer key-value pair.
    /// \param k Key.
    /// \param v Value.
    Pair(unsigned long k, long v) : next(static_cast<Pair*>(0))
    {
        key = k;
        val._long = v;
        type = _long;
    }

    /// \brief Constructor for unsigned long integer key-value pair.
    /// \param k Key.
    /// \param v Value.
    Pair(unsigned long k, unsigned long v) : next(static_cast<Pair*>(0))
    {
        key = k;
        val._unsigned_long = v;
        type = _unsigned_long;
    }

    /// \brief Constructor for float-point number key-value pair.
    /// \param k Key.
    /// \param v Value.
    Pair(unsigned long k, float v) : next(static_cast<Pair*>(0))
    {
        key = k;
        val._float = v;
        type = _float;
    }

    /// \brief Constructor for double precision float-point number key-value pair.
    /// \param k Key.
    /// \param v Value.
    Pair(unsigned long k, double v) : next(static_cast<Pair*>(0))
    {
        key = k;
        val._double = v;
        type = _double;
    }

    private:
    friend class Message;

    unsigned long key;

    union {
        void* _void_pointer;
        const char* _string;
        char _char;
        unsigned char _unsigned_char;
        short _short;
        unsigned short _unsigned_short;
        int _int;
        unsigned int _unsigned_int;
        long _long;
        unsigned long _unsigned_long;
        float _float;
        double _double;
    } val;

    enum {
        _void_pointer,
        _string,
        _char,
        _unsigned_char,
        _short,
        _unsigned_short,
        _int,
        _unsigned_int,
        _long,
        _unsigned_long,
        _float,
        _double
    } type;

    Pair* next;
};

/// \brief The transmission unit to propagate through the bus.
class Message
{
    public:
    /// \brief Constructor.
    /// \param p The temporary instance of key-value pair.
    explicit Message(const Pair& p)
    {
        list = const_cast<Pair*>(&p);
        last = const_cast<Pair*>(&p);
    }

    /// \brief Appends key-value pair to the message.
    /// \param p The temporary instance of key-value pair.
    /// \return Reference to the caller object to make the chain of adds.
    Message& add(const Pair& p)
    {
        last->next = const_cast<Pair*>(&p);
        last = last->next;
        return *this;
    }

    /// \brief Extracts void-pointer from the message by key.
    /// \warning Returns 0 if any error occured.
    /// \param key Key.
    /// \return Value.
    void* get_void_pointer(unsigned long key) const
    {
        Pair* p = find(key);
        if (p != static_cast<Pair*>(0) && p->type == Pair::_void_pointer)
        {
            return p->val._void_pointer;
        }
        return static_cast<void*>(0);
    }

    /// \brief Extracts string literal from the message by key.
    /// \warning Returns 0 if any error occured.
    /// \param key Key.
    /// \return Value.
    const char* get_string(unsigned long key) const
    {
        Pair* p = find(key);
        if (p != static_cast<Pair*>(0) && p->type == Pair::_string)
        {
            return p->val._string;
        }
        return static_cast<const char*>(0);
    }

    /// \brief Extracts character from the message by key.
    /// \warning Returns 0 if any error occured.
    /// \param key Key.
    /// \return Value.
    char get_char(unsigned long key) const
    {
        Pair* p = find(key);
        if (p != static_cast<Pair*>(0) && p->type == Pair::_char)
        {
            return p->val._char;
        } 
        return static_cast<char>(0);
    }

    /// \brief Extracts unsigned character from the message by key.
    /// \warning Returns 0 if any error occured.
    /// \param key Key.
    /// \return Value.
    unsigned char get_unsigned_char(unsigned long key) const
    {
        Pair* p = find(key);
        if (p != static_cast<Pair*>(0) && p->type == Pair::_unsigned_char)
        {
            return p->val._unsigned_char;
        }
        return static_cast<unsigned char>(0);
    }

    /// \brief Extracts short integer from the message by key.
    /// \warning Returns 0 if any error occured.
    /// \param key Key.
    /// \return Value.
    short get_short(unsigned long key) const
    {
        Pair* p = find(key);
        if (p != static_cast<Pair*>(0) && p->type == Pair::_short)
        {
            return p->val._short;
        }
        return static_cast<short>(0);
    }

    /// \brief Extracts unsigned short integer from the message by key.
    /// \warning Returns 0 if any error occured.
    /// \param key Key.
    /// \return Value.
    unsigned short get_unsigned_short(unsigned long key) const
    {
        Pair* p = find(key);
        if (p != static_cast<Pair*>(0) && p->type == Pair::_unsigned_short)
        {
            return p->val._unsigned_short;
        }
        return static_cast<unsigned short>(0);
    }

    /// \brief Extracts integer from the message by key.
    /// \warning Returns 0 if any error occured.
    /// \param key Key.
    /// \return Value.
    int get_int(unsigned long key) const
    {
        Pair* p = find(key);
        if (p != static_cast<Pair*>(0) && p->type == Pair::_int)
        {
            return p->val._int;
        }
        return static_cast<int>(0);
    }

    /// \brief Extracts unsigned integer from the message by key.
    /// \warning Returns 0 if any error occured.
    /// \param key Key.
    /// \return Value.
    unsigned int get_unsigned_int(unsigned long key) const
    {
        Pair* p = find(key);
        if (p != static_cast<Pair*>(0) && p->type == Pair::_unsigned_int)
        {
            return p->val._unsigned_int;
        }
        return static_cast<unsigned int>(0);
    }

    /// \brief Extracts long integer from the message by key.
    /// \warning Returns 0 if any error occured.
    /// \param key Key.
    /// \return Value.
    long get_long(unsigned long key) const
    {
        Pair* p = find(key);
        if (p != static_cast<Pair*>(0) && p->type == Pair::_long)
        {
            return p->val._long;
        }
        return static_cast<long>(0);
    }

    /// \brief Extracts unsigned long integer from the message by key.
    /// \warning Returns 0 if any error occured.
    /// \param key Key.
    /// \return Value.
    unsigned long get_unsigned_long(unsigned long key) const
    {
        Pair* p = find(key);
        if (p != static_cast<Pair*>(0) && p->type == Pair::_unsigned_long)
        {
            return p->val._unsigned_long;
        }
        return static_cast<unsigned long>(0);
    }

    /// \brief Extracts float-point number from the message by key.
    /// \warning Returns 0 if any error occured.
    /// \param key Key.
    /// \return Value.
    float get_float(unsigned long key) const
    {
        Pair* p = find(key);
        if (p != static_cast<Pair*>(0) && p->type == Pair::_float)
        {
            return p->val._float;
        }
        return static_cast<float>(0);
    }

    /// \brief Extracts double precision float-point number from the message by key.
    /// \warning Returns 0 if any error occured.
    /// \param key Key.
    /// \return Value.
    double get_double(unsigned long key) const
    {
        Pair* p = find(key);
        if (p != static_cast<Pair*>(0) && p->type == Pair::_double)
        {
            return p->val._double;
        }
        return static_cast<double>(0);
    }

    private:
    Pair* find(unsigned long key) const
    {
        Pair* iter = list;
        while (iter != static_cast<Pair*>(0))
        {
            if (key == iter->key) { return iter; }
            iter = iter->next;
        }
        return static_cast<Pair*>(0);
    }

    Pair* list;
    Pair* last;
};

/// \brief   The basic class for handling messages.
/// \details Use it as class member and initialize with static member-function.
class Handler
{
    public:
    /// \brief Constructor for parametrized callback
    /// \param arg  Will be passed to the callback
    /// \param func Callback, would be called for every message on the bus
    Handler(void* arg, void (*func)(void* arg, const Message& msg))
    : next(static_cast<Handler*>(0)),
      arg(arg),
      func(func),
      func2(static_cast<void (*)(const Message&)>(0))
    { }

    /// \brief Constructor for unparametrized callback
    /// \param func Callback, would be called for every message in the bug.
    explicit Handler(void (*func)(const Message& msg))
    : next(static_cast<Handler*>(0)),
      arg(static_cast<void*>(0)),
      func(static_cast<void (*)(void*, const Message&)>(0)),
      func2(func)
    { }

    private:
    friend class Bus;
    Handler* next;
    void* arg;
    void (*func)(void* arg, const Message& msg);
    void (*func2)(const Message& msg);
};

/// \brief Message propagation mechanism.
/// \details Instantiate it once for many modules.
class Bus
{
    public:
    Bus() : hnd(static_cast<Handler*>(0)) { }


    /// \brief Propagates the message through the bus.
    /// \param msg Temporary message object.
    void send(const Message& msg)
    {
        if (hnd == (Handler*)0) { return; }

        Handler* iter = hnd;
        while (iter != static_cast<Handler*>(0))
        {
            if (iter->func !=
                static_cast<void (*)(void*, const Message&)>(0))
            {
                iter->func(iter->arg, msg);
            }
            else if (iter->func2 != static_cast<void (*)(const Message&)>(0))
            {
                iter->func2(msg);
            }
            iter = iter->next;
        }
    }

    /// \brief Subscribes the handler for messages.
    /// \param handler Reference to the subscriber's handler.
    void reg(const Handler& handler)
    {
        Handler* _hnd = const_cast<Handler*>(&handler);
        if (this->hnd == static_cast<Handler*>(0)) { this->hnd = _hnd; return; }
        
        Handler* last = this->hnd;
        while (last->next != static_cast<Handler*>(0)) { last = last->next; }
        last->next = _hnd;
    }

    private:
    Handler* hnd;
};

}} // namespace boost::independency

#endif // INDEPENDENCY_HPP
