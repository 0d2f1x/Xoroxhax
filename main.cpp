#include "vars.h"
#include "stream.h"
//#include "hex.h"

//Tools::Junk junk;

int main()
{
	Tools::checkRun();
	//GetPlayerName();
	init();
	string turn;
	cout << "Podrubit' servak? (y/n) ";
	std::cin >> turn;

	if (turn == "y")
	{
		thread trigger(Hex::trigger);
		WebSocket();
	}
	else
	{
		thread headWalker(Hex::headWalker);
		thread trigger(Hex::trigger);
		while (true)
		{
			Hex::rad(),
			Hex::flash(),
			Sleep(10);
		} 
	}
	//junk.screenSaver();
}