#pragma once

template <typename T>
class with_initial
{
public:
    with_initial(T initial_value)
        : initial(initial_value), current(initial_value) {}

    T get_initial() const { return initial; }
    T get_current() const { return current; }

    with_initial& operator=(const T& value) {
        current = value;
        return *this;
    }

    with_initial& operator-=(const T& value) {
        current -= value;
        return *this;
    }

    with_initial& operator+=(const T& value) {
        current += value;
        return *this;
    }

    with_initial& operator--() {
        --current;
        return *this;
    }

    with_initial operator--(int) {
        with_initial temp = current;
        --current;
        return temp;
    }

    void set_current(T value) { current = value; }
    bool is_initial() const { return current == initial; }
    void reset() { current = initial; }

private:
    T initial;
    T current;
};