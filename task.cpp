#include <algorithm>
#include "task.h"


using namespace test_task;

states device_adapter::getDeviceState() const {
    return device_state;
}

states device_adapter::getConnectionState() const {
    return connection_state;
}

double device_adapter::getConnectionTime() const {
    if (connection_state == states::CONNECTED)
    {
        clock_t time_now = clock();
        return static_cast<double>(time_now - start_time) / CLOCKS_PER_SEC;
    } else
        return 0;
}

void publisher::addDevice(device_adapter *device) {
    devices.emplace_back(device);
}

void publisher::addSubscriber(subscriber_interface *subscriber) {
    subscribers.emplace_back(subscriber);
}

void publisher::removeDevice(device_adapter *device) {
    auto iter{ std::find(devices.begin(), devices.end(), device) };
    if ( iter != devices.end() ){
        devices.erase(iter);
    }
}

void publisher::removeSubscriber(subscriber_interface *sub) {
    auto iter{ std::find(subscribers.begin(), subscribers.end(), sub) };
    if ( iter != subscribers.end() ){
        subscribers.erase(iter);
    }
}

void publisher::notifySubscribers() {
    for (auto &device: devices){
        std::vector<uint8_t> output;
        if (device->read_bytes(output)){
            for (auto &subscriber: subscribers){
                subscriber->update_data(output);
            }
        }
    }
}



