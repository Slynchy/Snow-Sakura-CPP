/// \file 		M22Engine.h
/// \author 	Sam Lynch
/// \brief 		Header file for entire M22 engine
/// \version 	0.4.4
/// \date 		April 2016
/// \details	The header file for declaring the engine and its functions/variables.

#ifndef M22ENGINE_H
#define M22ENGINE_H

#define DEFAULT_MUSIC_VOLUME_MULT 0.25f
	/*!< The default volume of music playback. */
#define DEFAULT_SFX_VOLUME_MULT 0.35f
	/*!< The default volume of \a SFX. */
#define DEFAULT_LERP_SPEED 0.15f
	/*!< Defines the speed of any \a lerp function. */

#define RENDERING_API		"opengl"
	/*!< Defines which rendering API to use; generally \a direct3d or \a opengl. */
#define BILINEAR_FILTERING	"1"
	/*!< Set to 1 for bilinear filtering, 0 for not. Must be a string. */

#include "SDL.h"
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "engine/Vectors.h"
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <string>
#include <chrono>

/// \class 		M22Engine M22Engine.h "include/M22Engine.h"
/// \brief 		The main class of M22.
///
/// \details 	This class holds most barebones functions/variables for basic operation (SDL window, renderer, etc.)
///
class M22Engine
{
	private:
	public:
		/// Different window states for the renderer
		enum WINDOW_STATES
		{
			WINDOWED,
				///< Screen is windowed and resizable.
			FULLSCREEN,
				///< Screen is fullscreen at the application resolution.
			FULLSCREEN_BORDERLESS,
				///< Screen is windowed but fullscreen-borderless.
			NUM_OF_STATES
		};
		
		/// Data structure of the options file
		struct OPTIONS_STRUCTURE
		{
			Uint8 WINDOWED;
				///< Refers to M22Engine::WINDOW_STATES for what state the window should be in.
			float AUTO_SPEED;
				///< Speed to auto-click/progress at. \a CURRENTLY UNUSED.
			float MUSIC_VOLUME;
				///< Volume to play music at.
			float SFX_VOLUME;
				///< Volume to play \a SFX at.
			OPTIONS_STRUCTURE()
			{
				WINDOWED = 0;
				AUTO_SPEED = 1.0f;
				MUSIC_VOLUME = DEFAULT_MUSIC_VOLUME_MULT;
				SFX_VOLUME = DEFAULT_SFX_VOLUME_MULT;
			};
		};

		static OPTIONS_STRUCTURE OPTIONS; ///< Current game options, initialized to default, later loaded.

		/// Data structure of loaded characters
		struct Character
		{
			std::string name;
				///< Characters name (e.g. "Yuuji")
			std::vector<std::vector<SDL_Texture*>> sprites;
				///< 2D array of sprites; y = outfit, x = emotion
			std::vector<std::string> emotions;
				///< Emotion names (for file-loading, e.g. "Happy_1" -> "Happy_1.png")
			std::vector<std::string> outfits;
				///< Outfit names (for file-loading, e.g. "School" -> "School/Happy_1.png")
		};
		/// Structure for characters to draw on-screen.
		struct CharacterReference
		{
			SDL_Texture* sprite;
				///< The active sprite
			SDL_Rect rect;
				///< Position to draw to screen
			float alpha;
				///< Current alpha amount
			bool clearing;
				///< Is this character currently being erased off the screen?
			CharacterReference()
			{
				alpha = 0.0f;
				clearing = false;
				sprite = NULL;
			};
		};
		/// Structure for backgrounds
		struct Background
		{
			SDL_Texture* sprite;
				///< The background image
			float alpha;
				///< Current alpha amount
		};
		/// Possible gamestates
		enum GAMESTATES
		{
			MAIN_MENU,
				///< Is on the main menu
			INGAME
				///< Is in-game
		};
		
		static Vec2 MousePos; 		///< Current mouse position
		static bool LMB_Pressed; 	///< Is LMB currently pressed?

		static bool skipping;		///< Is player currently skipping dialogue?
		
