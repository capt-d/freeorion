// -*- C++ -*-
//ClientUI.h
#ifndef _ClientUI_h_
#define _ClientUI_h_

#ifndef _SitRepEntry_h_
#include "../util/SitRepEntry.h"
#endif

#ifndef _StringTable_h_
#include "StringTable.h"
#endif

#ifndef _GGWnd_h_
#include "GGWnd.h"
#endif

#ifndef _SDLGGApp_h_
#include "SDLGGApp.h"
#endif

class ClientNetworkCore;
class ClientUniverse;
class ClientEmpire;
class Combat;
class Fleet;
class MapWnd;
class IntroScreen;
class TurnProgressWnd;
class Tech;
class ToolContainer;
class ToolWnd;
class SitRepEntry;
class System;
namespace GG {
class Clr;
class SubTexture;
class XMLElement;
}
namespace log4cpp {
class Category;
}

//! \brief ClientUI Main Module
//!This is the main class of the ClientUI module.
//!it contains objects of many other classes and controls the
//!display of all information oTURN_PROGRESS_WND_COLORnscreen.

class ClientUI
{
public:
    //!Internal States
    //!These determine what screen is currently being displayed
    enum State
    {
        STATE_STARTUP     = 0,
        STATE_INTRO       = 1,
        STATE_SETTINGS    = 2,
        STATE_EMPIRESEL   = 3,
        STATE_TURNSTART   = 4,
        STATE_MAP         = 5,
        STATE_SITREP      = 6,
        STATE_PROCESS     = 7,
        STATE_BATTLE      = 8,
        STATE_SAVE        = 9,
        STATE_LOAD        = 10,
        STATE_SHUTDOWN    = 11
    };

    enum Cursor
    {
        CURSOR_DEFAULT     = 0,
        CURSOR_COLONIZE    = 1,
    };

    
    //! \name Structors //!@{
    ClientUI();        //!< construction (calls Initialize())
    ~ClientUI();    //!< destruction (calls Cleanup())

    bool Initialize(const std::string& string_table_file);    //!< provided to setup initial variables. 
    bool Cleanup();        //!< provided to clean up ClientUI's memory space. 
    //!@}
    
    //! \name Accessors //!@{
    const log4cpp::Category& Logger() {return s_logger;}    //!< Returns the logger associated with ClientUI

    const std::string&  Language() const;                   //!< Returns the language of the StringTable object associated with ClientUI

    MapWnd* GetMapWnd() {return m_map_wnd;}                 //!< Returns the main map window (may be null).

    const GG::SubTexture& SitRepIcon(SitRepEntry::EntryType type) const; //!< returns the icon for this sitrep entry type; returns the default icon if \a type has no associated icon

    GG::XMLElement XMLEncode() const; //!< encodes ClientUI to XML
    //!@}
    
    //! \name Mutators //!@{
    //! @param width screen width
    //! @param height screen height
    bool ChangeResolution(int width, int height);    //!< changes the screen resolution and modifies any images or data required to make the change.  
    
    //! @param parent A pointer to the Wnd that should contain the tooltip
    //! @param tool A pointer to a ToolWnd to be associated with that window
    bool AttachToolWnd(GG::Wnd* parent, ToolWnd* tool);    //!< Adds a ToolWnd to the given window

    // GameCore Interface functions
    // calling these changes internal state to display the proper screen
    // and initializes and displays the screen
    // see Interface Doc for details
    // takes as a paramter the new turn number
    //!@{
    void InitTurn( int turn_number );      //!< resets all active controls to use the latest data when it has been changed at the beginning of a new turn
    
    void ScreenIntro();                        //!< Intro Screen
    void ScreenSettings(const ClientNetworkCore& net);    //!< Game/Network Options Screen
    void ScreenEmpireSelect();                    //!< Empire Selection Screen
    void ScreenProcessTurn();                     //!< Turn Star Progress Splash Screen

    void ScreenMap();     //!< Universe Map Screen

    //!< Updates turn progress window
    void UpdateTurnProgress( const std::string& phase_str, const int empire_id );

    //! @param events vector containing all the events to be listed
    void ScreenSitrep(const std::vector<SitRepEntry>& events);    //!< Sitrep Screen
   
    //! @param combat pointer to a Combat module
    void ScreenBattle(Combat* combat);                //!< Battle Screen
    
    //! @param show true if the screen is to be displayed, false if it is to be turned off
    void ScreenSave(bool show);                    //!< Savegame Screen
    
    //! @param show true if the screen is to be displayed, false if it is to be turned off
    void ScreenLoad(bool show);                    //!< Load Game Screen

    // Zooming Functions
    //! @param id address of a planet that we wish to zoom to
    //! @return true if successful, false if object doesn't exist
    bool ZoomToPlanet(int id);    //!< Zooms to a particular planet on the galaxy map and opens the planet screen for that planet

    //! @param id address of system that we wish to zoom to
    //! @return true if successful, false if object doesn't exist
    bool ZoomToSystem(int id);    //!< Zooms to a particular system on the galaxy map
    
