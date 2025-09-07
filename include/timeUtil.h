#ifndef timeUtil_h
#define timeUtil_h

#include <chrono>
#include <iostream>
#include <sstream>
#include <iomanip>
//#include <ctime>

class TimeUtil {
  private:
    // brief: setting epoch with accuracy upto seconds
    //        setting size of epoch to < 1e12
    // param[in] epoch, time in epoch
    // param[out] time_t, epoch(in seconds) casted to time_t 
    std::time_t normalizeEpochSeconds(long long epoch);
    // brief: if epoch is for milli second accuracy
    // param[in] epoch, time in epoch
    // param[out] bool, true if epoch accuracy in milli second
    //                  false if not

    bool isMilliseconds(long long epoch);
    // brief: get time in readable format like yyyy-mm-dd hh:mm:ss
    //
    // param[in] epoch, epoch to be formatted
    // param[in] format, required output format
    // param[out] ostringstream, string stream with time in formats like yyyy-mm-dd hh:mm:ss

    std::ostringstream formatFromEpoch(long long epoch, const char* format);

  public:
    // brief: get current time(upto seconds) in epoch
    //        size < 1e12
    // param[out]: long long int, epoch
    long long int getEpochSecond();
    // brief: get current time(upto milli seconds) in epoch
    //        size > 1e12
    // param[out]: long long int, epoch
    long long int getEpochMilliSecond();
    // brief: get time in readable format like hh:mm:ss.mmm
    //
    // param[in] epoch, epoch to be formatted
    // param[in] format, required output format
    // param[out] string, string with time in formats like hh:mm:ss.mmm

    std::string getTimeFromEpoch(long long int epoch);
    // brief: get time in readable format like yyyy-mm-dd
    //
    // param[in] epoch, epoch to be formatted
    // param[in] format, required output format
    // param[out] string, string with time in formats like yyyy-mm-dd

    std::string getDateFromEpoch(long long int epoch);
    // brief: get time in readable format like yyyy-mm-dd hh:mm:ss.mmm
    //
    // param[in] epoch, epoch to be formatted
    // param[in] format, required output format
    // param[out] string, string with time in formats like yyyy-mm-dd hh:mm:ss.mmm

    std::string getDateTimeFromEpoch(long long int epoch);
};

#endif
