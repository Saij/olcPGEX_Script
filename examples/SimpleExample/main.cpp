#include "../olcPGE.h"

class SimpleExample : public olc::PixelGameEngine
{
public:
	SimpleExample()
	{
		sAppName = "SimpleExample";
	}

public:
	bool OnUserCreate() override
	{
		std::string example1 = "5 + 5 - 2";
		std::string example2 = "2 * 5 + 5";
		std::string example3 = "2 * (5 + 5)";
		std::string example4 = "10 / 2 + 5 * 20";

		olc::Script script;

		script.LoadScript(example1);
		script.LoadScript(example2);
		script.LoadScript(example3);
		script.LoadScript(example4);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		return true;
	}
};


int main()
{
	SimpleExample demo;
	if (demo.Construct(256, 240, 2, 2))
		demo.Start();

	return 0;
}