    //! @param id address of fleet that we wish to zoom to
    //! @return true if successful, false if object doesn't exist
    bool ZoomToFleet(int id);    //!< Zooms to a particular fleet on the galaxy map and opens the fleet window
    
    //! @param id address of ship that we wish to zoom to
    //! @return true if successful, false if object doesn't exist
    bool ZoomToShip(int id);    //!< Zooms to a particular ship on the galaxy map and opens its fleet and/or ship window
    
    //! @param id address of technology that we wish to zoom to
    //! @return true if successful, false if object doesn't exist
    bool ZoomToTech(int id);    //!< Opens the technology screen and presents a description of the given technology
   
    //! @param str address of encyclopedia entry that we wish to zoom to
    //! @return true if successful, false if object doesn't exist
    bool ZoomToEncyclopediaEntry(const std::string& str);    //!< Opens the encyclodedia screen and presents the entry for the given item

    void ZoomToSystem(System* system); //!< Zooms to a particular system on the galaxy map
    void ZoomToFleet(Fleet* fleet);    //!< Zooms to a particular fleet on the galaxy map and opens the fleet window

    /** brings up the SidePanel as a modal window, and allows the user to select one of the planets in system \a system_id;
	returns -1 if no planet is selected */
    int SelectPlanet(int system_id);

    void SetCursor( Cursor new_cursor ); //! < Sets the current cursor
    //!@}
    
    static ClientUI*    GetClientUI() {return s_the_UI;}   //!< returns a pointer to the singleton ClientUI class

    //! @param message The message to display
    static void MessageBox(const std::string& message);    //!< shows a message dialog box with the given message     

    static void LogMessage(const std::string& msg); //!<sends a message to the logger
    static const std::string&  String(const std::string& index);    //!< Returns a lookup from the string table

    static void GenerateSitRepText( SitRepEntry *p_sit_rep ); ///< generates a SitRep string from it's XML data.



    //! \name Static Config Data
    //!@{
    static std::string FONT;    //!< The default font to use
    static int         PTS;    //!< default point size
    static std::string TITLE_FONT;    //!< The default font to use for the window title
    static int         TITLE_PTS;    //!< default point size to use for window title
    
    static std::string DIR;     //!< directory currently being used, contains config files
    static std::string ART_DIR;    //!< directory holding artwork, ("*/art/small/" or "*/art/large/"

    static GG::Clr     TEXT_COLOR; //!< color of UI text
    
    // generic UI windows
    static GG::Clr     WND_COLOR; //!< color of a UI window
    static GG::Clr     WND_BORDER_COLOR; //!< color of window borders
    static GG::Clr     WND_OUTER_BORDER_COLOR; //!< color of the outermost border
    static GG::Clr     WND_INNER_BORDER_COLOR; //!< color of the innermost border

    // controls
    static GG::Clr     CTRL_COLOR; //!< color of UI controls
    static GG::Clr     CTRL_BORDER_COLOR;

    static GG::Clr     BUTTON_COLOR;
    static int         BUTTON_WIDTH;    //!< default width to use for window buttons

    static GG::Clr     STATE_BUTTON_COLOR;

    static GG::Clr     SCROLL_TAB_COLOR;
    static  int        SCROLL_WIDTH;
    
    static GG::Clr     DROP_DOWN_LIST_INT_COLOR;
    static GG::Clr     DROP_DOWN_LIST_ARROW_COLOR;

    static GG::Clr     EDIT_INT_COLOR;

    static GG::Clr     MULTIEDIT_INT_COLOR;

    static GG::Clr     STAT_INCR_COLOR; //!< used to color increasing stats text (eg "+2")
    static GG::Clr     STAT_DECR_COLOR; //!< used to color decreasing stats text (eg "-3")

    static int         SYSTEM_ICON_SIZE; //!< the width/height of a System/Icon at zoom = 1.0
    static double      FLEET_BUTTON_SIZE; //!< the width/height of a FleetButton at zoom = 1.0, relative to the size of a SystemIcon

    // game UI windows
    static GG::Clr     SIDE_PANEL_COLOR;
    static GG::Clr     SIDE_PANEL_BUILD_PROGRESSBAR_COLOR;
    static int         SIDE_PANEL_PLANET_NAME_PTS;
    static int         SIDE_PANEL_PTS;

        
    //!@}

private:
    void HideAllWindows();              //!< hides all the UI windows from view

    const Uint32 TOOLTIP_DELAY;         //!< number of milliseconds to initialize tooltips to
    ToolContainer* m_tooltips;          //!< the single toolcontainer object
    State m_state;                      //!< represents the screen currently being displayed

    StringTable* m_string_table;        //!< a string table to lookup international strings

    IntroScreen*     m_intro_screen;      //!< the intro (and main menu) screen first showed when the game starts up
    MapWnd*          m_map_wnd;           //!< the galaxy map
    TurnProgressWnd* m_turn_progress_wnd; //!< the turn progress window

    SDL_Cursor*      m_default_cursor;              //!< used to store default cursor
 
    static log4cpp::Category& s_logger; //!< log4cpp logging category
    static ClientUI* s_the_UI;          //!<pointer to the one and only ClientUI object
};


#endif
