#include "server.h"
#include "client.h"
#include "FaceLandMarkDetectionManager.h"
#include <thread>
#include <iostream>
#include "timeUtil.h"

using namespace std;

int main() {

  // Start server in background thread
  Server server;
  std::thread server_thread([&]() {
                            server.bind("tcp://127.0.0.1:5555");
                            server.start();
                            });

  // Let server start
  std::this_thread::sleep_for(std::chrono::seconds(1));

  // initialising client
  Client client;
  client.connect("tcp://127.0.0.1:5555");

  // store data for sample get , post and delete functionality
  TimeUtil tObj;
  long long currTimeFrame = tObj.getEpochMilliSecond();

  // model paths
  const string detectorModelPath = "../face_detection_yunet_2022mar.onnx"  ; 
  const string landmarkModelPath = "../MediaPipeFaceLandmarkDetector.onnx" ;

  // get detection for a video
  FaceLandmarkDetectionManager detectorObj(detectorModelPath, landmarkModelPath, JsonMode::WRITE ) ;
  string data = detectorObj.getDetections();

  // Store data at current time frame
  client.sendRequest("POST", data, currTimeFrame);

  client.sendRequest("WRITE", {}, currTimeFrame);

  //closing serevr threads
  server_thread.join();

  return 0;
}

