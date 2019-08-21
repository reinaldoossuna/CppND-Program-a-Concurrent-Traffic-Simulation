cmake_minimum_required(VERSION 3.1...3.15)

project(Traffic_simulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
find_package(OpenCV 4.1 REQUIRED
  COMPONENTS
    core
    imgproc
    highgui)

# Glob it's not recommended
# :r !ls src/*.cpp
set(SOURCES
  src/Graphics.cpp
  src/Intersection.cpp
  src/Street.cpp
  src/TrafficLight.cpp
  src/TrafficObject.cpp
  src/TrafficSimulator-Final.cpp
  src/Vehicle.cpp)

# it should work without this
set(SOURCES_ABS)
foreach(source ${SOURCES})
  list(APPEND SOURCES_ABS ${PROJECT_SOURCE_DIR}/${source})
endforeach()

# Add project executable
add_executable(traffic_simulation ${SOURCES_ABS})

target_link_libraries(traffic_simulation
                      PUBLIC Threads::Threads
                      PRIVATE ${OpenCV_LIBS})
