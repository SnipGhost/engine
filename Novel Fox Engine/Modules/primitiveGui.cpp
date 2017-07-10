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

#include "primitiveGui.h"

bool sortWindows( pWindow* const w1, pWindow* const w2 )
{
	return (w1->zValue < w2->zValue);
}

bool sortWindows2(pWindow* const w1, pWindow* const w2)
{
	return (w1->zValue > w2->zValue);
}

pGui::pGui( )
{
	this->widgetsCount = 0;
	this->windowsCount = 0;
	this->windowOnTopId = -1;
	this->windowClickId = -1;
}

void pGui::addWidget( pWidget* widget )
{
	widget->widgetId = this->widgetsCount;
	this->widgetsList.push_back( widget );
	this->widgetsCount += 1;
}

void pGui::addWindow( pWindow* window )
{
	window->widgetId = this->windowsCount;
	window->zValue = this->windowsCount+1;
	window->guiHandler = this;
	if(this->windowsCount == 0)
	{
		this->windowOnTop = window;
		this->windowOnTopId = this->windowsCount;
	}
	else
		this->changeOnTopWindow( window );
	for(unsigned int i = 0; i < window->widgetsList.size(); i++)
	{
		window->widgetsList[i]->widgetOnWindowId = this->windowsCount;
		window->widgetsList[i]->updateWidget();
	}
	window->toggleButton.widgetId = this->windowsCount;
	this->windowsList.push_back( window );
	this->windowsCount += 1;
}

void pGui::changeOnTopWindow( pWindow* window )
{
	if(window->widgetId != this->windowOnTop->widgetId)
	{
		int oz = this->windowOnTop->zValue;
		int nz = window->zValue;

		this->windowOnTop->zValue = nz;
		window->zValue = oz;

		this->windowOnTop = window;
		this->windowOnTopId = window->widgetId;
	}
}

void pGui::update( sf::Event& event, const sf::Window& window )
{
	this->mousePosition = sf::Mouse::getPosition( window );

	for(unsigned int i = 0; i < this->windowsList.size(); i++)
		this->windowsList[i]->updateWindow( event, window );

	for(unsigned int i = 0; i < this->widgetsList.size(); i++)
	{
		this->widgetsList[i]->widgetEvent = pWidget::NoEvent;

		if(this->widgetsList[i]->visiblity)
		{
			bool isIn = false;

			sf::FloatRect controlRect(this->widgetsList[i]->widgetPosition, this->widgetsList[i]->widgetSize);
			for(unsigned int j = 0; j < this->windowsList.size(); j++)
			{
				if(!isIn)
					isIn = controlRect.intersects(sf::FloatRect(this->windowsList[j]->widgetPosition, this->windowsList[j]->widgetSize));
			}

			this->widgetsList[i]->checkMouseEvent( this->mousePosition, false );
			if( event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && isIn == false)
				this->widgetsList[i]->checkMouseEvent( this->mousePosition, true );

			if( event.type == sf::Event::MouseMoved && this->widgetsList[i]->widgetType == pWidget::Slider && isIn == false)
			{
				sf::Vector2i mp = sf::Vector2i( event.mouseMove.x, event.mouseMove.y );
				if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
					this->widgetsList[i]->checkMouseEvent( mp, true );
			}

			if(this->widgetsList[i]->widgetType == pWidget::TextEdit)
				if( event.type == sf::Event::TextEntered && this->widgetsList[i]->isActive )
					this->widgetsList[i]->symbolEntered( event.text.unicode );
		}
	}

	std::sort(this->windowsList.begin(), this->windowsList.end(), sortWindows2);
}

void pGui::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
	for(unsigned int i = 0; i < this->widgetsList.size(); i++)
	{
		if(this->widgetsList[i]->visiblity)
			target.draw( *this->widgetsList[i] );
	}
	for(unsigned int i = 0; i < this->windowsList.size(); i++)
	{
		if(this->windowsList[i]->visiblity)
			target.draw( *this->windowsList[i] );
	}
}

//#####################################################################################################################

pWidget::pWidget( float x, float y, float width, float height )
	: widgetPosition(x, y), widgetSize(width, height)
{
	this->widgetId = 0;
	this->widgetOnWindowId = -1;
	this->visiblity = true;
	this->isActive = false;
}

