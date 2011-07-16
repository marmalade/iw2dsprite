/***********************************************************************************

	class			Iw2DSprite
	description		Animation class that features non-complete squared sprite sheets
					support. Includes basics functions.

	date			11/07/2011
	version			0.1
	notes			For a day-by-day status check Changes.txt
					Remember that destroying a sprite does not destroy the resource group
					or frees the memory of the resource used within the resource manager!
					Those steps should be performed by you.
					
	author			Leandro 'The Phantom Lord' / 'Grün' Ostera
					leostera at gmail dot com

************************************************************************************/

#pragma once // might be more suitable to use define checks, but pragma once seems to work fine on almost every compiler

#include <IwResManagerClass.h>
#include <IwResGroup.h>
#include <Iw2D.h>
#include <string>

class Iw2DSprite
{
private:
	/*
	 *	m_Built			bool flag to determine whether the object is already built or not
	 */
	bool	m_Built;

	/*
	 *	m_SpriteSheet	is the pointer to CIw2DImage that actually holds the sprite sheet image.
	 *					It uses the Marmalade SDK resource manager to load it
	 */
	CIw2DImage*		m_SpriteSheet;

	/*
	 *	m_ResourceName	name of the resource loaded into m_SpriteSheet;
	 */
	CIwResGroup*	m_ResourceGroup;

	/*
	 *	m_ResourceGroup	name of the group where the resource named m_ResourceName is located
	 */
	std::string		m_ResourceName;

	/*
	 *	m_DrawRect		(x,y) position of the currently to be drawn frame
	 */
	CIwSVec2	m_DrawRect;

	/*
	 *	m_FrameSize		the actual size of a frame. .x for width, .y for height
	 */
	CIwSVec2	m_FrameSize;

	/*
	 *	m_Frames		the amount of rows (.x) and columns (.y) the animation has
	 */
	CIwSVec2	m_Frames;

	/*
	 *	m_Offset		the horizontal and vertical pixel offset of each frame
	 */
	CIwSVec2	m_Offset;

	/*
	 *	m_CurrentFrame	in which row (.x) and column (.y) are we now
	 */	
	CIwSVec2	m_CurrentFrame;

	
	/*
	 *	m_MaxFrames		the max amount of frames that actually have something.
	 *					mostly used in square sprite sheets where some frames are blank.
	 */
	int16		m_MaxFrames;

	/*
	 *	m_FrameCounter	internal counter to iterate through the frame "matrix".
	 */
	int16		m_FrameCounter;

	/*
	 *	m_EndFrame		sets which frame ends the animation.
	 */	
	int16		m_EndFrame;

	
	/*
	 *	m_Loop			boolean flag to determine whether to loop or not once reached the final frame
	 */
	bool		m_Loop;

	/*
	 *	m_Stop			boolean flag that indicates if the animation has been stopped and therefore return to the first frame
	 */
	bool		m_Stop;

	/*
	 *	m_Pause			boolean flag telling us if we shall continue with the animation or freeze it
	 */	
	bool		m_Pause;

	/*
	 *	Private class methods: do not attempt to use in your own code!
	 */

	/*
	 *	method		LoadFromResourceName	
	 *
	 *	parameters	pGroup			the resource group inwhere to look for the resource
	 *				pName			the name of the resource to look up in the group
	 *				pDataFromFile	wheter or not to read width, height and max frames from file
	 *
	 *	returns		bool	true if sprite was succesfully rebuilt, false otherwise.
	 *
	 *	description			loads a resource named name from the resource group that group points to
	 *						if the current group is different from the one specified, the new group
	 *						is set to be the current.
	 *						all the other parameters are initialized from configuration file
	 *						if the resource name and group equals the current resource name and group
	 *						then nothing is done.
	 */
	bool LoadFromResource(CIwResGroup* pGroup, std::string pName, bool pDataFromFile = true);

	/*
	 *	method		UnLoad
	 *
	 *	returns		bool	true if sprite was succesfully destroyed, false otherwise.
	 *
	 *	description			cleans up the object.
	 */
	bool UnLoad();
	

public:
	/*
	 *	method		Parameterized constructor
	 *
	 *	parameters	pGroup		the resource group inwhere to look for the resource
	 *				pName		the name of the resource to look up in the group
	 *				pLoop		whether the animation sohuld loop (true) or not (false)
	 *
	 *	description			initializes the class.
	 */
	Iw2DSprite(CIwResGroup* pGroup, std::string pName, bool pLoop = true);

	/*
	 *	method		LoadFromResourceName	
	 *
	 *	description			performs clean up of the asigned memory blocks.
	 */
	~Iw2DSprite();

	/*
	 *	method		Copy constructor
	 *
	 *	parameters	pSprite	the sprite object to be copied
	 *
	 *	description			copies all data from the sprite parameter object, incluiding the pointer to the spritesheet
	 */
	Iw2DSprite(Iw2DSprite const& pSprite);

	/*
	 *	operator	copy assign
	 *
	 *	parameters	pSprite	the sprite object to be assigned (copied)
	 *
	 *	description			copies all data from the sprite parameter object, incluiding the pointer to the spritesheet
	 */
	Iw2DSprite&	operator= (Iw2DSprite const& pSprite);
	
	/*
	 *	method		Step
	 *
	 *	returns		bool	true if animation is within range, false if animation is about to restart/finish
	 *						depending on looping
	 *
	 *	description			iterates through the frames of the sprite and updates the drawing rect
	 */
	bool		Step();

