#ifndef client_H
#define client_H


#include <zmq.hpp>
#include <string>
#include <iostream>
#include <chrono>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Client{
  private:

    zmq::context_t m_context;

    zmq::socket_t m_socket;

    long long currentEpochMs();

  public:

    Client();

    void connect(const std::string &address);

    void sendRequest(const std::string &method, const json &data = {}, long long timestamp = -1);
};

#endif
