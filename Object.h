#pragma once
#include < iostream >
#include "Container.h"
#include < string >
#include< sstream > 

class Object {
public:
    virtual std::string to_string(  ) const = 0;
    static size_t copies() { return sm_counter; }
protected:
    Object() { ++sm_counter; }
    Object(Object const& other) { ++sm_counter; }
    virtual ~Object() { --sm_counter; }
private:
    inline static size_t sm_counter = 0;
};

class Named :virtual public Object {
public:
    Named( std::string & s  ):m_name( s ) {};
protected:
    std::string m_name;
};

class Task : virtual public Object { 
public:
    virtual void calculate(  ) = 0;
};

class BinaryTask : public Named, public Task {
public:
    BinaryTask( std::string & name, double x, double y ) : Named( name ), m_x( x ), m_y( y ) {};
    std::string to_string(  ) const override {
        if ( !m_calculated ) {
            return "operation " + Named::m_name + " is not performed";
        }
        return "operation " + Named::m_name + " has the m_result equal to " + std::to_string(m_res);
    }
    void calculate(  ) {
        if ( !m_calculated ) {
            m_calculated = true;
            if ( Named::m_name == "plus" ) {
                m_res = m_x + m_y;
            }
            if ( Named::m_name == "minus" ) {
                m_res = m_x - m_y;
            }
            if ( Named::m_name == "multiply" ) {
                m_res = m_x * m_y;
            }
            if ( Named::m_name == "div" ) {
                try
                {
                    if ( m_y == 0 ) throw std::exception( "Деление на 0" );
                    m_res = m_x / m_y;
                }
                catch ( std::exception& e )
                {
                    std::cout <<  e.what(  );
                }
            }
        }
    }
private:
    double m_x, m_y;
    double m_res = 0;
    bool m_calculated = false;
};

class AddTask : public Task {
public:
    AddTask( Container< Object&>& c, Object& o ) :m_c( c ), m_o( o ) {};
    std::string to_string(  ) const override {
        if (!m_calculated) {
            return "task of adding item to pending";
        }
        return "element addition task completed";
    }

    void calculate( ) {
        if( !m_calculated ) {
            m_calculated = true;
            m_c.push_back( m_o );
        }
    }
private:
    bool m_calculated = false;
    Container< Object&>& m_c;
    Object& m_o;
};

class CounterTask : public Task {
public:
    CounterTask( Container< Object&>& c ) : m_c( c ){};
    std::string to_string(  ) const override {
        if (!m_calculated) {
            return "item count task pending";
        }
        return "the number of elements in the array is" + std::to_string(m_res);
    }

    void calculate( ) {
        if (!m_calculated) {
            m_calculated = true;
            m_res = m_c.get_count();
        }
    }
private:
    bool m_calculated = false;
    Container< Object&>& m_c;
    int m_res = 0;
};

class CounterNammedTask : public Task {
    CounterNammedTask(Container< Object&>& c) : m_c(c) {};
    std::string to_string() const override {
        if (!m_calculated) {
            return "item count task pending";
        }
        return "the number of elements in the array is" + std::to_string(m_res);
    }

    void calculate() {
        if (!m_calculated) {
            m_calculated = true;
            m_res = m_c.get_count(); // I do not plan to add tasks without a name
        }
    }
private:
    bool m_calculated = false;
    Container< Object&>& m_c;
    int m_res = 0;
};

class CleanerTask : public Task {
    CleanerTask(Container< Object&>& c) : m_c(c) {};
    std::string to_string(  ) const override {
        if (!m_calculated) {
            return "pending container cleanup task";
        }
        return "container is clean";
    }

    void calculate( ) {
        if (!m_calculated) {
            m_calculated = true;
            m_c.clear();
        }
    }
private:
    bool m_calculated = false;
    Container< Object&>& m_c;
};

class GlobalCounterTask : public Task {
public:
    std::string to_string(  ) const override {
        return "at the moment there are " + std::to_string(res) + " Object";
    }

    void calculate(  ) {
    }
private:
    int res = Object::copies();
};