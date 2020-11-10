
#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>
#include <ctime>

#define OUTPUT_CAPACITY 1024

namespace test_task {

    //интерфейс подписчикв
    class subscriber_interface {
    public:
        virtual void update_data(const std::vector<uint8_t> &data) = 0;
    };

    enum class states {
        SUCCESS,
        FAIL,
        CONNECTED,
        DISCONNECTED,
        NOT_AVALIBLE
    };

    //интерфейс адаптера к устройству для издателя
    class device_adapter {
    protected:
        clock_t start_time;
        states connection_state, device_state;
        std::vector<subscriber_interface*> subscribers;
    public:
        virtual void connect() = 0;

        virtual void disconnect() = 0;

        virtual void update_state() = 0;

        virtual size_t read_bytes(std::vector<uint8_t> &data) = 0;

        states getConnectionState() const;

        states getDeviceState() const;

        double getConnectionTime() const;
    };

    //издатель
    class publisher
    {
        std::vector<device_adapter*> devices;
        std::vector<subscriber_interface*> subscribers;
    public:
        void addDevice(device_adapter *device);
        void addSubscriber(subscriber_interface *subscriber);
        void removeDevice(device_adapter *device);
        void removeSubscriber(subscriber_interface *sub);
        void notifySubscribers();
    };






}

#endif