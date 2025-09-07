#ifndef faceDetector_h
#define faceDetector_h

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <fstream>

using namespace std;
using namespace cv;

class FaceDetector
{
  public:

  /*
  * @brief : contructor
  * 
  * @param [in]: faceDetectionModelPath, model path 
   */
  FaceDetector(const string faceDetectionModelPath ) ; 

  /*
  *@brief : function to re-initialise parameters for fresh frame 
   */
  void initializer();

  /*
  *@brief :main function to be called from manager class, returns the output vectors of rects and face rois
  * @param[in] : Mat frame, input image 
  * @param[out] : faceROIV, vector for face Mats detected in a frame
  * @param[out] : faceRectV , vector having rect coordinates of detected faces.
   */
  void returnFaceVectors(vector<Mat> &faceROIV, vector<Rect> &faceRectV , Mat frame ); 

  private:
    /*
    * @brief : function to load the onnx model and store in m_faceDetector
    * param[in] : faceDetectionModelPath, const string to the onnx model
     */
    void loadModel(const string faceDetectionModelPath );
    /*
    * @brief : main function to detect face and store face coordinates in m_faceRects
    * @param[in] : frame, video frame for detection.
     */
    void detectFace(Mat frame );


    // vector to store the face detection by model.
    vector <Rect> m_faceRectsV ;

    // vector to store cropped faces
    vector <cv::Mat> m_faceROIsV ;
    // pointer to save model. 
    cv::Ptr<cv::FaceDetectorYN> m_faceDetector;

};

#endif
