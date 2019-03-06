// This is minimum implementation of fixed length circular buffer.

#ifndef __CIRCULAR_BUFFER__
#define __CIRCULAR_BUFFER__

#include <deque>

template<typename T>
class circular_buffer
{
private:
    std::deque<T> deque;
    size_t size;

public:
    circular_buffer( size_t size = 10 )
        : size( size )
    {
    }

    void push_front( const T& value )
    {
        if( deque.size() > size ){
            deque.pop_back();
        }
        deque.push_front( value );
    }

    void clear()
    {
        deque.clear();
    }

    typedef typename std::deque<T>::iterator iterator;
    typedef typename std::deque<T>::const_iterator const_iterator;

    iterator begin(){ return deque.begin(); }
    const_iterator begin() const { return deque.begin(); }
    iterator end(){ return deque.end(); }
    const_iterator end() const { return deque.end(); }
};

#endif // __CIRCULAR_BUFFER__