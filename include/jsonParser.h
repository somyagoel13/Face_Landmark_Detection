#ifndef jsonParser_h
#define jsonParser_h

#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;
using namespace std;

// modes for Json initialisation
enum JsonMode
{
  READ,
  WRITE
};

//images:{
//    frame_1: {
//      timestamp: epoch,
//      faces:{
//        landmark_1: {x: x, y: y},
//        landmark_2: {x: x, y: y},
//        landmark_3: {x: x, y: y}
//      }
//    }
//}

//file_name: fromTimestamp_toTimestamp.json

class JsonParser{
  private:
    // json object
    json m_jsonObj;

    // define mode 
    JsonMode m_mode;

    // initialise json
    void initJson();

    // @brief : To create a frame for per frame
    // @param[in] : frameId, ID of the corresponding frame
    // @param[in] : timestamp, time when the vector is sent.
    // @param[in] : landmarks, vector to be written in json object

    json createFrame(string frameId, long long int timestamp, vector<vector<pair<int, int>> > landmarks);

  public:

    // constructor
    // @param[in] : mode, in which class is initialised
    JsonParser(JsonMode mode);

    // brief: to be used in read mode only
    //        reads json string and saves it in m_jsonObj
    // param[in] jsonStr, json string
    // param[out] int,  0 in case of error
landmarks    int readJson(string jsonStr);

    // brief: insert frame details into the json object
    // param[in]: frameId, frame id eg. frame_1, frame_2
    // param[in]: timestamp, timestamp in string format dd:mm:yyyy:time
    // param[in]: landmarks, vector of pair containing {x, y} co-ordinates for landmarks
    // param[out]: int, 1 for success, if frame already present
    //                  0 for failure

    int addFrame(string frameId, long long int timestamp, vector<vector<pair<int, int>> > landmarks);

    int addFrame(string frameId, json frame);
    // brief: get string of member json object
    // param[out] string, m_json object value in string format

    string getJsonString();

    // brief: search landmark using timestamp
    // param[in]: timestamp, epoch
    // param[out]: json, json object of the face id

    json getFaceDataFromTimestamp(long long int timestamp);
};

#endif