void pWidget::setGeometry( float x, float y, float width, float height )
{
	if(this->widgetOnWindowId == -1)
		this->widgetPosition = sf::Vector2f(x, y);
	else
	{
		this->widgetPosition = sf::Vector2f(x+this->widgetInWindow->widgetPosition.x, y+this->widgetInWindow->widgetPosition.y+40);
	}
	this->onWindowPos = sf::Vector2f(x, y);
	this->widgetSize = sf::Vector2f(width, height);
	this->updateWidget();
}

void pWidget::setVisiblity( bool visiblity )
{
	this->visiblity = visiblity;
}

//#####################################################################################################################

pButton::pButton( float x, float y, float width, float height )
	: pWidget(x, y, width, height), buttonLabel(5,5,true,1)
{
	this->widgetType = pWidget::Button;

	this->borderColor = sf::Color::Color( 117, 119, 114, 255 );
	this->fillColor = sf::Color::Color( 85, 87, 82, 255 );
	this->hoverColor = sf::Color::Color( 136, 127, 131, 255 );
	this->clickedColor = sf::Color::Color( 81, 78, 45, 255 );
	this->colorToDraw = fillColor;

	this->buttonLabel.setParentGeom( this->widgetPosition, this->widgetSize );
	this->buttonLabel.addSymbol( sf::String("Button") );

	this->updateWidget();
}

void pButton::updateWidget()
{
	this->updateShape();
	this->buttonLabel.setParentGeom( this->widgetPosition, this->widgetSize );
	this->buttonLabel.updateText();
}

void pButton::updateShape()
{
	this->widgetShape.setPosition( this->widgetPosition );
	this->widgetShape.setSize( this->widgetSize );
	this->widgetShape.setFillColor( this->colorToDraw );
	this->widgetShape.setOutlineColor( this->borderColor );
	this->widgetShape.setOutlineThickness( 1 );
}

void pButton::checkMouseEvent( sf::Vector2i& mp, bool clicked )
{
	if(mp.x >= this->widgetPosition.x && mp.x <= this->widgetPosition.x + this->widgetSize.x && mp.y >= this->widgetPosition.y && mp.y <= this->widgetPosition.y + this->widgetSize.y)
	{
		this->colorToDraw = this->hoverColor;
		if(clicked)
		{
			this->colorToDraw = this->clickedColor;
			this->widgetEvent = pWidget::WidgetClicked;
		}
	}
	else
		this->colorToDraw = fillColor;

	this->updateWidget();
}

void pButton::setColors( sf::Color borderColor, sf::Color fillColor, sf::Color hoverColor, sf::Color clickedColor )
{
	this->borderColor = borderColor;
	this->fillColor = fillColor;
	this->hoverColor = hoverColor;
	this->clickedColor = clickedColor;
	this->colorToDraw = fillColor;
}

void pButton::setText( sf::String& text )
{
	this->buttonLabel.setNewSymbol( text );
}

sf::String pButton::getText()
{
	return this->buttonLabel.fullString;
}

void pButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw( this->widgetShape );
	target.draw( this->buttonLabel );
}

//#####################################################################################################################

pTextEdit::pTextEdit(float x, float y, float width, float height)
	: pWidget(x, y, width, height), textEditText(5,5,false,0)
{
	this->widgetType = pWidget::TextEdit;

	this->textEditText.setParentGeom( this->widgetPosition, this->widgetSize );

	this->borderColor = sf::Color::Color(117,119,114,255);
	this->notActiveColor = sf::Color::Color(200,165,157,255);
	this->activeColor = sf::Color::Color(177,177,177,255);
	this->colorToDraw = this->notActiveColor;

	this->updateWidget();
}

void pTextEdit::updateWidget()
{
	this->updateShape();
	this->textEditText.setParentGeom( this->widgetPosition, this->widgetSize );
	this->textEditText.updateText();
}

void pTextEdit::updateShape()
{
	this->widgetShape.setPosition( this->widgetPosition );
	this->widgetShape.setSize( this->widgetSize );
	this->widgetShape.setFillColor( this->colorToDraw );
	this->widgetShape.setOutlineColor( this->borderColor );
	this->widgetShape.setOutlineThickness( 1 );
}

