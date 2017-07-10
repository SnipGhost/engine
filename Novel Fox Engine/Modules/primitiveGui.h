 /*
	Primitive Gui - simple gui form SFML 2.0
	http://primitivegui.sourceforge.net/
    Copyright (C) 2013 sajmon (https://sourceforge.net/users/sajmonek)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef primitiveGui_h
#define primitiveGui_h

#include <SFML\Graphics.hpp>
#include <sstream>
#include <iostream>

class pGui;
class pText;
class pWidget;
class pButton;
class pTextEdit;
class pCheckBox;
class pSlider;
class pProgressBar;
class pWindow;


/**
*	\brief Helper class. Cut text to widget size.
*
*	\class pText
*	This class inherits of sf::Text class from SFML. It's used by widgets which have string to display.
*/
class pText : public sf::Text
{
public:
	/**
	*	Default constructor.
	*	\param marginLeft
	*	\param marginRight
	*	\param textCentered
	*	\param cutDirection : if 0 cut from begin of string or if 1 cut from end of string
	*/
	pText( float marginLeft, float marginRight, bool textCentered, int cutDirection );
	
	/**
	*	Set parent widget geometry to fill text in this rectangle.
	*	\param position
	* 	\param size
	*/
	void setParentGeom( sf::Vector2f position, sf::Vector2f size);
	
	/**
	*	Add new string to existing string.
	*	\param symbol
	*/
	void addSymbol( sf::String& symbol );
	
	/**
	*	Set new string instead of existing string.
	*	\param symbol
	*/
	void setNewSymbol( sf::String& symbol );
	
	/** 
	*	Remove last symbol from string
	*/
	void removeLastSymbol( );
	
	/**
	*	Update text ( cut to parent widget size and set parent position ).
	*/
	void updateText( );
	
	sf::String fullString; ///< Full string of text object
	sf::String visibleString; ///< Visible string of text object
private:
	sf::Vector2f parentPos;
	sf::Vector2f parentSize;
	float marginLeft;
	float marginRight;
	bool textCentered;
	int cutDirection;//{0:zostaje końcówka tekstu, 1:zostaje początek tekstu}
};

/**
*	\brief Abstract class. Every widget class inherits from this class.
*
* 	\class pWidget
* 	This class inherits of sf::Drawable class from SFML. It's a base class for all widgets.
*/
class pWidget : public sf::Drawable
{
	friend class pGui;
	friend class pButton;
	friend class pTextEdit;
	friend class pCheckBox;
	friend class pSlider;
	friend class pProgressBar;
	friend class pWindow;

public:
	pWidget();
	
	/**
	*	Default constructor.
	*	\param x
	*	\param y
	*	\param width
	*	\param height
	*/
	pWidget( float x, float y, float width, float height );
	
	/**
	*	Set window geometry - position and size
	*	If widget is in some pWindow widget you must set position ( x and y ) relative to this window
	*	\param x
	*	\param y
	*	\param width
	*	\param height
	*/
	void setGeometry( float x, float y, float width, float height );
	
	/**
	*	Set visiblity of widget. If vidget visiblity is false widget isn't shown and isn't recive any event's.
	*	\param visiblity
	*/
	void setVisiblity( bool visiblity );
	
	/**
	*	Update widget. Every widget have own impementation of this function, because some widgets have more elements to draw.
	*/
	virtual void updateWidget() = 0;
	
	int widgetId; ///< Widget unique index
	int widgetOnWindowId; ///< If widget is on some window this is a unique index of this window ( it's widgetId of this window ) else it is -1
	bool visiblity; ///< visibility of widget
	
	/**
	*	\brief Enumeration of the different types of widgets
	*/
	enum widgetTypeEnum {
		Button=0, ///< Widget type is button
		TextEdit, ///< Widget type is text edit
		CheckBox, ///< Widget type is check box
		Slider, ///< Widget type is slider 
		ProgressBar, ///< Widget type is progress bar
		Window ///< Widget type is window
	};
	widgetTypeEnum widgetType; ///< Type of the widget
	
	/**
	*	\brief Enumeration of the different types of widgets
	*/
	enum widgetEventType {
		NoEvent=0, ///< Widget have no event
		WidgetClicked ///< Widget is clicked
	};
	widgetEventType widgetEvent; ///< Type of the event
	
