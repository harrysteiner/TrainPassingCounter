/* 
 * File:   cCommonTools.h
 * Author: juia
 *
 * Created on 17. April 2016, 17:12
 */

#ifndef CCOMMONTOOLS_H
#define	CCOMMONTOOLS_H

#include <iostream>
#include <iomanip>
#include <functional>
#include <string>
#include <exception>
#include <numeric>
#include <set>
#include <locale.h>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/circular_buffer.hpp>


#define STREAMSTRING(STRMSG)        [&]()->std::string{using namespace std; stringstream oStr023974tcnw0re; oStr023974tcnw0re << STRMSG; return oStr023974tcnw0re.str();}()
#define STMSTR(STREAMMSG) [&]()->std::string{std::stringstream oStrStr_4AE80E30_FBCE_4FCC_A15C_D9F26679E350; oStrStr_4AE80E30_FBCE_4FCC_A15C_D9F26679E350 << STREAMMSG; return oStrStr_4AE80E30_FBCE_4FCC_A15C_D9F26679E350.str();}()
#define COUTSTRSTR(STRMSG)          std::cout << STREAMSTRING(STRMSG) << std::flush
#define EXCEPTSTR(STRMSG)           STREAMSTRING(__FUNCTION__ << " " << STRMSG)
#define FORMATINTEGER(WIDTH, FILLED)      std::setw(WIDTH) << std::setfill(FILLED)
#define FI(WIDTH, FILLED)           FORMATINTEGER(WIDTH, FILLED)
#define FORMATFLOAT(WIDTH, PRESICISION, FILLED)      std::fixed << std::setw(WIDTH) << std::setprecision(PRESICISION) << std::setfill(FILLED)
#define FFVALUE(VALUE) std::internal << std::setfill('0') << std::showpos << std::setw(9) << std::fixed << std::setprecision(5) << VALUE << std::noshowpos
#define FF(WIDTH, PRESICISION, FILLED)           FORMATFLOAT(WIDTH, PRESICISION, FILLED)
#define LOGSTR(STRFUNCTION, STRMSG) STREAMSTRING(STRFUNCTION << " " << STRMSG)
#define LOG_ERROR(STRMSG)           std::cout << "  ERROR: " << LOGSTR(__FUNCTION__, STRMSG) << std::endl
#define LOG_FATAL(STRMSG)           std::cout << "  FATAL: " << LOGSTR(__FUNCTION__, STRMSG) << std::endl
#define LOG_WARNING(STRMSG)         std::cout << "WARNING: " << LOGSTR(__FUNCTION__, STRMSG) << std::endl
#define LOG_DEBUG(STRMSG)           std::cout << "  DEBUG: " << LOGSTR(__FUNCTION__, STRMSG) << std::endl
#define LOG_TRACE(STRMSG)           std::cout << "  TRACE: " << LOGSTR(__FUNCTION__, STRMSG) << std::endl
#define LOG_INFO(STRMSG)            std::cout << "   INFO: " << LOGSTR(__FUNCTION__, STRMSG) << std::endl

template <class charT>
struct no_separator : public std::numpunct<charT>
{
protected:

  virtual std::string do_grouping() const
  {
    return "\000"; // groups of 0 (disable)
  }

  charT do_decimal_point() const
  {
    return ',';
  }
};

#define FORMATSTREAMFORCSV [](std::iostream& streamObject){ streamObject.imbue( std::locale(streamObject.getloc(),  new no_separator<char>()) ); }

namespace nTimeTypes
{
  typedef long long MilliSecs;
}

class cCommonTools
{
public:
  static int _kbhit();
  static void Sleep(nTimeTypes::MilliSecs p_oTimeMS);

  static bool Break(const char p_cKey);

  /** defines epoche start for all internal used timestamps
   * default EPOCHE_START_2014_1_1
   * @author wmk
   * @date 2016/03/23
   */
  const static boost::gregorian::date m_oEpocheStart;


  static nTimeTypes::MilliSecs GetCurrentTimeStamp();

  static void writeFile(const std::string& p_strFileName, const std::string& p_oBytes);

  const static double_t PI;
  const static double_t TODEG;
  const static double_t TORAD;

  static char ROTATECURSOR()
  {
    if (0 == g_RotationCounter)
    {
      g_RotationCounter = 1;
      return '|';
    }
    if (1 == g_RotationCounter)
    {
      g_RotationCounter = 2;
      return '/';
    }
    if (2 == g_RotationCounter)
    {
      g_RotationCounter = 3;
      return '-';
    }
    if (3 == g_RotationCounter)
    {
      g_RotationCounter = 0;
      return '\\';
    }
    return '-';
  }

private:
  static int g_RotationCounter;
};

struct AVGDEVCOUNTER2
{

