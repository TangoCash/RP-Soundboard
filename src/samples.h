#ifndef samples_H__
#define samples_H__

#include "SampleBuffer.h"
#include "SampleProducerThread.h"

#include <mutex>

class InputFile;

class Sampler
{
public:
	Sampler();
	~Sampler();
	void init();
	void shutdown();
	int fetchInputSamples(short *samples, int count, int channels, bool *finished);
	int fetchOutputSamples(short *samples, int count, int channels, const unsigned int *channelSpeakerArray, unsigned int *channelFillMask);
	void playFile(const char *filename);

private:
	int fetchSamples(SampleBuffer &sb, short *samples, int count, int channels, bool eraseConsumed, int ciLeft, int ciRight, bool overLeft, bool overRight);
	int findChannelId(int channel, const unsigned int *channelSpeakerArray, int count);

private:
	class OnBufferProduceCB : public SampleBuffer::ProduceCallback
	{
	public:
		OnBufferProduceCB(Sampler &parent) :
			parent(parent)
		{}
		void onProduceSamples(const short *samples, int count, SampleBuffer *caller);
	private:
		Sampler &parent;
	};
	
private:
	SampleBuffer m_sbCapture;
	SampleBuffer m_sbPlayback;
	SampleProducerThread m_sampleProducerThread;
	OnBufferProduceCB m_onBufferProduceCB;

	bool m_playing;
	InputFile *m_inputFile;
	
	std::mutex m_mutex;
};


#endif // samples_H__
