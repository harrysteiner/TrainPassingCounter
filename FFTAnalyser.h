/* 
 * File:   FFTAnalyser.h
 * Author: wmk
 *
 * Created on 23. Januar 2017, 16:52
 */

#pragma once

#include "TimeServer.h"
#include "TimeServerClock.h"
#include "FFTAnalyserResultHandler.h"
#include <utility>

class FFTAnalyser
{
public:
  FFTAnalyser();
  FFTAnalyser(FFTAnalyserResultHandlerPtr handlerResults);

  virtual ~FFTAnalyser();

  bool add(double p_sample, helper::TimeServerUnix::ms&& p_timestamp);
  bool add(double p_sample);

  void processSamples();
  void processSamples(helper::TimeServerUnix::secAsDouble samplingDuration);

  helper::TimeServerUnix::secAsDouble getDurationOverallSamples();
  
  double getLastSamplingRate();

private:
  FFTAnalyserResultHandlerPtr m_handlerResults;

  const uint32_t m_maxSampels;

  struct sample
  {
    double m_sample;
    helper::TimeServerUnix::ms m_timestamp;
  };

  boost::circular_buffer<sample> m_samples;

  /**
   * extracts a simple double vector from samples
   */
  std::vector<double> getSamples();

  /**
   * calcs a index for the amplitude spectrum
   */
  std::vector<double> getFrequencyIndex(double N, double frequencyResolution);
  
  double m_samplingRate;
};