		static bool QUIT;			///< Exit the program?
		
		/// Finds the index of the character from the string
		///
		/// \param _name Character's name.
		/// \param _dialogue Don't know; just leave it blank!
		/// \return Index of character, 0 if narrative
		static int GetCharacterIndexFromName(std::string, bool _dialogue = false); 

		/// Finds the outfit index for the specified character
		///
		/// \param _input Outfit's name.
		/// \param _charIndex Index of character
		/// \return Index of outfit
		static int GetOutfitIndexFromName(std::string, int);
		
		/// Finds the emotion index for the specified character
		///
		/// \param _input Emotion name.
		/// \param _charIndex Index of character
		/// \return Index of emotion
		static int GetEmotionIndexFromName(std::string, int _charIndex);

		/// Resets the game + variables
		static void ResetGame(void);

		/// Starts the game
		static void StartGame(void);

		/// Saves the current configuration of options to OPTIONS.SAV
		static void SaveOptions(void);

		/// Loads the current configuration of options to OPTIONS.SAV
		static void LoadOptions(void);
		
		/// Updates OPTIONS struct + file with new settings
		static void UpdateOptions(void);
		
		/// Shuts down the engine, SDL, and destroys variables
		static void Shutdown(void);
		
		/// Updates delta time variables
		static void UpdateDeltaTime(void);
		
		/// Updates SDL Events
		static void UpdateEvents(void);
		
		/// Updates keyboard input array
		static void UpdateKeyboard(void);
		
		/// Initializes OPTIONS data by saving if it doesn't exist, loading if it does
		static void OptionsFileInitializer(void);

		static Vec2 ScrSize; 		///< Logical screen resolution to render at

		static std::vector<std::string> CHARACTER_NAMES;		///< Array of character names (for file-loading, e.g. "Yuuji" -> "Yuuji/School/Happy_1.png")
		static unsigned short int ACTIVE_BACKGROUND_INDEX;		///< The index of the current background, relative to \a ACTIVE_BACKGROUNDS;
		static std::vector<Background> ACTIVE_BACKGROUNDS;		///< Array of backgrounds to be drawn (should be no larger than two)

		static std::vector<Character> CHARACTERS_ARRAY;			///< Array of loaded characters and their sprites
		static std::vector<std::string> CHARACTER_EMOTIONS;		///< Array of character emotions \a(UNUSED?)

		static Uint32 last, DELTA_TIME; 						///< Delta time variables
		static Uint32 TIMER_CURR, TIMER_TARGET;					///< Delta time variables

		static M22Engine::GAMESTATES GAMESTATE;					///< Active gamestate from \a M22Engine::
		static SDL_Window* SDL_SCREEN;							///< Context for the SDL Window
		static SDL_Renderer *SDL_RENDERER;						///< Context for the SDL Renderer
		static SDL_Event SDL_EVENTS;							///< Context for the SDL Events
		static const Uint8 *SDL_KEYBOARDSTATE;					///< Current keyboard button states
		static SDL_DisplayMode SDL_DISPLAYMODE;
		
		/// Initializes the M22 engine
		///
		/// \param ScrW Screen width
		/// \param ScrH Screen height
		/// \return Error code, if 0 then init'd fine
		static short int InitializeM22(int ScrW, int ScrH);
		
		/// Initializes the SDL part of the engine
		///
		/// \param _windowTitle Title of window
		/// \param _version Version of software
		/// \param ScrPos Position window should be when initialized if windowed
		/// \return Error code, if 0 then init'd fine
		static short int InitializeSDL(const std::string _windowTitle, const std::string _version, Vec2 ScrPos);
};

