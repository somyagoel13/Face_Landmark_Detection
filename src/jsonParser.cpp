#include "jsonParser.h"

using json = nlohmann::json;
using namespace std;

//images:{
//    frame_1: {
//      timestamp: epoch,
//      faces:{
//        landmark_1: {x: x1, y: y1},
//        landmark_2: {x: x2, y: y2},
//        landmark_3: {x: x3, y: y3}
//      }
//    }
//}

// brief: read mode to be used on server side majorly for get queries.
//        write mode is on client side to insert face landmarks.
// param[in] mode, initialize the class object as read or write mode
JsonParser::JsonParser(JsonMode mode)
{
  m_mode = mode;
  m_jsonObj = json::object();
  
  if (m_mode == WRITE)
  {
    initJson();
  }
}

// brief: to be used in write mode only
//        initiates m_jsonObj to {images:{}}
void JsonParser::initJson()
{
  // initializing the json to add frames later
  m_jsonObj = {{"images", json::object()}};
}

// brief: to be used in read mode only
//        reads json string and saves it in m_jsonObj
// param[in] jsonStr, json string
// param[out] int,  0 in case of error
//                  1 for success
int JsonParser::readJson(string jsonStr)
{
  try
  {
    m_jsonObj = json::parse(jsonStr);
  }
  catch (const nlohmann::json::parse_error& e)
  {
    std::cerr << "JSON parse error: " << e.what() << std::endl;
    return 0;
  }

  return 1;
}

// brief: create json object of the frame
// param[in]: frameId, frame id eg. frame_1, frame_2
// param[in]: timestamp, timestamp in string format dd:mm:yyyy:time
// param[in]: landmarks, vector of pair containing {x, y} co-ordinates for landmarks
// param[out]: json object, return json object of the frame
json JsonParser::createFrame(string frameId,
                             long long int timestamp,
                             vector<vector<pair<int, int>> > landmarks)
{
  json frameObj;
  frameObj["timestamp"] = timestamp;

for(int face = 0;face<landmarks.size();++face)
{
  json faces;
  int size = landmarks[face].size();
  string landmark = "landmark_";
  for (int i = 0; i < size; i++)
  {
    string key = landmark + to_string(i + 1);
    faces[key] = {
      {"x", landmarks[face][i].first},
      {"y", landmarks[face][i].second}
    };
  }
  string faceID = "face_"+to_string(face);
  frameObj[faceID] = faces;
}
  return frameObj;
}

// brief: insert frame details into the json object
// param[in]: frameId, frame id eg. frame_1, frame_2
// param[in]: timestamp, timestamp in string format dd:mm:yyyy:time
// param[in]: landmarks, vector of pair containing {x, y} co-ordinates for landmarks
// param[out]: int, 1 for success, if frame already present
//                  0 for failure
int JsonParser::addFrame(string frameId,
                         long long int timestamp,
                         vector<vector<pair<int, int>> >landmarks)
{
  // check if frame already present
  if (m_jsonObj["images"].contains(frameId))
  {
    return 0;
  }
  else
  {
    json frame = createFrame(frameId, timestamp, landmarks);
    m_jsonObj["images"][frameId] = frame;
  }

  return 1;
}

int JsonParser::addFrame(string frameId, json frame)
{
  // check if frame already present
  if (m_jsonObj["images"].contains(frameId))
  {
    return 0;
  }
  else
  {
    m_jsonObj["images"][frameId] = frame;
  }
   
  return 1;
}

// brief: get string of member json object
// param[out] string, m_json object value in string format
string JsonParser::getJsonString()
{
  return m_jsonObj.dump();
}

// brief: search landmark using timestamp
// param[in]: timestamp, epoch
// param[out]: json, json object of the face id
json JsonParser::getFaceDataFromTimestamp(long long int timestamp)
{
  json face = json::object();
  if (m_jsonObj.empty())
  {
    return face; // return empty object
  }

  for (auto it = m_jsonObj["images"].begin(); it != m_jsonObj["images"].end(); it++)
  {
    json temp = it.value();
    if (temp["timestamp"] == timestamp)
    {
      face = it.value();
      break;
    }
  }

  return face;
}

