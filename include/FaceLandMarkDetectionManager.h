#ifndef FaceLandmarkDetectionManager_h
#define FaceLandmarkDetectionManager_h

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>
#include <vector>

#include "faceLandmark.h"
#include "faceDetector.h"
#include "jsonParser.h"
#include "timeUtil.h"

using namespace std;
using namespace cv;


class FaceLandmarkDetectionManager
{
  public:
 /*
 * @brief : constructor
  * @param[in] : detectorModelPath, face detection model
  * @param[in]: landmarkModelPath, face landmark model
  * @param[in] : jsonMode, mode in which json file is initialised.
 */
 FaceLandmarkDetectionManager(const string detectorModelPath, const string landmarkModelPath, JsonMode jsonMode) ;
/*
 * @brief : Function to get detection, this is main function to be called from main class
 *          this calls detection on every frame
*/
string getDetections();
private:

// Object to face detector
  FaceDetector *m_faceDetectorObj ;
  
  // Object to landmark detection class 
  FaceLandmark *m_faceLandmarkObj;
  
  // Object to json parser class 
  JsonParser *m_jsonParserObj;
   
   /*
    * @brief : Function to initialise the member variables for all classes
    * 
   */
 void initialise();
  
/*
 * @brief: function to perform detection on single frame in the video stream
 *
 * @param[in] : frame, input mat of frame
 * @param[in] : frameCount, frame number for given frame
 * writes the detection to json obejct  
*/
  void doDetectionOnFrame(Mat frame ,int frameCount);
   

};
#endif