/// \class 		M22Graphics M22Engine.h "include/M22Engine.h"
/// \brief 		Class for graphics drawing
///
/// \details 	This class is responsible for drawing backgrounds and characters (graphics-related operations).
///
class M22Graphics
{
	private:
	public:
		/// The type of update the background is doing
		enum BACKGROUND_UPDATE_TYPES
		{
			NONE,
			BACKGROUND,				///< Changing the background
			CHARACTER				///< Updating the characters
		};
		/// Data structure for the animated arrow for text progression
		struct ArrowObj
		{
			SDL_Texture* sprite;	///< Sprite
			float frame;			///< The frame that the arrow is on
			ArrowObj()
			{
				sprite = NULL;
				frame = 0.0f;
			};
		};
		static SDL_Texture* BACKGROUND_RENDER_TARGET;						///< The off-screen render target for the background
		static SDL_Texture* NEXT_BACKGROUND_RENDER_TARGET;					///< The off-screen render target for the next background
		static float NEXT_BACKGROUND_ALPHA;									///< The alpha of the next background (for fading in)
		static BACKGROUND_UPDATE_TYPES changeQueued;						///< The type of the background change scheduled

		static SDL_Texture* textFrame;										///< Texture for the primary text frame
		static ArrowObj arrow;												///< The text arrow object
		static std::vector<SDL_Texture*> characterFrameHeaders;				///< The array of sprites for character names when they talk
		static std::vector<M22Engine::CharacterReference> activeCharacters;	///< Array of active characters to draw to the screen
		static std::vector<SDL_Texture*> mainMenuBackgrounds;				///< The possible backgrounds for the main menu to use, loaded into this array
		static M22Engine::Background activeMenuBackground;					///< The active background for the main menu
		static M22Engine::Background menuLogo;								///< The game's logo to draw onto the main menu
		static SDL_Texture* OPTION_BAR;										///< The texture for bars in the options, like volume (UNUSED)

		static SDL_Texture* BLACK_TEXTURE;									///< A simple solid black texture for fading to black (0,0,0,255)

		static std::vector<SDL_Texture*> BACKGROUNDS;						///< Loaded background textures
		static std::vector<std::string> backgroundIndex;					///< Names of the backgrounds to know which one to load for scripts
		static short int LoadBackgroundsFromIndex(const char* _filename);	
		static void DrawBackground(SDL_Texture* _target);
		static void UpdateBackgrounds(void);
		static void UpdateCharacters(void);
		static void DrawInGame(bool _draw_black = true);
		static void FadeToBlackFancy(void);
		static void AddActiveCharacter(int _charindex, int _outfitindex, int _emotionindex, int _xPosition, bool _brutal = false);
		static void UpdateBackgroundRenderTarget(void);
		static void AddCharacterToBackgroundRenderTarget(int _charindex, int _outfitindex, int _emotionindex, int _xPosition, bool _brutal = false);

		static void DrawArrow(int ScrW, int ScrH);

		static float Lerp(float _var1, float _var2, float _t); 

		static TTF_Font *textFont;											///< The TTF font to use for speech/narrative text.

		static SDL_Rect* wipePosition;										///< The current position of the active transition wipe
		static SDL_Texture* wipeBlack;										///< A texture for wiping black; just \a BLACK_TEXTURE reference
		static SDL_Rect wipeBlackRect;										///< The current position of the black wipe
		
		/// Type of transition
		enum TRANSITIONS
		{
			SWIPE_TO_RIGHT,													///< Swipe from left of the screen to right
			SWIPE_DOWN,														///< Swipe from top of the screen to bottom
			SWIPE_TO_LEFT,													///< Swipe from right of the screen to left
			FADEIN,															///< Lerp/fade between the backgrounds
			NUMBER_OF_TRANSITIONS
		};

		static const std::string TRANSITION_NAMES[NUMBER_OF_TRANSITIONS];	///< Name of transitions for scripts to use

		static Uint8 activeTransition;										///< Which transition to use, refering to \a TRANSITIONS enum
};