void pTextEdit::checkMouseEvent(sf::Vector2i& mp, bool clicked)
{
	if(clicked)
	{
		if(mp.x >= this->widgetPosition.x && mp.x <= this->widgetPosition.x + this->widgetSize.x && mp.y >= this->widgetPosition.y && mp.y <= this->widgetPosition.y + this->widgetSize.y)
		{
			this->isActive = true;
			this->colorToDraw = this->activeColor;
		}
		else
		{
			this->isActive = false;
			this->colorToDraw = this->notActiveColor;
		}
	}
	this->updateWidget();
}

void pTextEdit::symbolEntered(sf::Uint32 u)
{
	if(u == 8)
		this->textEditText.removeLastSymbol();
	else
		this->textEditText.addSymbol( sf::String(u) );
}

void pTextEdit::setColors( sf::Color borderColor, sf::Color notActiveColor, sf::Color activeColor )
{
	this->borderColor = borderColor;
	this->notActiveColor = notActiveColor;
	this->activeColor = activeColor;
	
	if(this->isActive)
		this->colorToDraw = this->activeColor;
	else
		this->colorToDraw = this->notActiveColor;
}

void pTextEdit::setText( sf::String& text )
{
	this->textEditText.setNewSymbol( text );
}

sf::String pTextEdit::getText()
{
	return this->textEditText.fullString;
}

void pTextEdit::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw( this->widgetShape );
	target.draw( this->textEditText );
}

//#####################################################################################################################

pCheckBox::pCheckBox(float x, float y, float width, float height)
	: pWidget(x, y, width, height), checkBoxLabel(height+5,5,false,1)
{
	this->widgetType = pWidget::CheckBox;
	this->isChecked = false;

	this->borderColor = sf::Color::Color( 117, 119, 114, 255 );
	this->uncheckedColor = sf::Color::Color( 85, 87, 82, 255 );
	this->checkedColor = sf::Color::Red;

	this->checkBoxLabel.setParentGeom( this->widgetPosition, this->widgetSize );
	this->checkBoxLabel.addSymbol( sf::String("CheckBox") );

	this->updateWidget();
}

void pCheckBox::updateWidget()
{
	this->updateShape();
	this->checkBoxLabel.setParentGeom( this->widgetPosition, this->widgetSize );
	this->checkBoxLabel.updateText();
}

void pCheckBox::updateShape()
{
	this->widgetShape.setPosition( this->widgetPosition.x+5, this->widgetPosition.y+5 );
	this->widgetShape.setSize( sf::Vector2f(this->widgetSize.y-10, this->widgetSize.y-10));
	this->widgetShape.setFillColor( this->uncheckedColor );
	this->widgetShape.setOutlineColor( this->borderColor );
	this->widgetShape.setOutlineThickness( 1 );

	this->checkedShape.setPosition(this->widgetPosition.x+7, this->widgetPosition.y+7 );
	this->checkedShape.setSize( sf::Vector2f(this->widgetSize.y-14, this->widgetSize.y-14));
	this->checkedShape.setFillColor( this->checkedColor );
}

void pCheckBox::checkMouseEvent(sf::Vector2i& mp, bool clicked)
{
	if(mp.x >= this->widgetPosition.x && mp.x <= this->widgetPosition.x + this->widgetSize.x && mp.y >= this->widgetPosition.y && mp.y <= this->widgetPosition.y + this->widgetSize.y)
	{
		if(clicked)
		{
			if(this->isChecked)
				this->isChecked = false;
			else
				this->isChecked = true;
		}
	}

	this->updateWidget();
}

void pCheckBox::setColors( sf::Color borderColor, sf::Color uncheckedColor, sf::Color checkedColor )
{
	this->borderColor = borderColor;
	this->uncheckedColor = uncheckedColor;
	this->checkedColor = checkedColor;
}

void pCheckBox::setChecked( bool checked )
{
	this->isChecked = checked;
	this->updateWidget();
}

void pCheckBox::setText( sf::String& text )
{
	this->checkBoxLabel.setNewSymbol( text );
}

sf::String pCheckBox::getText()
{
	return this->checkBoxLabel.fullString;
}

void pCheckBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw( this->widgetShape );
	if(this->isChecked)
		target.draw( this->checkedShape );
	target.draw( this->checkBoxLabel );
}

//#####################################################################################################################

pSlider::pSlider(float x, float y, float width, float height)
	: pWidget(x, y, width, height)
{
	this->widgetType = pWidget::Slider;

	this->value = 0.f;

	this->borderColor = sf::Color::Color( 117, 119, 114, 255 );
	this->fillColor = sf::Color::Color( 85, 87, 82, 255 );
	this->sliderColor = sf::Color::Color( 81, 130, 66, 255 );

	this->updateWidget();
}

