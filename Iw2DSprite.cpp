#include "Iw2DSprite.h"
/***********************************************************************************

	class			Iw2DSprite
	description		Animation class that features non-complete squared sprite sheets
					support. Includes basics functions.

	date			11/07/2011
	version			0.1
	notes			for a day-by-day status check Changes.txt
					
	author			Leandro 'The Phantom Lord' Ostera
					leostera at gmail dot com

************************************************************************************/

#include <s3eConfig.h>

Iw2DSprite&	Iw2DSprite::operator = (Iw2DSprite const& pSprite)
{
	if(this != &pSprite)
	{
		if(pSprite.IsBuilt())
		{
			if(m_Built)
				delete m_SpriteSheet;
			m_Built = m_Pause = m_Stop = false;
			m_Offset = m_CurrentFrame = m_DrawRect = CIwSVec2::g_Zero;			
			m_FrameCounter = 0;

			LoadFromResource(pSprite.GetResourceGroup(),pSprite.GetResourceName(),false);
			
			m_MaxFrames = pSprite.GetMaxFrames();
			m_Offset = pSprite.GetFrameOffset();
			m_FrameSize = pSprite.GetFrameSize();
			m_Loop = pSprite.IsLooping();
		}		
	}
	return (*this);
}

/*
 *	method		Constructor	 
 *
 *	parameters	CIwResGroup*	pGroup		The resource group in where to search for the resouce name pName.
 *				std::string		pName		The name of the resource to load.
 *				bool			pLoop		True if the animation should loop. False otherwIse.
 *
 *	description		ThIs Is the main constructor. It initializes the following properties:
						*	bool		m_Built			false
						*	CIwSVec2	m_Offset		CIwSVec2::g_Zero (zero vector)						
						*	CIwSVec2	m_CurrentFrame	CIwSVec2::g_Zero (zero vector)
						*	short		m_FrameCounter	0 (ZERO, integer value)
						*	CIwSVec2	m_DrawRect		CIwSVec2::g_Zero (zero vector)
						*	bool		m_Pause			false
						*	bool		m_Stop			false
						*	bool		m_Loop			pLoop

 */
Iw2DSprite::Iw2DSprite(CIwResGroup* pGroup, std::string pName, bool pLoop)
	: m_Built(false), m_Offset(CIwSVec2::g_Zero), m_CurrentFrame(CIwSVec2::g_Zero), m_FrameCounter(0), m_DrawRect(CIwSVec2::g_Zero),m_Pause(false),m_Stop(false),m_Loop(pLoop)
{
	LoadFromResource(pGroup,pName);
}

/*
 *	method		Deconstructor	 
 *
 *	description		If the class has been succesfully built before, clean the resource from memory.
 */
Iw2DSprite::~Iw2DSprite()
{
	UnLoad();
}

/*
 *	method		Copy constructor
 *
 *	parameters	Iw2DSprite const&	pSprite		A constant reference of a Iw2DSprite object.
 *
 *	description		Copies the propierties of the parameter object into the object that Is being constructed.
 *					It uses the overloaded operator = to such ends. Current object flag m_Built will be set along
 *					with flag m_Copy.
 */
Iw2DSprite::Iw2DSprite(Iw2DSprite const& pSprite)
: m_Built(false), m_Offset(CIwSVec2::g_Zero), m_CurrentFrame(CIwSVec2::g_Zero), m_FrameCounter(0), m_DrawRect(CIwSVec2::g_Zero),m_Pause(false),m_Stop(false),m_MaxFrames(pSprite.GetMaxFrames()), m_FrameSize(pSprite.GetFrameSize()),m_Loop(pSprite.IsLooping())
{
	if(pSprite.IsBuilt())
	{
		if(m_Built)
			delete m_SpriteSheet;
		LoadFromResource(pSprite.GetResourceGroup(),pSprite.GetResourceName(),false);		
	}			
}

/*
 *	method		LoadFromResource
 *
 *	returns		bool		True if it was succesfully built. False otherwIse.
 *
 *	parameters	CIwResGroup*	pGroup		Resource group in where to load resource named pName
 *				std::string		pName		Name of the resource to be loaded
 *
 *	description		If current resource name differs from pName, current resource group Is not pGroup,
 *					the object has not yet been built or both the resource name and resource group has
 *					not been set previously, set pGroup as current resource group, pName as current
 *					resource name and try to load the resource.
 *					If anything but error, the image should have been succesfully loaded into memory, and all
 *					image-dependant properties of the class properly initialized.
 */
