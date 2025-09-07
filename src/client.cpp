#include "client.h"

using json = nlohmann::json;

Client::Client(): m_context(1), m_socket(m_context, ZMQ_REQ)
{}

long long Client::currentEpochMs()
{
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void Client::connect(const std::string &address)
{
  m_socket.connect(address);
}

void Client::sendRequest(const std::string &method, const json &data, long long timestamp)
{
  if (timestamp < 0) timestamp = currentEpochMs(); // default to "now"

  json req = { {"method", method}, {"timestamp", std::to_string(timestamp)}, {"data", data} };
  std::string req_str = req.dump();

  zmq::message_t request(req_str.size());
  memcpy(request.data(), req_str.data(), req_str.size());
  m_socket.send(request, zmq::send_flags::none);

  zmq::message_t reply;
  m_socket.recv(reply, zmq::recv_flags::none);
  std::string resp_str(static_cast<char*>(reply.data()), reply.size());

  std::cout << "Client received data ! "  << std::endl;
}
