#include <iostream>
#include <thread>
#include <memory>
#include "unistd.h"   

#include "message_queue.h"
#include "posix_queue.h"

using namespace std;
constexpr char MSG_QUEUE_NAME[] = "simulatorQueue";
constexpr unsigned int MAX_NO_OF_MESSAGES_IN_QUEUE = 10;
constexpr unsigned int MAX_MSG_SIZE = 1024;
constexpr unsigned int SPEED_SENSOR = 1;
constexpr unsigned int ENGINE_SENSOR = 2;

struct sensorData_t{
    unsigned int sensorType{0};
    unsigned int sensorValue{0};
};

// vehicle speed simulator
void speed(std::shared_ptr<IMessageQueue> p_ptrMessageQueue)
{    
    int vehicle_speed = 0;
    while(1){
        
        if(vehicle_speed >= 100)
            vehicle_speed = 0;
        // cout << "vehicle speed:: " << vehicle_speed << endl;
        
        
        sensorData_t sensor_data;
        sensor_data.sensorType = SPEED_SENSOR;
        sensor_data.sensorValue = vehicle_speed;
        p_ptrMessageQueue->
                    writeToQueue((char*)&sensor_data,
                                 sizeof(sensorData_t));
        
        sleep(5);
        ++vehicle_speed;
    }
}  

// engine rpm simulator
void rpm(std::shared_ptr<IMessageQueue> p_ptrMessageQueue)
{
    int engine_speed = 0;
    while(1){
        if(engine_speed >= 1000)
            engine_speed = 0;
        
        sensorData_t sensor_data;
        sensor_data.sensorType = ENGINE_SENSOR;
        sensor_data.sensorValue = engine_speed;
        p_ptrMessageQueue->
                    writeToQueue((char*)&sensor_data,
                                 sizeof(sensorData_t));


        sleep(10);
        ++engine_speed;
    } 
}  

void ui(std::shared_ptr<IMessageQueue> p_ptrMessageQueue)
{
    while(1){
        // cout << "UI Waiting for the messages" << endl;
        
        sensorData_t sensor_data;
        if(!(p_ptrMessageQueue->
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
    //create a message queue that can be passed to all three threads 
    std::shared_ptr<IMessageQueue> posix_queue_ptr = make_shared<CPosixQueue>();
    if( !(posix_queue_ptr->
                openQueue(MSG_QUEUE_NAME,
                          MAX_MSG_SIZE,
                          MAX_NO_OF_MESSAGES_IN_QUEUE)) ){
        cerr << "Failed to Create a message Queue in Speed" << endl;
        exit(0);
    }

    //start a component that simulates speed
    thread speedometer(speed, posix_queue_ptr);
    
    //start a component that simulates engine rpm
    thread enginerpm(rpm, posix_queue_ptr);
    
    // start the display thread 
    thread display(ui, posix_queue_ptr);
    
    //wait for child threads to finish the task
    speedometer.join();
    enginerpm.join(); 
    display.join();
  
    //remove the created message queue
    posix_queue_ptr->deleteQueue();
    
    return 0;
}