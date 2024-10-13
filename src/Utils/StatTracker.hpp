#pragma once
#include <iostream>
#include <vector>
#include <thread>

class StatTracker;
inline StatTracker* g_stats = nullptr;

class StatTracker
{

public:

	StatTracker()
	{
		g_stats = this;

		fpsHistory.push_back(0);
		frameTimeHistory.push_back(0.f);
		cpuTimeHistory.push_back(0.f);
		gpuTimeHistory.push_back(0.f);

		frames = 0;
		maxHistory = 100;

		shouldMeasureTime = false;

		minFps = std::numeric_limits<uint32_t>::max();
		maxFps = std::numeric_limits<uint32_t>::min();

		std::thread t(&StatTracker::update, this);
		t.detach();
	}

	~StatTracker()
	{
		g_stats = nullptr;
	}

	inline void countFrame()
	{
		frames++;
	}

	template <typename Func>
	void measureTime(Func func)
	{
		if (shouldMeasureTime)
		{
			const auto start = std::chrono::high_resolution_clock::now();

			GLuint query;
			glGenQueries(1, &query);
			glBeginQuery(GL_TIME_ELAPSED, query);

			func();

			glEndQuery(GL_TIME_ELAPSED);

			// total time
			const auto end = std::chrono::high_resolution_clock::now();
			const std::chrono::duration<float> duration = end - start;
			const float cpuTime = duration.count();

			glFinish();

			// gpu time
			GLuint64 queryResult;
			glGetQueryObjectui64v(query, GL_QUERY_RESULT, &queryResult);
			const float gpuTime = static_cast<float>(queryResult / 1e9);

			addCpuTimeToHistory(cpuTime * 1000.f);
			addGpuTimeToHistory(gpuTime * 1000.f);

			glDeleteQueries(1, &query);

			shouldMeasureTime = false;
		}
		else
		{
			func();
		}
	}


	uint32_t minFps, maxFps;
	std::vector<uint32_t> fpsHistory;
	std::vector<float> frameTimeHistory;
	std::vector<float> cpuTimeHistory;
	std::vector<float> gpuTimeHistory;

	int maxHistory;

private:

	void addCpuTimeToHistory(float value)
	{
		if (cpuTimeHistory.size() >= maxHistory)
		{
			cpuTimeHistory.erase(cpuTimeHistory.begin());
		}

		cpuTimeHistory.push_back(value);
	}

	void addGpuTimeToHistory(float value)
	{
		if (gpuTimeHistory.size() >= maxHistory)
		{
			gpuTimeHistory.erase(gpuTimeHistory.begin());
		}

		gpuTimeHistory.push_back(value);
	}

	void addFramesToHistory()
	{
		if (frames < minFps)
			minFps = frames;
		if (frames > maxFps)
			maxFps = frames;

		if (fpsHistory.size() >= maxHistory)
		{
			fpsHistory.erase(fpsHistory.begin());
			frameTimeHistory.erase(frameTimeHistory.begin());
		}

		fpsHistory.push_back(frames);
		frameTimeHistory.push_back(1000.f / frames);
		frames = 0;
	}

	void update()
	{
		while (true)
		{
			addFramesToHistory();
			shouldMeasureTime = true;

			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}

	uint32_t frames;

	bool shouldMeasureTime;
};