void pSlider::updateWidget()
{
	this->updateShape();
}

void pSlider::updateShape()
{
	this->widgetShape.setPosition( this->widgetPosition.x, this->widgetPosition.y+10 );
	this->widgetShape.setSize( sf::Vector2f(this->widgetSize.x, this->widgetSize.y-20) );
	this->widgetShape.setFillColor( this->fillColor );
	this->widgetShape.setOutlineColor( this->borderColor );
	this->widgetShape.setOutlineThickness( 1 );

	float max = this->widgetSize.x;
	float pos = (max*this->value)/100.f;

	this->sliderShape.setOrigin( 10, this->widgetSize.y/2 );
	this->sliderShape.setPosition( this->widgetPosition.x+pos, this->widgetPosition.y+this->widgetSize.y/2 );
	this->sliderShape.setSize( sf::Vector2f(20, this->widgetSize.y) );
	this->sliderShape.setFillColor( this->sliderColor );
}

void pSlider::checkMouseEvent(sf::Vector2i& mp, bool clicked)
{
	if(mp.x >= this->widgetPosition.x && mp.x <= this->widgetPosition.x + this->widgetSize.x && mp.y >= this->widgetPosition.y && mp.y <= this->widgetPosition.y + this->widgetSize.y)
	{
		if(clicked)
		{
			this->value = (mp.x-this->widgetPosition.x)/(this->widgetSize.x/100);
			this->sliderShape.setPosition( (float)mp.x, this->sliderShape.getPosition().y );
		}
	}
}

void pSlider::setColors( sf::Color borderColor, sf::Color fillColor, sf::Color sliderColor )
{
	this->borderColor = borderColor;
	this->fillColor = fillColor;
	this->sliderColor = sliderColor;
}

void pSlider::setValue( float value )
{	
	this->value = value;
	this->sliderShape.setPosition(value*(this->widgetSize.x/100)+this->widgetPosition.x, this->sliderShape.getPosition().y );
}

float pSlider::getValue()
{
	return this->value;
}

void pSlider::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw( this->widgetShape );
	target.draw( this->sliderShape );
}

//#####################################################################################################################

pProgressBar::pProgressBar(float x, float y, float width, float height)
	: pWidget(x, y, width, height), progressBarLabel(5,5,true,0)
{
	this->widgetType = pWidget::ProgressBar;
	this->value = 0.f;

	this->borderColor = sf::Color::Color( 117, 119, 114, 255 );
	this->fillColor = sf::Color::Color( 81, 78, 45, 255 );
	this->barColor = sf::Color::Color( 81, 130, 66, 255 );
	
	this->progressBarLabel.setParentGeom( this->widgetPosition, this->widgetSize );
	this->progressBarLabel.addSymbol( sf::String("0%") );

	this->updateWidget();
}

void pProgressBar::updateWidget()
{
	this->updateShape();
	this->progressBarLabel.setParentGeom( this->widgetPosition, this->widgetSize );
	this->progressBarLabel.updateText();
}

void pProgressBar::updateShape()
{
	this->widgetShape.setPosition( this->widgetPosition );
	this->widgetShape.setSize( this->widgetSize );
	this->widgetShape.setFillColor( this->fillColor );
	this->widgetShape.setOutlineColor( this->borderColor );
	this->widgetShape.setOutlineThickness( 1 );

	this->inBar.setPosition( this->widgetPosition );
	this->inBar.setSize( sf::Vector2f(this->value*this->widgetSize.x/100, this->widgetSize.y ) );
	this->inBar.setFillColor( this->barColor );
}

void pProgressBar::setColors( sf::Color borderColor, sf::Color fillColor, sf::Color barColor )
{
	this->borderColor = borderColor;
	this->fillColor = fillColor;
	this->barColor = barColor;
}

void pProgressBar::setValue( float value )
{
	if(value >= 0 && value <= 100)
	{
		this->value = value;
		std::stringstream valueS (std::stringstream::in | std::stringstream::out);
		valueS << this->value << "%";
		this->progressBarLabel.setNewSymbol( sf::String(valueS.str()) );
		this->updateWidget();
	}
}

float pProgressBar::getValue()
{
	return this->value;
}

void pProgressBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw( this->widgetShape );
	target.draw( this->inBar );
	target.draw( this->progressBarLabel );
}

//#####################################################################################################################

pWindow::pWindow( float x, float y, float width, float height )
	: pWidget(x,y,width,height), titleBarLabel(5,5,false, 1), mOldPos((int)x, (int)y), toggleButton(x+width-30,y+10,20,20)
{
	this->widgetType = pWidget::Window;
	this->zValue = 0;
	this->guiHandler = NULL;
	this->widgetsCount = 0;
	this->toggle = true;
	this->isMovable = true;
	this->toggleButton.widgetId = this->widgetId;

	this->titleBarLabel.setParentGeom(this->widgetPosition, sf::Vector2f(this->widgetSize.x-30, 40));
	this->titleBarLabel.addSymbol( sf::String("Window") );

	this->borderColor = sf::Color( 117, 119, 114, 255 );
	this->titleBarColor = sf::Color::Color( 81, 130, 66, 255 );
	this->windowColor = sf::Color::Color( 85, 87, 82, 200 );
	this->notActiveMaskColor = sf::Color::Color( 255, 0, 0, 50 );

	this->toggleButton.setText( sf::String("_") );
	
	this->updateWidget();
}

void pWindow::addWidget( pWidget* widget )
{
	widget->widgetId = this->widgetsCount;
	widget->widgetOnWindowId = this->widgetId;
	widget->widgetInWindow = this;
	widget->onWindowPos.x = widget->widgetPosition.x;
	widget->onWindowPos.y = widget->widgetPosition.y;
	widget->widgetPosition.x = widget->widgetPosition.x+this->widgetPosition.x;
	widget->widgetPosition.y = widget->widgetPosition.y+this->widgetPosition.y+40;
	this->widgetsList.push_back(widget);
	this->widgetsCount += 1;
	widget->updateWidget();
}

void pWindow::updateWidget()
{
	this->updateShape();
	this->titleBarLabel.setParentGeom( this->widgetPosition, sf::Vector2f(this->widgetSize.x-30, 40) );
	this->titleBarLabel.updateText();
	this->toggleButton.setGeometry(this->widgetPosition.x+this->widgetSize.x-30, this->widgetPosition.y+10, 20, 20);
	this->toggleButton.buttonLabel.setParentGeom(sf::Vector2f(this->toggleButton.widgetPosition.x, this->toggleButton.widgetPosition.y-10), this->toggleButton.widgetSize);
}

void pWindow::updateShape()
{
	this->titleBar.setSize(sf::Vector2f( this->widgetSize.x, 40 ));
	this->titleBar.setPosition( this->widgetPosition );
	this->titleBar.setFillColor( this->titleBarColor );
	this->titleBar.setOutlineColor( this->borderColor );
	this->titleBar.setOutlineThickness( 1 );

	this->widgetShape.setSize( sf::Vector2f( this->widgetSize.x, this->widgetSize.y-40 ));
	this->widgetShape.setPosition( this->widgetPosition.x, this->widgetPosition.y+39 );
	this->widgetShape.setFillColor( this->windowColor );
	this->widgetShape.setOutlineColor( this->borderColor );
	this->widgetShape.setOutlineThickness( 1 );

	this->notActiveMask.setSize( this->widgetSize );
	this->notActiveMask.setPosition( this->widgetPosition );
	this->notActiveMask.setFillColor( this->notActiveMaskColor );
}

void pWindow::setColors( sf::Color borderColor, sf::Color titleBarColor, sf::Color windowColor, sf::Color notActiveMaskColor)
{
	this->borderColor = borderColor;
	this->titleBarColor = titleBarColor;
	this->windowColor = windowColor;
	this->notActiveMaskColor = notActiveMaskColor;
}

