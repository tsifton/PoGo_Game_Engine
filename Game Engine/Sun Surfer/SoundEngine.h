#ifndef SOUNDENGINE_H_
#define SOUNDENGINE_H_

#pragma warning(push)
#pragma warning(disable: 4458)
#include "yse.hpp"
#pragma warning(pop)

#include "ConfigReader.h"

class SoundEngine
{
private:
	SoundEngine() {};
	~SoundEngine() {};

public:
	static bool Initialize(ConfigReader* cfg);
	static bool Shutdown();
	static void Update();
	
private:
	static float occlusionCallback(const YSE::Vec& source, const YSE::Vec& listener);

private:
	static ConfigReader* m_cfg;
};
#endif // !SOUNDENGINE_H_
