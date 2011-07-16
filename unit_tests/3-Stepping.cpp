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
	Iw2DSprite* s1 = new Iw2DSprite(group,"2");
	
	if(s->IsBuilt() && s1->IsBuilt())
		cout << "Woohooo!" << endl;
	
	int j=0;

	while(j<300)
	{
		int64 start = s3eTimerGetMs();

		Iw2DSurfaceClear(0xff000000); //paint the screen black

		s->Render(CIwSVec2(0,s->GetFrameSize().y));
		s->Step();
		s1->Render(CIwSVec2(j,200));
		s1->Step();

		Iw2DSurfaceShow();

		
		while (s3eTimerGetMs()- start< MS_PER_FRAME)
		{
			int32 yield = (int32) (MS_PER_FRAME - (s3eTimerGetMs()-start));
			if (yield<0)
				break;
			s3eDeviceYield(yield);
		}
		
		++j;
	}
	
	delete s;
	delete s1;
	// Terminate Airplay Systems		
	IwResManagerTerminate();		
	Iw2DTerminate();

	return 0;
}