#include "faceLandmark.h"

FaceLandmark :: FaceLandmark(const string faceLandMarkDetectionModelPath )
{
  loadModel(faceLandMarkDetectionModelPath);
  initialise();
  }

/*
* Function to load model and savw in m_landmarkNet
 */
void FaceLandmark:: loadModel (const string landmarkNetModelPath)
{
  // Load the Qualcomm facial landmark model
  m_landmarkNet = cv::dnn::readNet(landmarkNetModelPath);
  //cv::dnn::Net landmark_net = cv::dnn::readNet("/home/yankee/Desktop/test/model.onnx/model.onnx");

  if (m_landmarkNet.empty()) {
    std::cerr << "Failed to load the facial landmark model!" << std::endl;
    return ;
  }

}

/*
* Function to reset the parameters for each frame.
 */

void FaceLandmark :: initialise()
{
  m_faceLandmarkV.clear();

}
// Function to detect facial landmarks using onnx model
void FaceLandmark :: detectLandmarks(vector <Mat>& faceRoiV) 
{

  //for(auto itr: faceRoiV)
  for(int i=0;i<faceRoiV.size();++i)
  {
    // vector to save landmarks    
    std::vector<cv::Point2f> landmarks;

    // save current Mat in face roi
    Mat face_roi = faceRoiV[i];
    // check for empty or invalid face size
    if (face_roi.empty() || face_roi.cols < 10 || face_roi.rows < 10) 
    {
      // printing invalid message and exiting function.  
      std::cerr << "Invalid face ROI provided!" << std::endl;
      return ;
    }

    try {
      // Preprocess the image according to the model's requirements
      // The model expects RGB input normalized to [0, 1]
      cv::Mat resized, rgb;
      cv::resize(face_roi, resized, cv::Size(192, 192));
      cv::cvtColor(resized, rgb, cv::COLOR_BGR2RGB);
      rgb.convertTo(rgb, CV_32F, 1.0/255.0);

      // Create blob from image
      cv::Mat blob = cv::dnn::blobFromImage(rgb);
      m_landmarkNet.setInput(blob);

      // Forward pass
      cv::Mat output = m_landmarkNet.forward();

      // The model outputs 468 landmarks with 3 coordinates each (x, y, z)
      // We only need x and y coordinates
      int num_landmarks = output.size[1];

      // iterating through the vector
      for (int i = 0; i < num_landmarks; i++) {
        // Get normalized coordinates (0-1 range)
        float x = output.at<float>(0, i, 0);
        float y = output.at<float>(0, i, 1);

        // Convert to pixel coordinates in the face ROI
        float px = x * face_roi.cols;
        float py = y * face_roi.rows;

        // saving in vector model results
        landmarks.push_back(cv::Point2f(px, py));
      }
    } 
    // safe check if in case model fails  
    catch (const cv::Exception& e) {
      std::cerr << "Error in landmark detection: " << e.what() << std::endl;
    }
    m_faceLandmarkV.push_back(landmarks); //return landmarks;
  }
}

/*
* Main function called from outside class , and return the facial landmark per frame. 
 */
vector <vector<pair<int,int> > > FaceLandmark:: returnLandmarkPoints(vector<Mat> faceRoiV )
{
  // re set all parameter values to default
  initialise(); 

  // final vector containing detections, to be returned.
  vector <vector<pair<int,int> > > facialLandmarkV ; 

  // detecting landmark and save in member  variable. 
  detectLandmarks(faceRoiV);

  // iterating through landmark points recieved.
  // first loop for iterating via num of faces.
  for(int  i=0;i<m_faceLandmarkV.size();++i)
  {
    vector < pair<int,int> > currFaceLandMark ; 
    // for ach face iterating points
    for(int j =0;j<m_faceLandmarkV[i].size();++j)
    {
      currFaceLandMark.push_back( {int(m_faceLandmarkV[i][j].x) ,int(m_faceLandmarkV[i][j].y) });
    }
    // insert for a face roi
    facialLandmarkV.push_back(currFaceLandMark);
  }
  return facialLandmarkV;
}