bool Iw2DSprite::LoadFromResource(CIwResGroup* pGroup, std::string pName, bool pDataFromFile)
{
	bool sameName = ( pName.compare(m_ResourceName) > 0 ) ? true : false;
	bool emptyName = m_ResourceName.empty();
	if( !sameName || m_ResourceGroup != pGroup || !m_Built || ( !emptyName && !m_ResourceGroup) )
	{
		m_ResourceGroup = pGroup;
		m_ResourceName = pName;
		bool f_Built = false;
		
		IwGetResManager()->SetCurrentGroup(m_ResourceGroup);
		
		if( m_ResourceGroup->GetResNamed(m_ResourceName.c_str(),IW_GX_RESTYPE_TEXTURE) )
		{				
			f_Built = true;

			UnLoad();

			if(pDataFromFile)
			{
				std::string w,h,f;
				w = m_ResourceName + "_w" ;
				h = m_ResourceName + "_h" ;
				f = m_ResourceName + "_frames" ;
	
				int temp;
				s3eConfigGetInt("Sprites",w.c_str(),&temp);
				m_FrameSize.x = temp;
				s3eConfigGetInt("Sprites",h.c_str(),&temp);
				m_FrameSize.y = temp;
				s3eConfigGetInt("Sprites",f.c_str(),&temp);	
				m_MaxFrames = temp;
			}			

			m_SpriteSheet = Iw2DCreateImageResource(m_ResourceName.c_str()); 

			m_Frames.x = m_SpriteSheet->GetWidth() / m_FrameSize.x;
			m_Frames.y = m_SpriteSheet->GetHeight() / m_FrameSize.y;	
		}		
		m_Built = f_Built;
	}

	return m_Built;
}

/*
 *	method		UnLoad
 *
 *	returns		bool	true if sprite was succesfully destroyed, false otherwise.
 *
 *	description			if the sprite was built, delete the spritesheet and set built flag to false.
 */
bool Iw2DSprite::UnLoad()
{
	if(m_Built)
	{
		delete m_SpriteSheet;
		m_Built = false;
	}
	
	return !m_Built;
}

/*
 *	method		Render	 
 *
 *	returns		void
 *
 *	parameters	CIwSVec2	pPosition		(x,y) 2D integer vector of the position of the top left corner of the sprite on the screen.
 *
 *	description				Draws a region of the image starting at m_DrawRect and finIshing at m_DrawRect+m_FrameSize,
 *							at the coordinates passed by pPosition.
 */
void Iw2DSprite::Render(CIwSVec2 pPosition)
{
	Iw2DDrawImageRegion(m_SpriteSheet,
						pPosition,
						m_FrameSize,
						m_DrawRect,
						m_FrameSize);
}

/*
 *	method		Step	 
 *
 *	returns		bool	True if the animation has not yet finIshed. False if it has.
 *
 *	description		Iterates through the sprite sheet as if it was a matrix by incrementing m_CurrentFrame.x
 *					(if not paused nor stopped) untill it Is equal or greater than (note that it will never
 *					be greater -or at least shouldn't!) then it goes back to zero and m_CurrentFrame.y Is incremented.
 *					m_CurrentFrame(x,y) works as indexes for the sprites in the sheet: x Is for the column, y for the
 *					rows.
 *					Additionally, in case that the sprites has / uses less than x*y frames in the sheet, there Is max
 *					frames number and a frame counter used to check for it.
 *					Later, the drawing rect Is updated
 */
bool Iw2DSprite::Step()
{
	if(!m_Pause && !m_Stop)
	{	
		++m_CurrentFrame.x;
		if( m_CurrentFrame.x >= m_Frames.x )
		{
			m_CurrentFrame.x = 0;
			++m_CurrentFrame.y;

			if ( m_CurrentFrame.y >= m_Frames.y )
				m_CurrentFrame.y = 0;					
		}	
		
		++m_FrameCounter;
		if( m_FrameCounter >= m_MaxFrames )
		{
			m_Stop = (m_Loop) ? false : true;
			m_CurrentFrame = CIwSVec2::g_Zero;
			m_FrameCounter = 0;
		}

		m_DrawRect.x = m_FrameSize.x * m_CurrentFrame.x;
		m_DrawRect.y = m_FrameSize.y * m_CurrentFrame.y;

	}

	return !m_Stop;
}

/*
 *	method		Play	 
 *
 *	returns		void
 *
 *	description		Unstops and unpauses the animation.		
 */
void Iw2DSprite::Play(){
	m_Pause = m_Stop = false;
}

/*
 *	method		IsPaused	 
 *
 *	returns		void 
 *
 *	description		If the animation Is paused, unpause it!
 */
void Iw2DSprite::Resume(){
	if(m_Pause)
		m_Pause = false;
}

/*
 *	method		Pause	 
 *
 *	returns		void
 *
 *	description		Sets the m_Pause boolean flag to true.
 */
void Iw2DSprite::Pause(){
		m_Pause = true;
}

/*
 *	method		Stop	 
 *
 *	returns		void
 *
 *	description		Sets the m_Stop boolean flag to true and resets the current frame to (0,0).
 */
void Iw2DSprite::Stop(){
	m_Stop = true;
	m_CurrentFrame = CIwSVec2::g_Zero;
}

/*
 *	method		goToFrame	 
 *
 *	returns		void
 *
 *	parameters	CIwSVec2	pFrame	(x,y) 2D integer vector for current column (x) and row (y) of the frame in the sheet.
 *
 *	description		Sets the current frame to be the one located in the x column and y row of the sprite sheet.
 */
void Iw2DSprite::GoToFrame(CIwSVec2 pFrame){
	if(pFrame.x >= 0 && pFrame.x <= m_Frames.x)
		if(pFrame.y >= 0 && pFrame.y <= m_Frames.y)
			m_CurrentFrame = pFrame;
}