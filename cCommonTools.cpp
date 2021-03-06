/* 
 * File:   cCommonTools.cpp
 * Author: juia
 * 
 * Created on 17. April 2016, 17:12
 */

#include "cCommonTools.h"
#include <stdio.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/filesystem.hpp>

#include <stdio.h>
#include <sys/select.h>
#include <termios.h>
#include <stropts.h>
#include <unistd.h>

using namespace boost::posix_time;
using namespace boost::local_time;
using namespace boost::gregorian;


const boost::gregorian::date cCommonTools::m_oEpocheStart = boost::gregorian::date(2017,1,1);
const double_t cCommonTools::PI = 3.14159265359;
const double_t cCommonTools::TORAD = cCommonTools::PI / 180.;
const double_t cCommonTools::TODEG = 180. / cCommonTools::PI;
int cCommonTools::g_RotationCounter = 0;


int cCommonTools::_kbhit()
{
    static const int STDIN = 0;
    static bool initialized = false;

    if (!initialized) {
        // Use termios to turn off line buffering
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    timeval timeout;
    fd_set rdset;

    FD_ZERO(&rdset);
    FD_SET(STDIN, &rdset);
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    return select(STDIN + 1, &rdset, NULL, NULL, &timeout);
}

void cCommonTools::Sleep(nTimeTypes::MilliSecs p_oTimeMS)
{
  if (p_oTimeMS == 0.)
  {
    return;
  }

  boost::this_thread::sleep(boost::posix_time::milliseconds(p_oTimeMS));
}

bool cCommonTools::Break(const char p_cKey) {
    
}

nTimeTypes::MilliSecs cCommonTools::GetCurrentTimeStamp()
{
  local_date_time oNow = local_microsec_clock::local_time(time_zone_ptr()); // now with sub seconds resolution
  ptime oTimeBase(m_oEpocheStart);
  time_duration oDiff = oNow.utc_time() - oTimeBase;
  return oDiff.total_milliseconds();

}

void cCommonTools::writeFile( const std::string& p_strFileName, const std::string& p_oBytes )
{
  std::ofstream out(p_strFileName, std::ios::out | std::ios::binary);
  out << p_oBytes;
  out.close();
}