	/*
	 *	method		Loop
	 *
	 *	parameters	pLoop	true for looping, false otherwise
	 *
	 *	description			set loop on or off
	 */
	inline void	Loop(bool pLoop) { m_Loop = pLoop; }

	/*
	 *	method		toggleLoop
	 *
	 *	description			switches looping on/off
	 */
	inline void	ToggleLoop() { m_Loop = (m_Loop)?false:true; }

	/*
	 *	method		Stop
	 *
	 *	description			stops the animation and returns to first frame
	 */
	void		Stop(); 

	/*
	 *	method		Play
	 *
	 *	description			starts the animation from the current frame
	 */
	void		Play();

	/*
	 *	method		Resume
	 *
	 *	description			if animation was paused, it resumes playing from the current frame
	 */
	void		Resume();

	/*
	 *	method		Pause
	 *
	 *	description			stops the animation at the current frame
	 */
	void		Pause();
	
	/*
	 *	method		goToFrame
	 *
	 *	parameters	pFrame	CIwSVec2 2D integer vector indicating row and column of the frame to set as current
	 *
	 *	description			if not specified a vector, returns to first frame, otherwise sets f as current frame
	 */
	void		GoToFrame(CIwSVec2 pFrame = CIwSVec2::g_Zero);

	/*
	 *	method		Restart
	 *
	 *	description			returns to the first frame
	 */
	void		Restart() { GoToFrame(); }


	/*
	 *	method		isPaused
	 *
	 *	returns		bool
	 *
	 *	description			true if it is paused, false otherwise
	 */
	inline bool		IsPaused()			const { return m_Pause; }

	/*
	 *	method		isStopped
	 *
	 *	returns		bool
	 *
	 *	description			true if it is stopped, false otherwise
	 */
	inline bool		IsStopped()			const { return m_Stop; }	

	/*
	 *	method		isLooping
	 *
	 *	returns		bool
	 *
	 *	description			true if it is looping, false otherwise
	 */
	inline bool		IsLooping()			const { return m_Loop; }	

	/*
	 *	method		isPlaying
	 *
	 *	returns		bool
	 *
	 *	description			true if it is not paused and not stopped, false otherwise
	 */
	inline bool		IsPlaying()			const { return (!m_Stop&&!m_Pause)?true:false; }

	/*
	 *	method		IsBuilt
	 *
	 *	returns		bool	
	 *
	 *	description			true if the sprite has been built. false otherwise
	 */
	inline bool		IsBuilt()			const { return m_Built; }

	/*
	 *	method		getWidth
	 *
	 *	returns		int16	
	 *
	 *	description			returns current spritesheet width as integer
	 */
	int16		GetWidth()			const { return m_SpriteSheet->GetWidth(); }
	
	/*
	 *	method		getHeight	 
	 *
	 *	returns		int16
	 *
	 *	description			returns current spritesheet height as integer
	 */
	int16		GetHeigth()			const { return m_SpriteSheet->GetHeight(); }

	/*
	 *	method		getMaxFrames	 
	 *
	 *	returns		int16
	 *
	 *	description			returns the maximum amount of useful frames in the spritesheet
	 *						that is, ignoring blank frames at the end of the sheet, as integer
	 */
	int16		GetMaxFrames()		const { return m_MaxFrames; }

	/*
	 *	method		getEndFrame	 
	 *
	 *	returns		int16
	 *
	 *	description			returns the last frame number as integer
	 */
	int16		GetEndFrame()		const { return m_EndFrame; }
	
	/*
	 *	method		getCurrentFrame	 
	 *
	 *	returns		CIwSVec2
	 *
	 *	description			returns current frame row and column as a 2D integer vector
	 */
	CIwSVec2		GetCurrentFrame()	const { return m_CurrentFrame; }

	/*
	 *	method		getFrameSize	 
	 *
	 *	returns		CIwSVec2
	 *
	 *	description			returns frame size as 2D integer vector
	 */
	CIwSVec2		GetFrameSize()		const { return m_FrameSize; }

	/*
	 *	method		getFrameOffset	 
	 *
	 *	returns		CIwSVec2
	 *
	 *	description			returns x and y offset of each frame as 2D integer vector
	 */
	CIwSVec2		GetFrameOffset()	const { return m_Offset; }

	/*
	 *	method		getResourceName
	 *
	 *	returns		std::string
	 *
	 *	description			returns a copy of the resource name as a std::string
	 */
	std::string		GetResourceName()	const { return m_ResourceName; }

	/*
	 *	method		getResourceGroup	 
	 *
	 *	returns		CIwResGroup*
	 *
	 *	description			returns a pointer to the current resource group
	 */
	CIwResGroup*	GetResourceGroup()	const { return m_ResourceGroup; }

	/*
	 *	method		getCopy	 
	 *
	 *	returns		const Iw2DSprite*
	 *
	 *	description			returns a pointer to a const Iw2DSprite (which is this)
	 */
	const Iw2DSprite*	GetCopy()		const { return this; }

	/*
	 *	method		Render
	 *
	 *	returns		void
	 *
	 *	parameters	pPosition	(x,y) 2D integer vector
	 *
	 *	description				draws the current frame rect of the spritesheet at the given coordinates
	 */
	void		Render(CIwSVec2 pPosition = CIwSVec2::g_Zero);
};