#ifndef faceLandmark_h
#define FaceLandmark_h

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

class FaceLandmark
{

public:
  /*
   * @brief : contructor
   * 
   * @param [in]: faceLandMarkDetectionModelPath, model path 
   */
  FaceLandmark(const string faceLandMarkDetectionModelPath ) ; 

  /*
  *@brief : function to re-initialise parameters for fresh frame 
  */
  void initialise();
 
vector <vector<pair<int,int> > > returnLandmarkPoints(vector<Mat> faceRoiV );

private:
  /*
   * @brief : function to load the onnx model and store in m_faceDetector
   * param[in] : landmarkNetModelPath, const string to the onnx model
   */
  void loadModel(const string landmarkNetModelPath);
 /*
   * @brief : function to detect face and store face coordinates in m_faceRects
   */
  void detectLandmarks(vector <Mat> &faceRoiV);


  // pointer to save model. 
  cv::dnn::Net m_landmarkNet ;

  // vector to save landmarks corresponding to each detected face.
  vector <vector<Point2f>> m_faceLandmarkV ; 
};

#endif