/// \class 		M22Sound M22Engine.h "include/M22Engine.h"
/// \brief 		Class for sound handling
///
/// \details 	This class is responsible for music/sound playback/loading/managing.
///
class M22Sound
{
	private:
	public:
		/// Enumerator for different mixers/channels for specific types of sound
		enum MIXERS
		{
			BGM,											///< Background music
			SFX,											///< Sound FX
			VOICE,											///< Voice (UNUSED CURRENTLY)
			LOOPED_SFX										///< SFX that is to loop continuously
		};
		static std::vector<Mix_Chunk*> SOUND_FX;			///< Array of loaded SFX files
		static std::vector<Mix_Music*> MUSIC;				///< Array of loaded music files
		static std::vector<std::string> MUSIC_NAMES;		///< Array of loaded music file names (for scripts)
		static std::vector<std::string> SFX_NAMES;			///< Array of loaded SFX file names (for scripts)
		static float* MUSIC_VOLUME;							///< Current volume for music playback
		static float* SFX_VOLUME;							///< Current volume for SFX playback
		static int currentTrack;							///< The active track to play in \a MUSIC array. 0 = silence

		/// Plays a SFX once, doesn't play if a SFX is already playing
		///
		/// \param _position Index of sound file from \a SOUND_FX array.
		/// \return Error code if problem encountered, 0 if fine
			static short int PlaySting(short int);
			
		/// Plays a SFX once, can be forced to playback if true
		///
		/// \param _position Index of sound file from \a SOUND_FX array.
		/// \param _forceplayback If true, will force the sound effect to play as high priority.
		/// \return Error code if problem encountered, 0 if fine
			static short int PlaySting(short int, bool);
			
		/// Searches for the SFX from the string, and plays back.
		///
		/// \param _name Name of sound file from \a SFX_NAMES array.
		/// \param _forceplayback If true, will force the sound effect to play as high priority.
		/// \return Error code if problem encountered, 0 if fine
			static short int PlaySting(std::string, bool);
			
		/// Searches for the SFX from the string, and plays back on a continuous loop.
		///
		/// \param _name Name of sound file from \a SFX_NAMES array.
		/// \return Error code if problem encountered, 0 if fine
			static short int PlayLoopedSting(std::string);
			
		/// Stops anything playing in the \a LOOPED_SFX mixer
			static void StopLoopedStings(void);

		/// Changes the active music track
		///
		/// \param _position Index of sound file in \a MUSIC array.
		/// \return Error code if problem encountered, 0 if fine
			static short int ChangeMusicTrack(short int _position);
			
		/// Searches for track from string, changes the active music track to result
		///
		/// \param _name Name of sound file from \a MUSIC_NAMES array.
		/// \return Error code if problem encountered, 0 if fine
			static short int ChangeMusicTrack(std::string _name);
			
		/// Stops anything playing in the \a BGM mixer
			static void StopMusic();
		/// Pauses anything playing in the \a LOOPED_SFX mixer
			static void PauseMusic();
		/// Resumes whatever is paused in the \a LOOPED_SFX mixer
			static void ResumeMusic();

		/// Updates the sound, checking whether to loop
			static void UpdateSound(void);

		/// Initializes music+SFX
		/// \return -1 if music fails, -2 if SFX fails, 0 if fine
			static short int InitializeSound(void);

		/// Initializes music
		/// \return Anything other than 0 means an error
			static short int InitializeMusic(void);

		/// Initializes SFX
		/// \return Anything other than 0 means an error
			static short int InitializeSFX(void);
			
		/// EMPTY FUNCTION, QUEUED FOR REMOVAL
			static short int StartMusic(short int _position);
};

