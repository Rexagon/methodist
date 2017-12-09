#ifndef ASYNC_H
#define ASYNC_H

#include <functional>
#include <memory>
#include <vector>
#include <string>
#include <map>

class Async
{
public:
    class Error
    {
    public:
        Error() : 
            m_code(0), m_isNull(true) 
        {}
        
        Error(int code, const std::string& message = std::string()) : 
            m_code(code), m_message(message), m_isNull(false) 
        {}
        
        int getCode() const { return m_code; }
        std::string getMessage() const { return m_message; }
        
        bool isNull() const { return m_isNull; }
        
    private:
        int m_code;
        std::string m_message;
        
        bool m_isNull;
    };
    
    typedef std::function<void(const Error&)> Callback;
    typedef std::vector<std::function<void(Callback)>> Functions;
    
    static void series(const Functions& functions, Callback callback);
    static void parallel(const Functions& functions, Callback callback);
    
private:
    class State
    {
    public:
        State(const Functions& functions, Callback callback) :
            m_id(m_currentId++), m_functions(functions), m_callback(callback), m_counter(functions.size()), m_hasFinished(false)
        {}
        
        virtual ~State() {}
        
        virtual void update(const Error& error) = 0;
        
        size_t getId() const { return m_id; }
        
    protected:
        static size_t m_currentId;
        size_t m_id;
        
        Functions m_functions;
        Callback m_callback;
        size_t m_counter;
        bool m_hasFinished;
    };
    
    class SeriesState : public State
    {
    public:
        SeriesState(const Functions& functions, Callback callback) :
            State(functions, callback)
        {
            update(Error());
        }
        
        void update(const Error& error) override;
    };
    
    class ParallelState : public State
    {
    public:
        ParallelState(const Functions& functions, Callback callback) :
            State(functions, callback), m_isInitialized(false)
        {
            update(Error());
        }
        
        void update(const Error& error) override;
        
    private:        
        bool m_isInitialized;
    };
    
    static void removeState(State* state);
    static std::map<size_t, std::unique_ptr<State>> m_states;
};

#endif // ASYNC_H
