#ifndef server_h
#define server_h

#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "jsonParser.h"

using json = nlohmann::json;

class Server{

  private:

    zmq::context_t m_context;

    zmq::socket_t m_socket;

    std::unordered_map<std::string, json> m_dataStore;

    JsonParser* m_jsonObj;

    void writeToFile();

    void appendData(std::string data);

  public:

    Server();

    void bind(const std::string &address);

    void start();
};

#endif
