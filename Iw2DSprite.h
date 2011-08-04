/*!**********************************************************************************

	\class			Iw2DSprite
	\brief			Animated sprite class.
	\date			11/07/2011
	\version		0.1					
	\author			Leandro Ostera

	Animation class that features non-complete squared sprite sheets support.
	Includes basics functionality as stopping, pausing, go to frame, etc.

************************************************************************************/

#pragma once // might be more suitable to use define checks, but pragma once seems to work fine on almost every compiler

/*
 * Marmalade SDK Header file includes
 */
#include <IwResManagerClass.h>
#include <IwResGroup.h>
#include <IwGeomCore.h>
#include <Iw2D.h>

/*
 * Standard C++ Header file includes
 */
#include <string>


class Iw2DSprite
{
private:
	/*!
	 * \property m_Built bool flag to determine whether the object is already built or not
	 */
	bool		m_Built; 

	/*!
	 * \property m_Flip CIwSVec2 vector determines if sprite is or not flipped in each axis.
	 */
	CIwSVec2	m_Flip;

	/*!
	 * \property m_Angle rotation angle in radians (32-bits integer)
	 */
	iwangle		m_Angle;

	/*!
	 * \property m_Center rotation Center.
	 */
	CIwSVec2	m_Center;

	/*!
	 *	is the pointer to CIwImage that actually holds the sprite sheet image.
	 *	It uses the Marmalade SDK resource manager to load it
	 */
	CIw2DImage*		m_SpriteSheet;

	/*!
	 *	name of the resource loaded into m_SpriteSheet;
	 */
	std::string		m_ResourceName;

	/*!
	 *	name of the group where the resource named m_ResourceName is located
	 */
	CIwResGroup*	m_ResourceGroup;
	
	/*!
	 *	(x,y) position of the currently to be drawn frame
	 */
	CIwSVec2	m_DrawRect;

	/*!
	 *	the actual size of a frame. .x for width, .y for height
	 */
	CIwSVec2	m_FrameSize;

	/*!
	 *	the amount of rows (.x) and columns (.y) the animation has
	 */
	CIwSVec2	m_Frames;

	/*!
	 *	the horizontal and vertical pixel offset of each frame
	 */
	CIwSVec2	m_Offset;

	/*!
	 *	in which row (.x) and column (.y) are we now
	 */	
	CIwSVec2	m_CurrentFrame;

	/*!
	 *	the max amount of frames that actually have something.
	 *	mostly used in square sprite sheets where some frames are blank.
	 */
	int16		m_MaxFrames;

	/*!
	 *	internal counter to iterate through the frame "matrix".
	 */
	int16		m_FrameCounter;

	/*!
	 *	sets which frame ends the animation.
	 */	
	int16		m_EndFrame;
	
	/*!
	 *	boolean flag to determine whether to loop or not once reached the final frame
	 */
	bool		m_Loop;

	/*!
	 *	boolean flag that indicates if the animation has been stopped and therefore return to the first frame
	 */
	bool		m_Stop;

	/*!
	 * boolean flag telling us if we shall continue with the animation or freeze it
	 */		
	bool		m_Pause; 


	//Private class methods: do not attempt to use in your own code!

	/*!
	 *	\brief	Loads a resource from a resource group.
	 *
	 *	\param	pGroup	the resource group inwhere to look for the resource
	 *	\param	pName	the name of the resource to look up in the group
	 *	\param	pDataFromFile	wheter or not to read width, height and max frames from file
	 *
	 *	\returns	bool	true if sprite was succesfully rebuilt, false otherwise.
	 *
	 *	Loads a resource named name from the resource group that group points to if the current
	 *	group is different from the one specified, the new group is set to be the current.
	 *	All the other parameters are initialized from configuration file if the resource name 
	 * 	and group equals the current resource name and group then nothing is done.
	 *  If current resource name differs from pName, current resource group is not pGroup,
	 *	the object has not yet been built or both the resource name and resource group has
	 *	not been set previously, set pGroup as current resource group, pName as current
	 *	resource name and try to load the resource.
	 *	If anything but error, the image should have been succesfully loaded into memory, and all
	 *	image-dependant properties of the class properly initialized.
	 */
	bool LoadFromResource(CIwResGroup* pGroup, std::string pName, bool pDataFromFile = true);
	
public:
	/*!
	 *	\brief	Parameterized constructor
	 *
	 *	\param	pGroup	the resource group inwhere to look for the resource
	 *	\param	pName	the name of the resource to look up in the group
	 *	\param	pLoop	whether the animation sohuld loop (true) or not (false
	 *
	 *	This is the main constructor. It initializes the following properties:
	 *					\arg	\c m_Built, false
	 *					\arg	\c m_Offset, CIwSVec2::g_Zero (zero vector)						
	 *					\arg	\c m_CurrentFrame,  CIwSVec2::g_Zero (zero vector)
	 *					\arg	\c m_FrameCounter,  0 (ZERO, integer value)
	 *					\arg	\c m_DrawRect,  CIwSVec2::g_Zero (zero vector)
	 *					\arg	\c m_Pause,  false
	 *					\arg	\c m_Stop,  false
	 *					\arg	\c m_Loop,  pLoop
	 */
	Iw2DSprite(CIwResGroup* pGroup, std::string pName, bool pLoop = true);

