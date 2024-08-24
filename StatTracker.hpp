#pragma once
#include <iostream>
#include <vector>
#include <GLFW/glfw3.h>
#include <thread>

class StatTracker;
inline StatTracker* g_stats = nullptr;

class StatTracker
{

public:

	StatTracker()
	{
		g_stats = this;

		frames = 0;

		minFps = std::numeric_limits<float>::max();
		maxFps = std::numeric_limits<float>::min();

		std::thread t(&StatTracker::addFramesToHistory, this);
		t.detach();
	}

	~StatTracker()
	{
		g_stats = nullptr;
	}

	void countFrame()
	{
		frames++;
	}


	float minFps, maxFps;
	std::vector<float> fpsHistory;

private:

	void addFramesToHistory()
	{
		while (true)
		{
			if (frames < minFps)
				minFps = frames;
			if (frames > maxFps)
				maxFps = frames;

			fpsHistory.push_back(frames);
			frames = 0;

			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}

	int frames;
};