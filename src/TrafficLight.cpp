#include <iostream>
#include <thread>
#include <random>
#include <future>
#include "TrafficLight.h"
#include <ctime>

/* Implementation of class "MessageQueue" */

template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait()
    // to wait for and receive new messages and pull them from the queue using move semantics.
    // The received object should then be returned by the receive function.

  std::unique_lock<std::mutex> uLock(_mutex);
  _cond.wait(uLock, [this]{return !_queue.empty(); });

  T msg = std::move(_queue.back());
  _queue.pop_back();

  return msg;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
  std::lock_guard<std::mutex> uLock(_mutex);

  _queue.push_back(std::move(msg));
  _cond.notify_one();
}

/* Implementation of class "TrafficLight" */

TrafficLight::TrafficLight() {
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop
    // runs and repeatedly calls the receive function on the message queue.
    // Once it receives TrafficLightPhase::green, the method returns.
  while(true) {
    auto currentPhase = queue->receive();
    if (currentPhase == TrafficLightPhase::green)
      return;
  }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class.
  threads.push_back(std::thread(&TrafficLight::cycleThroughPhases,this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles
    // and toggles the current phase of the traffic light between red and green and sends an update method
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds.
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles.
    auto _timePast = std::chrono::high_resolution_clock::now();
    //random value between 4 and 6 secs
    srand(time(NULL));
    auto cycle_duration = rand()% (6 - 4 + 1) + 4;


    while(true) {

      std::this_thread::sleep_for(std::chrono::milliseconds(1));

      auto _timeNow = std::chrono::high_resolution_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::seconds>(_timeNow - _timePast).count();
      if (duration >= cycle_duration) {
        _currentPhase = _currentPhase == TrafficLightPhase::red ?
            TrafficLightPhase::green : TrafficLightPhase::red;

        queue->send(std::move(_currentPhase));

        _timePast = _timeNow;
        cycle_duration = rand()% (6 - 4 + 1) + 4;
      }
    }
}

