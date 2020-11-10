#include <iostream>
#include <cstring>
#include <cassert>
#include "task.h"

class device_1 : public test_task::device_adapter{
public:
    void connect() override {
        start_time = clock();
        connection_state = test_task::states::CONNECTED;
    }
    void disconnect() override {
        connection_state = test_task::states::DISCONNECTED;
    }

    void update_state() override {}

    size_t read_bytes(std::vector<uint8_t> &data) override {
        const char *mes = {" mes_1 "};
        size_t mes_len{ strlen(mes)};
        std::vector<uint8_t> output;
        output.reserve(mes_len);
        for ( size_t byte = 0; byte < mes_len; ++byte )
            output.emplace_back(mes[byte]);
        data = output;
        return mes_len;
    }
};

class device_2 : public test_task::device_adapter{
public:
    void connect() override {
        start_time = clock();
        connection_state = test_task::states::CONNECTED;
    }
    void disconnect() override {
        connection_state = test_task::states::DISCONNECTED;
    }

    void update_state() override {}

    size_t read_bytes(std::vector<uint8_t> &data) override {
        const char *mes = {" mes_2 "};
        size_t mes_len{ strlen(mes)};
        std::vector<uint8_t> output;
        output.reserve(mes_len);
        for ( size_t byte = 0; byte < mes_len; ++byte )
            output.emplace_back(mes[byte]);
        data = output;
        return mes_len;
    }
};

class subscriber_1 : public test_task::subscriber_interface{
    std::string current_data;
public:
    void update_data(const std::vector<uint8_t> &data) override {
        std::string temp_string;
        temp_string.reserve(data.size());
        for ( auto &byte: data)
            temp_string.push_back(byte);

        current_data += temp_string;
    }

    auto getData() const {
        return current_data;
    }
};

class subscriber_2 : public test_task::subscriber_interface{
    std::string current_data;
public:
    void update_data(const std::vector<uint8_t> &data) override {
        std::string temp_string;
        temp_string.reserve(data.size());
        for ( auto &byte: data)
            temp_string.push_back(byte);

        current_data += temp_string += "smth...";
    }

    auto getData() const {
        return current_data;
    }
};

class device_int_type : public test_task::device_adapter{
public:
    void connect() override {
        start_time = clock();
        connection_state = test_task::states::CONNECTED;
    }
    void disconnect() override {
        connection_state = test_task::states::DISCONNECTED;
    }

    void update_state() override {}

    size_t read_bytes(std::vector<uint8_t> &data) override {
        int message[] = {1,2,3};
        size_t message_length = 3;
        std::vector<std::uint8_t> output;
        size_t bytes_length = message_length * sizeof(message[0]);
        output.reserve(message_length * sizeof(message[0]));
        for (size_t index = 0; index < bytes_length; ++index){
            output.emplace_back(*(reinterpret_cast<uint8_t*>(message) + index));
        }

        data = output;
        return output.size();

    }
};

class subscriber_int_type : public test_task::subscriber_interface{
    std::vector<int> current_data;
public:
    void update_data(const std::vector<uint8_t> &data) override {
        int temp;
        for ( int index = 0; index < data.size();){
            temp = 0;
            for (int i =0 ; i < sizeof(temp); ++i)
            {
                *(reinterpret_cast<uint8_t*>(&temp) + i) = data[index++];
            }
            current_data.emplace_back(temp);
        }


    }

    auto getData() const {
        return current_data;
    }
};

int main() {

    //устройства посылают строку
    test_task::publisher publisher_;
    device_1 dev_1{};
    device_2 dev_2{};
    subscriber_1 sub_1{};
    subscriber_2 sub_2{};
    publisher_.addDevice(static_cast<test_task::device_adapter*>(&dev_1));
    publisher_.addDevice(static_cast<test_task::device_adapter*>(&dev_2));
    publisher_.addSubscriber(static_cast<test_task::subscriber_interface*>(&sub_1));
    publisher_.addSubscriber(static_cast<test_task::subscriber_interface*>(&sub_2));
    publisher_.notifySubscribers();

    std::string expected_string_first_sub = {" mes_1  mes_2 "};
    std::string expected_string_second_sub = {" mes_1 smth... mes_2 smth..."};

    assert(expected_string_first_sub == sub_1.getData());
    assert(expected_string_second_sub == sub_2.getData());

    //устройства посылаю массив типа int
    test_task::publisher publisher_int_type;
    device_int_type dev_int;
    subscriber_int_type sub_int;

    publisher_int_type.addDevice(static_cast<test_task::device_adapter*>(&dev_int));
    publisher_int_type.addSubscriber(static_cast<test_task::subscriber_interface*>(&sub_int));

    publisher_int_type.notifySubscribers();

    std::vector<int> expected_arr = {1,2,3};
    assert(expected_arr == sub_int.getData());



    return 0;
}
