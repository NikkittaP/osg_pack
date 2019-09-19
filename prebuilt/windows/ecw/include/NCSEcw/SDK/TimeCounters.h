#ifndef TIME_COUNTERS_H_
#define TIME_COUNTERS_H_

#include "NCSTypes.h"
#include <map>
#include "NCSMutex.h"
#include "NCSThread.h"
#include <vector>


class TimeCounters
{
public:
	typedef struct
	{
		NCSTimeStampMs timeStampMs;
		UINT16 threadCount;
	}TimeCounterStampMs;

	typedef enum {
		ReadTime = 0,
		WriteTime = 1,
		ReassemblyTime = 2,
	}CompressionTimeCounterType;

	typedef std::map<CompressionTimeCounterType, TimeCounterStampMs> CompressionTimeCounters;

	TimeCounters();
	virtual ~TimeCounters();

	void IncrementThreadTimeCounters(CompressionTimeCounterType type, 
		const NCSTimeStampMs& timeLength);

	void Summarize();

	const CompressionTimeCounters& GetTotalTimeCounters() { return m_totalTimeCounters; };
private:
	CNCSMutex m_timeCountersMutex;
	NCSThreadLSKey m_compressionTimeCountersLSKey;
	std::vector<CompressionTimeCounters*> m_timeCounterVec;
	CompressionTimeCounters  m_totalTimeCounters;
};


#endif //TIME_COUNTERS_H_