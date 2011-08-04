#include <tut.hpp> 

#include <Iw2DSprite.h>

#define MS_PER_FRAME (1000 / 30)

namespace tut 
{ 
	struct data
	{ 
		Iw2DSprite* s, *s1, *s2;
		CIwResGroup* g;

		data()
		{
			IwGetResManager()->LoadGroup("sprites.group");
			g = IwGetResManager()->GetGroupNamed("sprites");
			s = new Iw2DSprite(g,"1");
		}

		~data()
		{
			delete s;
			IwGetResManager()->DestroyGroup("sprites");			
		}
	};
	
	typedef	test_group<data> tg;
	tg test_group("Sprite Tests");
	
	typedef tg::object testobject;

	template<> 
	template<> 
	void testobject::test<1>() 
	{ 
		set_test_name("Load the sprite");
		ensure_equals("Build",s->IsBuilt(),true);
	}	

	template<> 
	template<> 
	void testobject::test<2>() 
	{ 
		set_test_name("Draw the Sprite");
		s->Render();
		Iw2DSurfaceShow();
		s3eDeviceYield(1000);
	}	

	template<> 
	template<> 
	void testobject::test<3>() 
	{ 
		set_test_name("Draw the Sprite");
		int j=0;

		while(j<100)
		{
			int64 start = s3eTimerGetMs();
			Iw2DSurfaceClear(0xff000000); //paint the screen black
			s->Render(CIwSVec2(0,10));
			s->Step();
			s3eDebugPrintf(0,100,true,"`xFFFFFFFrame Number: %d\nFrame Offset: %d, %d",s->GetCurrentFrame(),s->GetFrameOffset());
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
	}	

	template<> 
	template<> 
	void testobject::test<4>() 
	{ 
		set_test_name("Copy the Sprite");

		s1 = new Iw2DSprite(g,"2");
	
		ensure_equals("Both sprites are built", (s1->IsBuilt() && s->IsBuilt()), true);
		
		int j=0;
	//first render of the original 2 sprites
		Iw2DSurfaceClear(0xff000000);
		s->Render(CIwSVec2(0,s->GetFrameSize().y));
		s1->Render(CIwSVec2(j,200));
		Iw2DSurfaceShow();
		s3eDeviceYield(1000);
	//Create a new one with the copy constructor
		s2 = new Iw2DSprite((*s1));
	//test for self-assignment
		*s1 = *s1;
		ensure_equals("S1 equals S1",s1==s1,true);
	//copy the first one into the second
		*s1 = *s;
	//show them all!
		while(j<100)
		{
			int64 start = s3eTimerGetMs();

			Iw2DSurfaceClear(0xff000000); //paint the screen black

			s->Render(CIwSVec2(0,s->GetFrameSize().y));
			s->Step();
			s2->Render(CIwSVec2(200,j));
			s2->Step();
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
		delete s1,s2;
	}	

	template<> 
	template<> 
	void testobject::test<5>() 
	{ 
		set_test_name("Rotate the Sprite");
		int j = 0;
		s->Rotate(j);
		ensure_equals("Angle",s->GetAngle(),0);
		while(j<360)
		{
			int64 start = s3eTimerGetMs();
			Iw2DSurfaceClear(0xff000000); //paint the screen black
			s->Render(CIwSVec2(200,200));
			s->Step();
			s->Rotate(j*10);
			s3eDebugPrintf(0,100,true,"`xFFFFFFFrame Number: %d\nFrame Offset: %d, %d",s->GetCurrentFrame(),s->GetFrameOffset());
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
	}	

	template<> 
	template<> 
	void testobject::test<6>() 
	{ 
		set_test_name("Flip the Sprite");
		int j = 0;
		s->Flip();
		ensure_equals("Flip (0,0)", s->IsFlipped().x , 0);
		ensure_equals("Flip (0,0)", s->IsFlipped().y , 0);
		while(j<200)
		{
			int64 start = s3eTimerGetMs();
			Iw2DSurfaceClear(0xff000000); //paint the screen black
			s->Render(CIwSVec2(200,200));
			s->Step();
			if(j == 50 )
			{
				s->Flip( CIwSVec2(1,0) );
				ensure_equals("Flip (1,0)", s->IsFlipped().x , 1);
			}
			else if(j == 100)
			{
				s->Flip( CIwSVec2(0,1) );
				ensure_equals("Flip (0,1)", s->IsFlipped().y , 1);
			}
			else if(j == 150)
			{
				s->Flip( CIwSVec2(1,1) );
				ensure_equals("Flip (1,1)", s->IsFlipped().x , 1);
				ensure_equals("Flip (1,1)", s->IsFlipped().y , 1);
			}

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
	}	
}; 