void pWindow::updateWindow(sf::Event& e, const sf::Window& w)
{
	this->mousePosition = sf::Mouse::getPosition(w);

	if(this->guiHandler->windowClickId == this->widgetId)
		this->guiHandler->windowClickId = -1;

	if(e.type == sf::Event::MouseButtonPressed)
	{
		this->mOldPos = sf::Vector2i(e.mouseButton.x - (int)this->widgetPosition.x, e.mouseButton.y - (int)this->widgetPosition.y);
		bool cc = true;
		sf::FloatRect ret;
		sf::FloatRect wR(this->widgetPosition, this->widgetSize);
		for(unsigned int j = 0; j < this->guiHandler->windowsList.size(); j++)
		{
			if(this->guiHandler->windowsList[j]->widgetId != this->widgetId)
			{
				sf::FloatRect w2R(this->guiHandler->windowsList[j]->widgetPosition, this->guiHandler->windowsList[j]->widgetSize);
				wR.intersects(w2R, ret);
				cc = ret.contains(sf::Vector2f((float)this->mousePosition.x, (float)this->mousePosition.y));
			}
		}
		if(this->mOldPos.x >= 0 && this->mOldPos.x <= this->widgetSize.x && this->mOldPos.y >= 0 && this->mOldPos.y <= 40 && e.mouseButton.x >= this->widgetPosition.x && e.mouseButton.x <= this->widgetPosition.x+this->widgetSize.x && e.mouseButton.y >= this->widgetPosition.y && e.mouseButton.y <= this->widgetPosition.y+40 && !cc)
		{
			this->guiHandler->windowClickId = this->widgetId;
			if(this->guiHandler->windowOnTop->widgetId != this->widgetId)
				this->guiHandler->changeOnTopWindow(this);
		}
	}

	if(e.type == sf::Event::MouseMoved && this->guiHandler->windowOnTop->widgetId == this->widgetId)
	{
		sf::Vector2i mp = sf::Vector2i(e.mouseMove.x, e.mouseMove.y);
		if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && this->isMovable)
		{
			if(mp.x >= this->widgetPosition.x && mp.x <= this->widgetPosition.x+this->widgetSize.x && mp.y >= this->widgetPosition.y && mp.y <= this->widgetPosition.y+40)
			{
				
				this->guiHandler->windowClickId = this->widgetId;
				this->widgetPosition.x =  (float)(mp.x - this->mOldPos.x);
				this->widgetPosition.y =  (float)(mp.y - this->mOldPos.y);
				
				
					if(this->widgetPosition.x < 0)
						this->widgetPosition.x = 0;
					if(this->widgetPosition.y < 0)
						this->widgetPosition.y = 0;
					if(this->widgetPosition.x > (w.getSize().x-this->widgetSize.x))
						this->widgetPosition.x = (w.getSize().x-this->widgetSize.x);
					if(this->widgetPosition.y > (w.getSize().y-this->widgetSize.y))
						this->widgetPosition.y = (w.getSize().y-this->widgetSize.y);

					
					for(unsigned int i = 0; i < this->widgetsList.size(); i++)
					{
						this->widgetsList[i]->widgetPosition.x = this->widgetsList[i]->onWindowPos.x + this->widgetPosition.x;
						this->widgetsList[i]->widgetPosition.y = this->widgetsList[i]->onWindowPos.y + 40 + this->widgetPosition.y;
					}
				
				
			}
		}
	}
		
	if((this->guiHandler->windowClickId == -1 || this->guiHandler->windowClickId == this->widgetId) && this->visiblity){
		
		this->toggleButton.widgetEvent = pWidget::NoEvent;
		this->toggleButton.checkMouseEvent(this->mousePosition, false);
		if(e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left && this->toggleButton.widgetId == this->guiHandler->windowOnTopId)
			this->toggleButton.checkMouseEvent(this->mousePosition, true);
		if(this->toggleButton.widgetEvent == pWidget::WidgetClicked)
		{
			if(this->toggle)
			{
				this->widgetToggledSize = this->widgetSize;
				this->widgetSize = sf::Vector2f(this->widgetSize.x, 40);
				this->toggle = false;
			}
			else{
				this->widgetSize = this->widgetToggledSize;
				this->toggle = true;
			}
		}

	for(unsigned int i = 0; i < this->widgetsList.size(); i++)
	{
		this->widgetsList[i]->widgetEvent = pWidget::NoEvent;
		this->widgetsList[i]->updateWidget();
		if(this->widgetsList[i]->visiblity && this->toggle && this->widgetsList[i]->widgetOnWindowId == this->guiHandler->windowOnTopId)
		{
			this->widgetsList[i]->checkMouseEvent(this->mousePosition, false);
			if(e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
				this->widgetsList[i]->checkMouseEvent(this->mousePosition, true);

			if(e.type == sf::Event::MouseMoved && this->widgetsList[i]->widgetType == pWidget::Slider)
			{
				sf::Vector2i mp = sf::Vector2i(e.mouseMove.x, e.mouseMove.y);
				if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
					this->widgetsList[i]->checkMouseEvent(mp, true);
			}

			if(this->widgetsList[i]->widgetType == pWidget::TextEdit)
				if(e.type == sf::Event::TextEntered && this->widgetsList[i]->isActive)
					this->widgetsList[i]->symbolEntered( e.text.unicode );
		}
	}}

	this->updateWidget();
}