  AVGDEVCOUNTER2()
  {
    Reset();
  };

  ~AVGDEVCOUNTER2()
  {
  };

  void SetValue(double p_dNewValue)
  {
    m_dSum += p_dNewValue;
    m_dSumSquare += (p_dNewValue * p_dNewValue);
    m_dN++;
  };

  double GetAvg()
  {
    return m_dSum / m_dN;
  };

  double GetStdDev()
  {
    return sqrt((m_dSumSquare / m_dN) - ((m_dSum * m_dSum) / (m_dN * m_dN)));
  };

  void Reset()
  {
    m_dN = 1.;
    m_dSum = 0.;
    m_dSumSquare = 0.;
  };

  double m_dN;
  double m_dSum;
  double m_dSumSquare;
};

/**
 * Uses a defined size
 */
template<typename ADT = double>
struct AVGDEVCOUNTER3
{

  AVGDEVCOUNTER3(unsigned int p_uiMaxSize)
  : m_oBufferCircular(p_uiMaxSize)
  , m_oBufferCircularSquare(p_uiMaxSize)
  {
    Reset();
  };

  ~AVGDEVCOUNTER3()
  {
  };

  void SetValue(ADT p_dNewValue)
  {
    m_oBufferCircular.push_back(p_dNewValue);
    m_oBufferCircularSquare.push_back(p_dNewValue * p_dNewValue);
  };

  ADT GetAvg()
  {
    ADT dSum = GetSum();
    ADT dN = GetN();
    return dSum / dN;
  };

  ADT GetStdDev()
  {
    ADT m_dSum = GetSum();
    ADT m_dSumSquare = GetSumSquare();
    ADT m_dN = GetN();
    return sqrt((m_dSumSquare / m_dN) - ((m_dSum * m_dSum) / (m_dN * m_dN)));
  };

  ADT GetSum()
  {
    return std::accumulate(m_oBufferCircular.begin(), m_oBufferCircular.end(), 0.);
  };

  ADT GetSumSquare()
  {
    return std::accumulate(m_oBufferCircularSquare.begin(), m_oBufferCircularSquare.end(), 0.);
  };

  void Reset()
  {
    m_oBufferCircular.clear();
    m_oBufferCircularSquare.clear();
  };

  ADT GetN()
  {
    return m_oBufferCircular.size();
  };

  bool Full()
  {
    return m_oBufferCircular.full();
  };

  boost::circular_buffer<ADT> m_oBufferCircular;
  boost::circular_buffer<ADT> m_oBufferCircularSquare;
};

/** Stores a new value and saves the last value; the difference is callable
 * @author wmk
 * @date 2008/12/10
 * @see http://www.tm-mathe.de/Themen/html/funnumdiff.html
 */
template<class ADT = double> //ADT --> arimtehic datatypes
struct FILTERSIMPLE
{

  FILTERSIMPLE(unsigned int sizeMax = 5)
  : m_sizeMax(sizeMax)
  , m_valuesSorted(m_sizeMax)
  , m_counter(0)
  {
    m_indexForMedianB = double(m_sizeMax) / 2. + 0.5;

    if (0 == m_sizeMax % 2)
    {
      m_indexForMedianA = m_indexForMedianB - 1;
    }
    else
    {
      m_indexForMedianA = m_indexForMedianB;
    }

    m_valuesSorted.resize(m_sizeMax, 0);
  };

  ~FILTERSIMPLE()
  {
  };

  FILTERSIMPLE<ADT>& SetValue(ADT p_valueNew)
  {
    //*** fill the whole buffer initially with the same value to make it working with the first call ***
    while (m_values.size() < m_sizeMax)
    {
      m_values.push_back(p_valueNew);
    }

    m_values[m_counter++ % m_sizeMax] = p_valueNew;

    return *this;
  };

  ADT GetAvg()
  {
    return  std::accumulate(m_values.begin(), m_values.end(), ADT(0)) / ADT(m_sizeMax);
  };

  ADT GetMedian()
  {
    std::memcpy(&m_valuesSorted[0], &m_values[0], sizeof (ADT) * m_values.size());

    std::sort(m_valuesSorted.begin(), m_valuesSorted.end());

    if (m_indexForMedianA == m_indexForMedianB)
      return m_valuesSorted[m_indexForMedianA];
    else
      return (m_valuesSorted[m_indexForMedianA] + m_valuesSorted[m_indexForMedianB]) / 2;
  };

private:
  std::vector<ADT> m_values;
  std::vector<ADT> m_valuesSorted;
  unsigned int m_sizeMax;
  unsigned int m_counter;
  unsigned int m_indexForMedianA;
  unsigned int m_indexForMedianB;
};

#endif	/* CCOMMONTOOLS_H */