	sf::Vector2f widgetPosition; ///< Position of widget
	sf::Vector2f widgetSize; ///< Size of widget
private:
	virtual void checkMouseEvent(sf::Vector2i&, bool) = 0;
	virtual void symbolEntered(sf::Uint32) = 0;
	
	bool isActive;
	pWindow* widgetInWindow;
	sf::Vector2f onWindowPos;
	sf::RectangleShape widgetShape;
};

/**
*	\brief Button class.
*
*	\class pButton
*	This class inherits of pWidget class.
*	Button recive click events.
* 
*	Usage example:
*	\code
*	pButton button(5, 5, 150, 50);
*	gui.addWidget( &button );
*	
*	// In event loop
*	if( button.widgetEvent == pWidget::WidgetClicked )
*		// button was clicked -> put your code here :)
*	\endcode
*/
class pButton : public pWidget
{
	friend pWindow;
public:
	pButton();
	
	/**
	*	Default constructor.
	*	\param x
	*	\param y
	*	\param width
	*	\param height
	*/
	pButton( float x, float y, float width, float height );
	
	/**
	*	Set button colors.
	*	\param borderColor 
	*	\param fillColor 
	*	\param hoverColor 
	*	\param clickedColor 
	*/
	void setColors( sf::Color borderColor, sf::Color fillColor, sf::Color hoverColor, sf::Color clickedColor );
	
	/**
	*	Set new string on buttonLabel.
	*	\param text
	*/
	void setText( sf::String& text );
	
	/**
	*	Get the text of buttonLabel.
	*	\return sf::String : fullString from buttonLabel \see pText
	*/
	sf::String getText( );
	
	virtual void updateWidget( );

	sf::Color borderColor; ///< Color of button border - \b default ( 117, 119, 114, 255 )
	sf::Color fillColor; ///< Color of default button background - \b default ( 85, 87, 82, 255 )
	sf::Color hoverColor; ///< Color of button when it's hovered - \b default ( 136, 127, 131, 255 )
	sf::Color clickedColor; ///< Color of button when it's clicked - \b default ( 81, 78, 45, 255 )
	sf::Color colorToDraw; ///< Color which is actually draw
	pText buttonLabel; ///< Text on button
private:
	virtual void checkMouseEvent(sf::Vector2i&, bool);
	virtual void symbolEntered(sf::Uint32){};
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void updateShape( );
};

/**
*	\brief Text edit class.
*
*	\class pTextEdit
*	This class inherits of pWidget class.
*	This is very simple text edit allow only put new character on the and of string and remove last character.
*	Text edit have two states active and not active.
*
*	User must click on text edit to have premmision to enter text.
*/

class pTextEdit : public pWidget
{
public:
	pTextEdit( );
	/**
	*	Default constructor.
	*	\param x
	*	\param y
	*	\param width
	*	\param height
	*/
	pTextEdit( float x, float y, float width, float height );
	
	/**
	*	Set text edit colors.
	*	\param borderColor
	*	\param notActiveColor
	*	\param activeColor
	*/
	void setColors( sf::Color borderColor, sf::Color notActiveColor, sf::Color activeColor );
	
	/**
	*	Set new string on text edit
	*	\param text
	*/
	void setText( sf::String& text );
	
	/**
	*	Get the full string from text edit
	*	\return sf::String \see pText
	*/
	sf::String getText( );

	virtual void updateWidget();

	pText textEditText; ///< Text in text edit
	sf::Color borderColor; ///< Text edit border color - \b default (117,119,114,255)
	sf::Color notActiveColor; ///< Background color of text edit when it isn't active - \b default (200,165,157,255)
	sf::Color activeColor; ///< Background color of text edit when it is active - \b default (177,177,177,255)
	sf::Color colorToDraw; ///< Color which is actually draw - \b default
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void checkMouseEvent(sf::Vector2i&, bool);
	virtual void symbolEntered(sf::Uint32);
	void updateShape();
};

/**
*	\brief Check box class.
*
*	\class pCheckBox
*	This class inherits of pWidget class.
*
*	pCheckBox has two states, you can change it by modifying the variable \b isChecked.
*/
class pCheckBox : public pWidget
{
public:
	pCheckBox( );
	/**
	*	Default constructor.
	*	\param x
	*	\param y
	*	\param width
	*	\param height
	*/
	pCheckBox( float x, float y, float width, float height );
	
