#include "timeUtil.h"

using namespace std;

// brief: get current time(upto seconds) in epoch
//        size < 1e12
// param[out]: long long int, epoch
long long int TimeUtil::getEpochSecond()
{
  return std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

// brief: get current time(upto milli seconds) in epoch
//        size > 1e12
// param[out]: long long int, epoch
long long int TimeUtil::getEpochMilliSecond()
{
  return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

// brief: setting epoch with accuracy upto seconds
//        setting size of epoch to < 1e12
// param[in] epoch, time in epoch
// param[out] time_t, epoch(in seconds) casted to time_t 
std::time_t TimeUtil::normalizeEpochSeconds(long long int epoch)
{
  if (epoch > 1e12) { 
    // if value is too large → assume ms
    return static_cast<std::time_t>(epoch / 1000);
  }
  return static_cast<std::time_t>(epoch);
}

// brief: if epoch is for milli second accuracy
// param[in] epoch, time in epoch
// param[out] bool, true if epoch accuracy in milli second
//                  false if not
bool TimeUtil::isMilliseconds(long long int epoch)
{
  return epoch > 1e12;
}

// brief: get time in readable format like yyyy-mm-dd hh:mm:ss
//
// param[in] epoch, epoch to be formatted
// param[in] format, required output format
// param[out] ostringstream, string stream with time in formats like yyyy-mm-dd hh:mm:ss
std::ostringstream TimeUtil::formatFromEpoch(long long int epoch, const char* format)
{
  std::time_t t = normalizeEpochSeconds(epoch);
  std::tm tm = *std::localtime(&t); // use std::gmtime for UTC

  std::ostringstream oss;
  oss << std::put_time(&tm, format);
  return oss;
}


// brief: get time in readable format like hh:mm:ss.mmm
//
// param[in] epoch, epoch to be formatted
// param[in] format, required output format
// param[out] string, string with time in formats like hh:mm:ss.mmm
string TimeUtil::getTimeFromEpoch(long long int epoch)
{
  std::ostringstream oss = formatFromEpoch(epoch, "%H:%M:%S");

  if (isMilliseconds(epoch)) {
    int ms = static_cast<int>(epoch % 1000);
    oss << "." << std::setfill('0') << std::setw(3) << ms; // add .mmm
  }

  return oss.str();
}

// brief: get time in readable format like yyyy-mm-dd
//
// param[in] epoch, epoch to be formatted
// param[in] format, required output format
// param[out] string, string with time in formats like yyyy-mm-dd
string TimeUtil::getDateFromEpoch(long long int epoch)
{
  return formatFromEpoch(epoch, "%Y-%m-%d").str();
}

// brief: get time in readable format like yyyy-mm-dd hh:mm:ss.mmm
//
// param[in] epoch, epoch to be formatted
// param[in] format, required output format
// param[out] string, string with time in formats like yyyy-mm-dd hh:mm:ss.mmm
string TimeUtil::getDateTimeFromEpoch(long long int epoch)
{
  std::ostringstream oss = formatFromEpoch(epoch, "%Y-%m-%d %H:%M:%S");

  if (isMilliseconds(epoch)) {
    int ms = static_cast<int>(epoch % 1000);
    oss << "." << std::setfill('0') << std::setw(3) << ms; // add .mmm
  }

  return oss.str();
}

/*
int main()
{
  TimeUtil obj;
  long long int sec = 1693305600;
  long long int ms = 1693305600123;

  cout << "Current time: " << obj.getEpochSecond() << endl;
  cout << "Current milli sec time: " << obj.getEpochMilliSecond() << endl;
  cout << "Date from sec: " << obj.getDateFromEpoch(sec) << endl;
  cout << "Date from milli sec: " << obj.getDateFromEpoch(ms) << endl;
  cout << "Time from sec: " << obj.getTimeFromEpoch(sec) << endl;
  cout << "Time from milli sec: " << obj.getTimeFromEpoch(ms) << endl;
  cout << "Date time from sec: " << obj.getDateTimeFromEpoch(sec) << endl;
  cout << "Date time from milli sec: " << obj.getDateTimeFromEpoch(ms) << endl;

  return 0;
}
*/
