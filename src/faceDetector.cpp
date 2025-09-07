#include "faceDetector.h"

/*
 * constructor
*/
FaceDetector :: FaceDetector(const string faceDetectionModelPath)
{

  // load model
  loadModel(faceDetectionModelPath);
  // initialize empty vectors
  initializer();
}

/*
* Detailed Description :
* fuction to load the given model .
* it loads model in the class variable.
*
 */
void FaceDetector::  loadModel(const string faceDetectionModelPath)
{

  // Load YuNet face detector
  m_faceDetector = cv::FaceDetectorYN::create(faceDetectionModelPath,"" , cv::Size(320, 320));

  std::cout << "model loaded successfully \n" ;

  // print error when no model is loaded
  if (m_faceDetector.empty()) {
    std::cerr << "Failed to load YuNet face detector!" << std::endl;
    return;
  }


}

/*
* Detailed Description
* Function to initialise the parameters.
* it is called each time a new frame is captured,
* to ensure fresh outputs.
 */
void FaceDetector :: initializer()
{
  m_faceROIsV.clear();
  m_faceRectsV.clear();
}

/*
* Detailed Description : 
* to run the model inference
* save the face roi (rect coordinates)
* save the cropped face images
 */

void FaceDetector :: detectFace(Mat frame)
{
  // Detect faces with YuNet
  cv::Mat faces;
  m_faceDetector->setInputSize(frame.size());
  if (m_faceDetector->detect(frame, faces) != 0) {
    // Process each detected face
    for (int i = 0; i < faces.rows; i++) {
      // Extract face coordinates (YuNet format: [x, y, w, h, conf, ...])
      int x = static_cast<int>(faces.at<float>(i, 0));
      int y = static_cast<int>(faces.at<float>(i, 1));
      int w = static_cast<int>(faces.at<float>(i, 2));
      int h = static_cast<int>(faces.at<float>(i, 3));
      float confidence = faces.at<float>(i, 14);

      // Only process high-confidence detections
      if (confidence > 0.9) {
        cv::Rect face_roi(x, y, w, h);

        // Ensure the ROI is within frame bounds
        if (face_roi.x >= 0 && face_roi.y >= 0 && 
            face_roi.x + face_roi.width <= frame.cols && 
            face_roi.y + face_roi.height <= frame.rows) 
        {

          // Extract the face region
          cv::Mat face_img = frame(face_roi).clone();

          cv::imwrite("faceYunet.png",face_img);
          m_faceROIsV.push_back(face_img);
          m_faceRectsV.push_back(face_roi);
        }
      }
    }
  }
}

void FaceDetector :: returnFaceVectors(vector<Mat> &faceROIV, vector<Rect> &faceRectV , Mat frame ) 
{
  detectFace(frame); 
  faceROIV = m_faceROIsV ;
  faceRectV = m_faceRectsV ;
  }