	/**
	*	Set check box colors.
	*	\param borderColor
	*	\param uncheckedColor
	*	\param checkedColor
	*/
	void setColors( sf::Color borderColor, sf::Color uncheckedColor, sf::Color checkedColor );
	
	/**
	*	Change check box state
	*	\param checked
	*/
	void setChecked( bool checked );
	
	/**
	*	Set text on check box label
	*	\param text
	*/
	void setText( sf::String& text );
	
	/**
	*	Get the text from checkBoxLabel
	*	\return sf::String : fullString from checkBoxLabel \see pText
	*/
	sf::String getText( );
	virtual void updateWidget( );
	
	bool isChecked; ///< Check box state, default \b false
	sf::Color borderColor; ///< Border color of checkable square - \b default ( 117, 119, 114, 255 )
	sf::Color uncheckedColor; ///< Background color of checkable square - \b default ( 85, 87, 82, 255 )
	sf::Color checkedColor; ///< Background color of checkable square when it's checked - \b default Red
	pText checkBoxLabel; ///< Text on check box
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void checkMouseEvent(sf::Vector2i&, bool);
	virtual void symbolEntered(sf::Uint32){};
	void updateShape();
	sf::RectangleShape checkedShape;
};

/**
*	\brief Slider class.
*
*	\class pSlider
*	This class inherits of pWidget class.
*
*	This is a basic slider widget, movable rect will have a height that you set and slider shabe is lower by 20 px from the movable rect.
*/
class pSlider : public pWidget
{
public:
	pSlider( );
	/**
	*	Default constructor.
	*	\param x
	*	\param y
	*	\param width
	*	\param height
	*/
	pSlider( float x, float y, float width, float height );
	
	/**
	*	Set slider colors
	*	\param borderColor
	*	\param fillColor
	*	\param sliderColor
	*/
	void setColors( sf::Color borderColor, sf::Color fillColor, sf::Color sliderColor );
	
	/**
	*	Set value of slider
	*	\param value
	*/
	void setValue( float value );
	
	/**
	*	Get current value of slider
	*	\return float
	*/
	float getValue();
	virtual void updateWidget();

	float value; ///< Current value of slider
	sf::Color borderColor; ///< Border color of slider shape - \b default ( 117, 119, 114, 255 )
	sf::Color fillColor; ///< Background color of sluder shape - \b default ( 85, 87, 82, 255 )
	sf::Color sliderColor; ///< Background color of movable rect - \b default ( 81, 130, 66, 255 )
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void checkMouseEvent(sf::Vector2i&, bool);
	virtual void symbolEntered(sf::Uint32){};
	void updateShape();
	sf::RectangleShape sliderShape;
};

/**
*	\brief Progress bar class.
*
*	\class pProgressBar
*	This class inherits of pWidget class.
*
*	This is a basic progress bar. It contains only two rectangles and text :P
*/
class pProgressBar : public pWidget
{
public:
	pProgressBar();
	/**
	*	Default constructor.
	*	\param x
	*	\param y
	*	\param width
	*	\param height
	*/
	pProgressBar( float x, float y, float width, float height );
	
	/**
	*	Set progress bar colors
	*	\param borderColor
	*	\param fillColor
	*	\param barColor
	*/
	void setColors( sf::Color borderColor, sf::Color fillColor, sf::Color barColor );
	
	/**
	*	Set value of progress bar
	*	\param value
	*/
	void setValue( float value );
	
	/**
	*	Get current value of progress bar
	*	\return float
	*/
	float getValue();
	virtual void updateWidget();
	
	float value; ///< Current value of progress bar
	pText progressBarLabel; ///< Text on progress bar
	sf::Color borderColor; ///< Border color of progress bar - \b default ( 117, 119, 114, 255 )
	sf::Color fillColor; ///< Background of widget shape - \b default ( 81, 78, 45, 255 )
	sf::Color barColor; ///< Background of progress rect shape - \b default ( 81, 130, 66, 255 )
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void checkMouseEvent(sf::Vector2i&, bool){};
	virtual void symbolEntered(sf::Uint32){};
	void updateShape();
	sf::RectangleShape inBar;
};