	/*!
	 *	\brief	Cleans up the object.
	 *
	 *	Performs clean up of the asigned memory blocks. 
	 *  If the class has been succesfully built before, delete the sprite pointer as it is no longer needed.
	 */
	~Iw2DSprite();

	/*!
	 *	\brief	Copy constructor
	 *
	 *	\param	pSprite	The sprite object to be copied
	 *
	 *	Copies the propierties of the parameter object into the object that is being constructed.
	 *	It uses the overloaded operator = to such ends. Current object flag m_Built will be set along
	 *	with flag m_Copy.
	 */
	Iw2DSprite(Iw2DSprite const& pSprite);

	/*!
	 *	\brief	Assignment operator
	 *
	 *	\param	pSprite	The sprite object to be assigned (copied)
	 *
	 *	Copies all data from the sprite parameter object, incluiding the pointer to the spritesheet.
	 */
	Iw2DSprite&	operator= (Iw2DSprite const& pSprite);

	/*!
	 *	\brief	Comparison operator
	 *
	 *	\param	pSprite1	A sprite object to compare against pSprite2
	 *	\param	pSprite21	A sprite object to compare against pSprite1
	 *
	 *	\returns	bool
	 *
	 *	Compares both sprites and returns true if are equal, or false if not.
	 */
	bool		operator== (Iw2DSprite const& pSprite) const;
	
	/*!
	 *	\brief	Updated the sprite.
	 *	\returns	bool	True if animation is within range, false if animation is about to restart/finish
	 *						depending on looping
	 *
	 *	Iterates through the sprite sheet as if it was a matrix by incrementing m_CurrentFrame.x
	 *	(if not paused nor stopped) untill it is equal or greater than (note that it will never
	 *	be greater -or at least shouldn't!) then it goes back to zero and m_CurrentFrame.y is incremented.
	 *	m_CurrentFrame(x,y) works as indexes for the sprites in the sheet: x is for the column, y for the
	 *	rows.
	 *	Additionally, in case that the sprites has / uses less than x*y frames in the sheet, there is max
	 *	frames number and a frame counter used to check for it.
	 *	Later, the drawing rect is updated.
	 */
	bool		Step();

	/*!
	 *	\brief	Loop's or not?
	 *
	 *	\param pLoop	true for looping, false otherwise
	 *
	 *	Set loop on or off
	 */
	inline void	Loop(bool pLoop) { m_Loop = pLoop; }

	/*!
	 *	\brief	Toggles looping.
	 *
	 *	Switches looping state on if off and off if on.
	 */
	inline void	toggleLoop() { m_Loop = (m_Loop)?false:true; }

	/*!
	 *	\brief	Stops the animation.
	 *
	 *	Stops the animation and returns to first frame.
	 */
	void		Stop(); 

	/*!
	 *	\brief	Starts the animation.
	 *
	 *	Starts the animation from the current frame.
	 */
	void		Play();

	/*!
	 *	\brief Resumes the animation from were it was.
	 *
	 *	If animation was paused, it resumes playing from the current frame.
	 */
	void		Resume();

	/*!
	 *	\brief Pauses the animation.
	 *
	 *	Pauses the animation at the current frame.
	 */
	void		Pause();
	
	/*!
	 *	\brief	Goes to the specified frame.
	 *
	 *	\param	pFrame	CIwSVec2 2D integer vector indicating row and column of the frame to set as current
	 *
	 *	Ff not specified a vector, returns to first frame, otherwise sets f as current frame.
	 */
	void		GoToFrame(CIwSVec2 pFrame = CIwSVec2::g_Zero);

	/*!
	 *	\brief Restarts the animation.
	 *
	 *  Restarts the animation by returning to the first frame.
	 */
	void		Restart() { GoToFrame(); }

	/*!
	 *	\brief Has the animation paused?
	 *
	 *	\returns		bool
	 *
	 *  True if it is paused, false otherwise.
	 */
	inline bool		IsPaused()			const { return m_Pause; }

	/*!
	 *	\brief Has the animation stopped?
	 *
	 *	\returns		bool
	 *
	 *	True if it is stopped, false otherwise.
	 */
	inline bool		IsStopped()			const { return m_Stop; }	

	/*!
	 *	\brief Is the animation looping?
	 *
	 *	\returns		bool
	 *
	 *	True if it is looping, false otherwise.
	 */
	inline bool		IsLooping()			const { return m_Loop; }	

	/*!
	 *	\brief Is the animation being played?
	 *
	 *	\returns		bool
	 *
	 *	True if it is not paused and not stopped, false otherwise.
	 */
	inline bool		IsPlaying()			const { return (!m_Stop&&!m_Pause)?true:false; }

