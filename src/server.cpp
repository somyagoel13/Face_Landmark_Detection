#include "server.h"
#include <fstream>


using namespace std;
using json = nlohmann::json;

Server::Server():m_context(1), m_socket(m_context, ZMQ_REP)
{
  JsonMode mode = WRITE;
  m_jsonObj = new JsonParser(mode);
}

void Server::bind(const std::string &address)
{
  m_socket.bind(address);
}

void Server::appendData(string data)
{
  json obj;

  try
  {
    obj = json::parse(data);
  }
  catch (const nlohmann::json::parse_error& e)
  {
    std::cerr << "JSON parse error: " << e.what() << std::endl;
    return;
  }

  for (auto it = obj["images"].begin(); it != obj["images"].end(); it++)
  {
    m_jsonObj->addFrame(it.key(), it.value());
  }
}

void Server::writeToFile()
{
  string data = m_jsonObj->getJsonString();

  //initialise object 
  ofstream outputFile;
  outputFile.open("log.txt"); // Opens for appending
  if (!outputFile.is_open()) {
	// Handle error, e.g., print an error message
	cout << "Error: Could not open file!" << std::endl;
	return ; // Indicate an error
  }

  // write data to output file
  outputFile << data ;
  // close file
  outputFile.close();
}

void Server::start()
{
  while (true) {
    zmq::message_t request;
    m_socket.recv(request, zmq::recv_flags::none);
    std::string msg(static_cast<char*>(request.data()), request.size());

    std::cout << "Server received: " << msg << std::endl;

    json req = json::parse(msg);
    std::string method = req["method"];
    std::string timestamp = req.value("timestamp", "");
    json response;

    if (method == "GET") {
      if (m_dataStore.find(timestamp) != m_dataStore.end()) {
        response = { {"status", "OK"}, {"timestamp", timestamp}, {"data", m_dataStore[timestamp]} };
      } else {
        response = { {"status", "NOT_FOUND"}, {"timestamp", timestamp} };
      }

      // just a placeholder for now
      // Create a db which contains data like
      // from_time, to_time, file_name
      // this meta data could be used to access file name quickly
      // then fetch the data from file
    } 
    else if (method == "POST") {
      /// TODO::
      // sanitize the data before entry
      // eg. verify data structure
      // verify data authentic or not
      // encryption could be added
      appendData(req["data"]);
      m_dataStore[timestamp] = req["data"];
      response = { {"status", "OK"}, {"message", "Data stored"}, {"timestamp", timestamp} };
    } 
    else if (method == "DELETE") {
      if (m_dataStore.erase(timestamp)) {
        response = { {"status", "OK"}, {"message", "Data deleted"}, {"timestamp", timestamp} };
      } else {
        response = { {"status", "NOT_FOUND"}, {"timestamp", timestamp} };
      }
      // just a placeholder
      // Could be used when old data needs to be deleted
      // This should delete the data as well.
    } 
    else if (method == "WRITE") {
      /// TODO::
      // Create buffer and create a new file to write when buffer size exceeds
      // some defined size
      // File name should be fromTimestamp_toTimestamp
      writeToFile();
    }
    else {
      response = { {"status", "ERROR"}, {"message", "Unknown method"} };
    }

    std::string resp_str = response.dump();
    zmq::message_t reply(resp_str.size());
    memcpy(reply.data(), resp_str.data(), resp_str.size());
    m_socket.send(reply, zmq::send_flags::none);
  }
}
