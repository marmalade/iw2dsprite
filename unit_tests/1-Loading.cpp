#include "TestNumber.h"
#include "../Iw2DSprite.h"

int main(int argc, char* argv[])
{	
	//Init Airplay Systems
	Iw2DInit();
	IwResManagerInit();		

	IwGetResManager()->LoadGroup("sprites.group");

	CIwResGroup*	group = IwGetResManager()->GetGroupNamed("sprites");
	
	Iw2DSprite* s = new Iw2DSprite(group,"1");
	
	if(s->IsBuilt())
		cout << "Woohooo!" << endl;
	
	delete s;
	// Terminate Airplay Systems		
	IwResManagerTerminate();		
	Iw2DTerminate();

	return 0;
}