#include "Async.h"

size_t Async::State::m_currentId = 0;
std::map<size_t, std::unique_ptr<Async::State>> Async::m_states;


void Async::series(const Functions& functions, Callback callback)
{
    std::unique_ptr<State> state(new SeriesState(functions, callback));
    m_states[state->getId()] = std::move(state);
}

void Async::parallel(const Functions& functions, Callback callback)
{
    std::unique_ptr<State> state(new ParallelState(functions, callback));
    m_states[state->getId()] = std::move(state);
}

void Async::removeState(Async::State* state)
{
    auto it = m_states.find(state->getId());
    if (it != m_states.end()) {
        m_states.erase(it);
    }
}


void Async::SeriesState::update(const Error& error)
{
    if ((m_counter == 0 || !error.isNull()) && !m_hasFinished) {
        m_hasFinished = true;
        m_callback(error);    
        Async::removeState(this);
        return;
    }
    
    --m_counter;
    
    auto it = m_functions.rbegin() + m_counter;
    (*it)([this](const Error& error) {
        update(error);
    });
}

void Async::ParallelState::update(const Error& error)
{
    if ((m_counter == 0 || !error.isNull()) && !m_hasFinished) {
        m_hasFinished = true;
        m_callback(error);
        Async::removeState(this);
        return;
    }
    
    --m_counter;
    
    if (!m_isInitialized) {
        m_isInitialized = true;
        
        auto cb = [this](const Error& error) {
            update(error);
        };
        
        for (auto& f : m_functions) {
            f(cb);
        }
    }
}