/// \class 		M22Script M22Engine.h "include/M22Engine.h"
/// \brief 		Class for script handling
///
/// \details 	This class is responsible for loading and handling script files for M22.
///
class M22Script
{
	private:
	public:
		/// Enumerator for the type of line the script is on
		enum LINETYPE
		{
			NEW_BACKGROUND,					///< Changes the background using active transition
			FADE_TO_BLACK,					///< Fade the background to black
			FADE_TO_BLACK_FANCY,			///< Fade the background to black; hijacks the thread for a nicer effect.
			NEW_MUSIC,						///< Changes the current music track
			DARK_SCREEN,					///< Darkens the screen slightly
			BRIGHT_SCREEN,					///< Restores the screen brightness from dark screen
			STOP_MUSIC,						///< Stops music playback
			PLAY_STING,						///< Plays the specified SFX
			PLAY_STING_LOOPED,				///< Plays the specified SFX on loop
			STOP_STING_LOOPED,				///< Stops any looped SFX
			GOTO,							///< Goes straight to specified line
			DRAW_CHARACTER,					///< Add new character to active characters
			CLEAR_CHARACTERS,				///< Remove all characters from active characters
			CLEAR_CHARACTERS_BRUTAL,		///< Remove all characters from active characters without transition
			DRAW_CHARACTER_BRUTAL,			///< Add new character to active characters without transition
			LOAD_SCRIPT,					///< Terminate the current script and load the specified one
			SPEECH,							///< Speech from a character
			COMMENT,						///< Code comment
			WAIT,							///< Waits N milliseconds before loading the next line
			EXITGAME,						///< Exit game
			SET_ACTIVE_TRANSITION,			///< Changes the active transition
			EXITTOMAINMENU,					///< Exit to main menu
			NARRATIVE						///< Speech without chat box (thoughts of main character; narrative)
		};
		static const unsigned short int DARKEN_SCREEN_OPACITY = 100;	///< Current opacity of the darken screen effect

		static std::string currentLine;									///< Current line from script, loaded into string
		static int currentLineIndex;									///< Current line index in \a currentScript
		static std::vector<std::string> currentScript;					///< Active script, loaded each line as an array of strings
		static int activeSpeakerIndex;									///< The index of the active speaker, for chat box names
		static SDL_Surface *currentLineSurface;							///< Current line surface, for drawing the text off-screen
		static SDL_Surface *currentLineSurfaceShadow;					///< Current line surface, for drawing the text shadow off-screen

		static float fontSize;											///< The size of the text font; not sure if still used?

		/// Loads the script file into \a currentScript
		///
		/// \param _filename File path/name of script file
		/// \return Error code if problem encountered, 0 if fine
			static short int LoadScriptToCurrent(const char* _filename);
			
		/// Draws the contents of \a currentLine to screen
		///
		/// \param ScrW Screen width resolution
		/// \param ScrH Screen height resolution
			static void DrawCurrentLine(int ScrW, int ScrH);
			
		/// Changes currentLine to target line index
		///
		/// \param _newLine Index of new target line
			static void ChangeLine(int _newLine);
			
		/// Splits string into parts between specified character into an array
		///
		/// \param txt Target string to split
		/// \param strs Address of string array to split into
		/// \param ch Character to split between
			static unsigned int SplitString(const std::string&, std::vector<std::string>&, char);
			
		/// Checks and returns the type of the string from \a LINETYPE
		///
		/// \param _input String to check
		/// \return Type of line as \a LINETYPE enumerator
			static M22Script::LINETYPE CheckLineType(std::string);
			
		/// Checks and returns if the character is a colon ( : )
		///
		/// \param _char Character to check
			static bool isColon(int _char);
			
		/// Clears active characters array
			static void ClearCharacters(void);
			
		/// Fades to screen black
			static void FadeToBlack(void);
};

/// \class 		M22Interface M22Engine.h "include/M22Engine.h"
/// \brief 		Class for interfaces/mouse input
///
/// \details 	This class is responsible for loading, running, displaying and updating interfaces.
///
class M22Interface
{
	private:
	public:
		/// Enumerator for the different states of interface button
		enum BUTTON_STATES
		{
			NOMOUSEOVER,						///< Mouse is over a different button (UNUSED?)
			RESTING,							///< Normal state
			MOUSEOVER,							///< Mouse is over this button
			CLICKED,							///< This button has been clicked
			NUM_OF_BUTTON_STATES
		};
		/// Enumerator for the different interfaces
		enum INTERFACES
		{
			INGAME_INTRFC,						///< Interface for ingame (skip, auto, menu)
			MENU_BUTTON_INTRFC,					///< Interface for ingame menu button (save, load, options)
			MAIN_MENU_INTRFC,					///< Interface for main menu (start, exit, options)
			OPTIONS_MENU_INTRFC,				///< Interface for options menu (exit to title, exit game, etc.)
			NUM_OF_INTERFACES
		};
		
