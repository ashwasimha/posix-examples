#include <iostream>
#include <thread>
#include "unistd.h"   

#include "message_queue.h"
#include "posix_queue.h"

using namespace std;
constexpr char MSG_QUEUE_NAME[] = "dataPointsQueue";
constexpr unsigned int MAX_NO_OF_MESSAGES_IN_QUEUE = 10;
constexpr unsigned int MAX_MSG_SIZE = 1024;
constexpr unsigned int SPEED_SENSOR = 1;
constexpr unsigned int ENGINE_SENSOR = 2;

struct sensorData_t{
    unsigned int sensorType{0};
    unsigned int sensorValue{0};
};

// vehicle speed simulator
void speed()
{    
    std::unique_ptr<IMessageQueue>  m_ptrMessageQueue{new CPosixQueue()};
    if( !(m_ptrMessageQueue->
                openQueue(MSG_QUEUE_NAME,
                          MAX_MSG_SIZE,
                          MAX_NO_OF_MESSAGES_IN_QUEUE)) ){
        cerr << "Failed to Create a message Queue in Speed" << endl;
        exit(0);
    }
    
    int vehicle_speed = 0;
    while(1){
        
        if(vehicle_speed >= 100)
            vehicle_speed = 0;
        // cout << "vehicle speed:: " << vehicle_speed << endl;
        
        
        sensorData_t sensor_data;
        sensor_data.sensorType = SPEED_SENSOR;
        sensor_data.sensorValue = vehicle_speed;
        m_ptrMessageQueue->
                    writeToQueue((char*)&sensor_data,
                                 sizeof(sensorData_t));
        
        sleep(5);
        ++vehicle_speed;
    }
}  

// engine rpm simulator
void rpm()
{
    std::unique_ptr<IMessageQueue>  m_ptrMessageQueue{new CPosixQueue()};
    if( !(m_ptrMessageQueue->
                openQueue(MSG_QUEUE_NAME,
                          MAX_MSG_SIZE,
                          MAX_NO_OF_MESSAGES_IN_QUEUE)) ){
        cerr << "Failed to Create a message Queue in Speed" << endl;
        exit(0);
    }

    int engine_speed = 0;
    while(1){
        if(engine_speed >= 1000)
            engine_speed = 0;
        
        sensorData_t sensor_data;
        sensor_data.sensorType = ENGINE_SENSOR;
        sensor_data.sensorValue = engine_speed;
        m_ptrMessageQueue->
                    writeToQueue((char*)&sensor_data,
                                 sizeof(sensorData_t));


        sleep(10);
        ++engine_speed;
    } 
}  

void ui()
{
    std::unique_ptr<IMessageQueue>  m_ptrMessageQueue{new CPosixQueue()};
    if( !(m_ptrMessageQueue->
                openQueue(MSG_QUEUE_NAME,
                          MAX_MSG_SIZE,
                          MAX_NO_OF_MESSAGES_IN_QUEUE)) ){
        cerr << "Failed to Create a message Queue in Speed" << endl;
        exit(0);
    }

    while(1){
        // cout << "UI Waiting for the messages" << endl;
        
        sensorData_t sensor_data;
        if(!(m_ptrMessageQueue->
          readFromQueue((char*)&sensor_data)))
        {
            cerr << "Failed to read from queue in UI" << endl;
            exit(0);
        }

        if(sensor_data.sensorType == 1 ){
            cout << "Vehicle Speed : " << sensor_data.sensorValue << endl;        
        }else {
            cout << "Engine RPM : " << sensor_data.sensorValue << endl;        
        }
    }    
}

int main()
{
    //start a component that simulates speed
    thread speedometer(speed);
    
    //start a component that simulates engine rpm
    thread enginerpm(rpm);
    
    // start the display thread 
    thread display(ui);
    
    //wait for child threads to finish the task
    speedometer.join();
    enginerpm.join(); 
    display.join();
  
    return 0;
}