void pWindow::setWindowTitle( sf::String& text )
{
	this->titleBarLabel.setNewSymbol( text );
}

sf::String pWindow::getWindowTitle( )
{
	return this->titleBarLabel.fullString;
}

void pWindow::setMovable( bool movable )
{
	this->isMovable = movable;
}

bool pWindow::checkMovable()
{
	return this->isMovable;
}

void pWindow::setToggle( bool toggle )
{
	if(this->toggle != toggle)
	{
		if(this->toggle)
		{
			this->widgetToggledSize = this->widgetSize;
			this->widgetSize = sf::Vector2f(this->widgetSize.x, 40);
			this->toggle = false;
		}
		else{
			this->widgetSize = this->widgetToggledSize;
			this->toggle = true;
		}
	}
}

bool pWindow::checkToggled()
{
	return this->toggle;
}

void pWindow::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(this->visiblity)
	{
		target.draw( this->titleBar );
		target.draw( this->titleBarLabel );
		if(this->toggle)
			target.draw( this->widgetShape );
		target.draw( this->toggleButton );

		if(this->toggle)
		{
			for(unsigned int i = 0; i < this->widgetsList.size(); i++)
			{
				if(this->widgetsList[i]->visiblity)
					target.draw( *this->widgetsList[i] );
			}

			if(this->guiHandler->windowOnTop->widgetId != this->widgetId)
				target.draw( this->notActiveMask );
		}
	}
}

//#####################################################################################################################

pText::pText(float marginLeft, float marginRight, bool textCentered, int cutDirection)
	: marginLeft( marginLeft ), marginRight( marginRight ), textCentered( textCentered ), cutDirection( cutDirection )
{
	this->setCharacterSize( 15 );
}

void pText::setParentGeom(sf::Vector2f position, sf::Vector2f size)
{
	this->parentPos = position;
	this->parentSize = size;
	this->visibleString = this->fullString;
	this->setString( this->visibleString );
	this->updateText();
}

void pText::addSymbol( sf::String& symbol )
{
	this->fullString += symbol;
	this->visibleString += symbol;
	this->setString( this->visibleString );
	this->updateText();
}

void pText::setNewSymbol( sf::String& symbol )
{
	this->fullString = symbol;
	this->visibleString = symbol;
	this->setString( this->visibleString );
	this->updateText();
}

void pText::removeLastSymbol()
{
	if(this->visibleString.getSize() > 0)
	{
		sf::String tmp1 = this->visibleString;

		if(this->fullString.getSize() > this->visibleString.getSize())
		{
			size_t lnumber = tmp1.getSize();
			
			tmp1.erase( tmp1.getSize() - 1, 1 );
			
			sf::String tmp2 = this->fullString;
			tmp2.erase( tmp2.getSize() - lnumber, lnumber );
			tmp2.erase( 0, tmp2.getSize() - 1 );

			tmp1.insert( 0, tmp2 );
		}else
		{
			tmp1.erase( tmp1.getSize() - 1, 1 );
		}

		this->fullString.erase(this->fullString.getSize()-1, 1);
		this->visibleString = tmp1;
		this->setString( this->visibleString );
		this->updateText();
	}
}

void pText::updateText()
{
	while(this->getGlobalBounds().width > (this->parentSize.x-(this->marginLeft+this->marginRight)))
	{
		if(this->cutDirection == 0)
			this->visibleString.erase(0, 1);
		else
			this->visibleString.erase(this->visibleString.getSize()-1, 1);
		
		this->setString( visibleString );
	}

	if(this->textCentered)
		this->setPosition(this->parentPos.x+((this->parentSize.x-this->getGlobalBounds().width)/2), this->parentPos.y+((this->parentSize.y-this->getGlobalBounds().height)/2));
	else
		this->setPosition(this->parentPos.x+this->marginLeft, this->parentPos.y+((parentSize.y-this->getGlobalBounds().height)/2));
}