		/// Data structure for a button on an interface
		struct Button
		{
			std::string name;						///< Name of button
			BUTTON_STATES state;					///< State of button
			SDL_Texture* sheet;						///< Sprite sheet for button
			SDL_Rect rectSrc[NUM_OF_BUTTON_STATES];	///< Where the current sprite is on the spritesheet
			SDL_Rect rectDst[NUM_OF_BUTTON_STATES];	///< Where to draw the sprite
			Button()
			{
				state = RESTING;
				sheet = NULL;
			};
		};

		/// Data structure for an interface
		struct Interface
		{
			std::vector<M22Interface::Button> buttons;		///< Array of buttons in the interface
			SDL_Texture* spriteSheet;						///< Background of interface (optional)
			float alpha;									///< Current alpha
			M22Interface::INTERFACES type;					///< Type of interface
			
			Interface()
			{
				alpha = 0.0f;
			};
			
			/// Fades in all buttons using \a Lerp and \a alpha, usually used for main menu
			void Interface::FadeInAllButtons(void)
			{
				this->alpha = M22Graphics::Lerp( this->alpha, 255.0f, DEFAULT_LERP_SPEED/4 );
				for(size_t i = 0; i < this->buttons.size(); i++)
				{
					SDL_SetTextureAlphaMod( this->buttons[i].sheet, Uint8(this->alpha) );
				};
			};
			
		};
		
		static std::vector<Interface> storedInterfaces;			///< Array of loaded interfaces
		static std::vector<Interface*> activeInterfaces;		///< Array of pointers to interfaces to draw/update
		static M22Interface::BUTTON_STATES* skipButtonState;	///< Current state of skip button
		static M22Interface::BUTTON_STATES* menuButtonState;	///< Current state of menu button
		static SDL_Texture* ChatBoxRenderer;					///< Render target for drawing speech box off-screen

		static bool menuOpen;									///< Is the menu open?

		/// Draws all interfaces in \a activeInterfaces
			static void DrawActiveInterfaces(void);
			
		/// Draws the buttons of the active interfaces
			static void DrawActiveInterfacesButtons(void);
			
		static bool DRAW_TEXT_AREA;								///< Draw the text area?
			
		/// Draws the text box, name of person talking, current line; off-screen then into the main renderer
		/// \param _ScrSizeX Screen width
		/// \param _ScrSizeY Screen height
			static void DrawTextArea(int, int);
			
		/// Updates active interfaces, checking if they've been clicked, etc.
		/// \param _ScrSizeX Screen width
		/// \param _ScrSizeY Screen height
			static void UpdateActiveInterfaces(int _ScrSizeX, int _ScrSizeY);
			
		/// Checks if a single point is within a box
		/// \param _pos1 Single point
		/// \param _pos2 Box position
		/// \param _size Size of box
		/// \return True if overlap, false if not
			static bool CheckOverlap(Vec2 _pos1, Vec2 _pos2, Vec2 _size);
			
		/// Resets all stored (and by extension, active) interfaces to default settings
			static void ResetStoredInterfaces(void);

		/// Initializes the text box and sprites
			static void InitTextBox(void);

		/// Initializes an interface from buttons file + constants
		///
		/// \param _interface Pointer to interface to initialize
		/// \param _num_of_buttons Number of buttons in interface
		/// \param _startline Line to start on in buttons file
		/// \param _filename File path/name of buttons file
		/// \param _opaque Is the interface opaque to begin with?
		/// \param _type Type of interface from \a M22Interface::INTERFACES
		/// \return Error code if problem encountered, 0 if fine
			static short int InitializeInterface(M22Interface::Interface* _interface, int _num_of_buttons, int _startline = 0, const std::string _filename = "graphics/interface/GAME_BUTTONS.txt", bool _opaque = true, M22Interface::INTERFACES _type = M22Interface::INTERFACES::INGAME_INTRFC);
};

#endif