	/*!
	 *	\brief Is the object built?
	 *
	 *	\returns		bool	
	 *
	 *	True if the sprite has been built, false otherwise.
	 */
	inline bool		IsBuilt()			const { return m_Built; }

	/*!
	 *	\brief Is the object flipped?
	 *
	 *	\returns		CIwSVec2
	 *
	 *	CIwSVec2 boolean vector with (1,0) for flipped on X, (0,1) for flipped on Y, (1,1) for flipped on both, and (0,0) for not flipped at all.
	 */
	inline CIwSVec2	IsFlipped()			const { return m_Flip; }

	/*!
	 *	\brief	Spritesheet width.
	 *
	 *	\returns		int16	
	 *
	 *	Returns current spritesheet width as integer.
	 */
	int16		GetWidth()			const { return m_SpriteSheet->GetWidth(); }
	
	/*!
	 *	\brief	Spritesheet height.
	 *
	 *	\returns		int16
	 *
	 *	Returns current spritesheet height as integer.
	 */
	int16		GetHeigth()			const { return m_SpriteSheet->GetHeight(); }

	/*!
	 *	\brief	Get the maximum useful frame.	 
	 *
	 *	\returns		int16
	 *
	 *	Returns the maximum amount of useful frames in the spritesheet
	 *  that is, ignoring blank frames at the end of the sheet, as integer.
	 */
	int16		GetMaxFrames()		const { return m_MaxFrames; }

	/*!
	 *	\brief	Get the final frame.	 
	 *
	 *	\returns		int16
	 *
	 *	Returns the last frame number as integer.
	 */
	int16		GetEndFrame()		const { return m_EndFrame; }
	
	/*!
	 *	\brief	Get the current frame vector.
	 *
	 *	\returns		CIwSVec2
	 *
	 *	Returns current frame row and column as a 2D integer vector.
	 */
	CIwSVec2		GetCurrentFrame()	const { return m_CurrentFrame; }

	/*!
	 *	\brief	Get the frame size vector.
	 *
	 *	\returns		CIwSVec2
	 *
	 *	Returns frame size as 2D integer vector.
	 */
	CIwSVec2		GetFrameSize()		const { return m_FrameSize; }

	/*!
	 *	\brief	Get the frame offset vector.
	 *
	 *	\returns		CIwSVec2
	 *
	 *	Returns x and y offset of each frame as 2D integer vector.
	 */
	CIwSVec2		GetFrameOffset()	const { return m_Offset; }

	/*!
	 *	\brief	Get the resource name string.
	 *
	 *	\returns		std::string
	 *
	 *	Returns a copy of the resource name as a std::string.
	 */
	std::string			GetResourceName()	const { return m_ResourceName; }

	/*!
	 *	\brief	Get the resources group pointer.	 
	 *
	 *	\returns		CIwResGroup*
	 *
	 *	Returns a pointer to the current resource group.
	 */
	CIwResGroup*		GetResourceGroup()	const { return m_ResourceGroup; }

	/*!
	 *	\brief	Pointer to const this.	 
	 *
	 *	\returns		const Iw2DSprite*
	 *
	 *	Returns a pointer to a const Iw2DSprite (which is this).
	 */
	const Iw2DSprite*	GetCopy()		const { return this; }

	/*!
	 *	\brief	Animation rotation.
	 *
	 *	\returns	Returns the current angle in radians.
	 *
	 */
	iwangle		GetAngle()	const { return m_Angle; }

	/*!
	 *	\brief	Animation rotation Center.
	 *
	 *	\returns	Returns the current Center as a vector.
	 *
	 */
	CIwSVec2	GetCenter()	const { return m_Center; }

	/*!
	 *	\brief	Rotates the animation.
	 *	\returns	void
	 *	\param	pRads	Angle in radians (32-bits integer)
	 *
	 *	Rotates the animation by pRads radians counterclockwise.
	 */
	void		Rotate(iwangle	pRads);

	/*!
	 *	\brief	Center the animation for rotation
	 *	\returns	void
	 *	\param	pCenter	integer vector for animation center.
	 *
	 *	Centers the animation at the given coordinates for rotation.
	 */
	void		Center(CIwSVec2 pCenter = CIwSVec2::g_Zero);

	/*!
	 *	\brief	Flips the animation
	 *	\returns	void
	 *	\param	pFlip	CIwSVec2 vector for flipping on X and/or Y axis.
	 *
	 *	Flips the animation according the the pFlip vector. If the current Flip vector equals the passed Flip vector, animation is unflipped.
	 */
	void		Flip(CIwSVec2 pFlip = CIwSVec2::g_Zero);

	/*!
	 *	\brief	Render the animation.
	 *
	 *	\returns		void
	 *
	 *	\param	pPosition	(x,y) 2D integer vector
	 *
	 *	Draws a region of the image starting at m_DrawRect and finishing at m_DrawRect+m_FrameSize,
	 *	at the coordinates passed by pPosition.
	 */
	void		Render(CIwSVec2 pPosition = CIwSVec2::g_Zero);
};