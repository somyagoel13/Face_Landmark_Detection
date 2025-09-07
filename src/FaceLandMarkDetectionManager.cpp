#include "FaceLandMarkDetectionManager.h"

//constructor
FaceLandmarkDetectionManager :: FaceLandmarkDetectionManager(const string detectorModelPath, const string landmarkModelPath, JsonMode jsonMode)
{

  m_faceDetectorObj = new FaceDetector(detectorModelPath);
  m_faceLandmarkObj = new FaceLandmark(landmarkModelPath);
  m_jsonParserObj  = new JsonParser(jsonMode);
}

// initialise the member variable 
// calling initialisation objects for all classes
void FaceLandmarkDetectionManager :: initialise()
{

  // initliase objects 
  m_faceDetectorObj->initializer(); 

  m_faceLandmarkObj->initialise();
}

/*

* Function to perform detections on a single frame.
 */
void FaceLandmarkDetectionManager:: doDetectionOnFrame(Mat frame,int frameCount)
{
  // clear variables before running each frame
  initialise();

 // vectors for face rois and rects
  vector <Mat> faceROIV ;
  vector <Rect> faceRectV;

 // getting faces per frame
  m_faceDetectorObj->returnFaceVectors(faceROIV,faceRectV , frame);

  // getting landmark for all the faces in a frame
  vector <vector<pair<int,int> > > landMarkPointV = m_faceLandmarkObj->returnLandmarkPoints(faceROIV );

  // fetch time 
  TimeUtil tObj;
  long long currTimeStamp = tObj.getEpochMilliSecond();
  // initialise frame ID for json
  string frameID = string("frame_") + to_string(frameCount);
  
// add curent frame in json
  m_jsonParserObj->addFrame(frameID, currTimeStamp, landMarkPointV);

}

/*
 * Main function to stream video and get detections,
 * this function will be called from main class
*/
string FaceLandmarkDetectionManager :: getDetections()
{

  // Open webcam
  VideoCapture cap(0);
  if (!cap.isOpened()) {
    std::cerr << "Failed to open webcam!" << std::endl;
    return "" ;
  }

  // Set camera resolution for better performance
  cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
  cap.set(cv::CAP_PROP_FPS, 20);

  int frameCount = 0;
  Mat frame;

  // terminate after 500 frames, for now. 
  // Can be configured later as per usage
  while(frameCount<=500)
  {

    // get frame from stream 
    cap >> frame;

    // get detection on single frame
    doDetectionOnFrame(frame, frameCount);

    // incrasing frame coiunt
    frameCount ++;

  }

  // return json object as a string
  return m_jsonParserObj->getJsonString();
}