/**
*	\brief Window class.
*
*	\class pWindow
*	This class inherits of pWidget class.
*
*	This is basic window. It is movable in main window rect. You can toggle content of window.
*
*	\b Warning: 
*
*	1) If window cover a part of some widget this widget don't recive events.
*
*	2) If window is not active there is drawn mask on it.
*
*	3) Default window is movable and toggled.
*/
class pWindow : public pWidget
{
	friend class pGui;
	friend class pWidget;
	friend class pButton;
	friend class pTextEdit;
	friend class pCheckBox;
	friend class pSlider;
	friend class pProgressBar;
public:
	pWindow();
	/**
	*	Default constructor.
	*	\param x
	*	\param y
	*	\param width
	*	\param height
	*/
	pWindow( float x, float y, float width, float height );
	
	/**
	*	Add widget to window.
	*	\param *widget
	*/
	void addWidget( pWidget* widget );
	
	/**
	*	Set new windowTitle.
	*	\param text
	*/
	void setWindowTitle( sf::String& text );
	
	/**
	*	Get the current window title.
	*	\return sf::String : fullString from titleBarLabel \see pText
	*/
	sf::String getWindowTitle( );

	/**
	*	Set window movalbe.
	*	\param movable
	*/
	void setMovable( bool movable );
	
	/**
	*	Check window is movable.
	*	\return bool
	*/
	bool checkMovable();
	
	/**
	*	Toggle window content ( Window title bar is still shown ).
	*	\param toggle
	*/
	void setToggle( bool toggle );
	
	/**
	*	Check is window content toggled.
	*	\return bool
	*/
	bool checkToggled();
	
	/**
	*	Set window colors.
	*	\param borderColor
	*	\param titleBarColor
	*	\param windowColor
	*	\param notActiveMaskColor
	*/
	void setColors( sf::Color borderColor, sf::Color titleBarColor, sf::Color windowColor, sf::Color notActiveMaskColor);
	virtual void updateWidget();
	
	int zValue;
	pText titleBarLabel; ///< Text on title bar
	sf::Color borderColor; ///< Border color of window - \b default ( 117, 119, 114, 255 )
	sf::Color titleBarColor; ///< Color of window title bar - \b default ( 81, 130, 66, 255 )
	sf::Color windowColor; ///< Color of window content - \b default ( 85, 87, 82, 200 )
	sf::Color notActiveMaskColor; ///< Color of mask drawn when window is not active - \b default ( 255, 0, 0, 50 )
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void updateWindow(sf::Event&, const sf::Window&);
	virtual void checkMouseEvent(sf::Vector2i&, bool){};
	virtual void symbolEntered(sf::Uint32){};
	void updateShape();
	sf::Vector2i mousePosition;
	sf::RectangleShape titleBar;
	sf::RectangleShape notActiveMask;
	sf::Vector2i mOldPos;
	sf::Vector2f widgetToggledSize;
	bool toggle;
	bool isMovable;
	pGui* guiHandler;
	pButton toggleButton;
	int widgetsCount;
	std::vector<pWidget*> widgetsList;
};

/**
*	\brief Main class of gui
*
*	\class pGui
*	This class inherits of sf::Drawable class from SFML.
*
*	This class contains all widgets, set it's unique index, handles events and draw widgets on main window.
*
*	\b Method update(...) must be call in event loop otherwise widgets will not be able to recive events:
*	\code
*	while(mainWindow.isOpen())
*		while(mainWindow.pollEvent(event))
*			ourGui.update(event, mainWindow);
*	\endcode
*	After clear main window you must draw gui
*	\code mainWindow.draw( ourGui ); \endcode
*/
class pGui : public sf::Drawable
{
	friend class pWindow;
public:
	/**
	*	Default constructor.
	*/
	pGui();
	
	/**
	*	Add widget to gui.
	*	\param *widget
	*/
	void addWidget( pWidget* widget );
	
	/**
	*	Add window widget to gui.
	*	\param *window
	*/
	void addWindow( pWindow* window );
	
	/**
	*	Update gui, check widgets events
	*	\param event
	*	\param window
	*/
	void update( sf::Event& event, const sf::Window& window );
	
	/**
	*	If you use windows you can set the window on top.
	*	\param *window
	*/
	void changeOnTopWindow( pWindow* window );
	int windowOnTopId; ///< Index of window on top
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
	int widgetsCount;
	std::vector<pWidget*> widgetsList;

	int windowsCount;
	std::vector<pWindow*> windowsList;
	
	pWindow* windowOnTop;
	int windowClickId;
	sf::Vector2i mousePosition;
};

#endif