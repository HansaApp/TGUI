/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus's Graphical User Interface
// Copyright (C) 2012 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Objects.hpp>
#include <TGUI/MenuBar.hpp>

#include <SFML/OpenGL.hpp>
#include <cmath>

///!!!  TODO: Use images instead of a simple color

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    MenuBar::MenuBar() :
    m_VisibleMenu        (-1),
    m_TextFont           (NULL),
    m_DistanceToSide     (4),
    m_MinimumSubMenuWidth(125)
    {
        m_Callback.objectType = Type_MenuBar;

        changeColors();

        m_Loaded = true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    MenuBar* MenuBar::clone()
    {
        return new MenuBar(*this);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool MenuBar::load(const std::string& configFileFilename)
    {
        // Open the config file
        ConfigFile configFile;
        if (!configFile.open(configFileFilename))
        {
            TGUI_OUTPUT("TGUI error: Failed to open " + configFileFilename + ".");
            return false;
        }

        // Read the properties and their values (as strings)
        std::vector<std::string> properties;
        std::vector<std::string> values;
        if (!configFile.read("MenuBar", properties, values))
        {
            TGUI_OUTPUT("TGUI error: Failed to parse " + configFileFilename + ".");
            return false;
        }

        // Close the config file
        configFile.close();

        // Handle the read properties
        for (unsigned int i = 0; i < properties.size(); ++i)
        {
            std::string property = properties[i];
            std::string value = values[i];

            if (property == "backgroundcolor")
            {
                setBackgroundColor(extractColor(value));
            }
            else if (property == "textcolor")
            {
                setTextColor(extractColor(value));
            }
            else if (property == "selectedbackgroundcolor")
            {
                setSelectedBackgroundColor(extractColor(value));
            }
            else if (property == "selectedtextcolor")
            {
                setSelectedTextColor(extractColor(value));
            }
            else if (property == "distancetoside")
            {
                setDistanceToSide(static_cast<unsigned int>(atoi(value.c_str())));
            }
            else
                TGUI_OUTPUT("TGUI warning: Unrecognized property '" + property + "' in section MenuBar in " + configFileFilename + ".");
        }

        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::setHeight(float height)
    {
        m_Size.y = height;
        setTextSize(static_cast<unsigned int>(height * 0.85f));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Vector2f MenuBar::getSize() const
    {
        return m_Size;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::addMenu(const sf::String& text)
    {
        Menu menu;

        menu.selectedMenuItem = -1;

        menu.text.setFont(*m_TextFont);
        menu.text.setString(text);
        menu.text.setColor(m_TextColor);
        menu.text.setCharacterSize(m_TextSize);
        menu.text.setCharacterSize(static_cast<unsigned int>(menu.text.getCharacterSize() - menu.text.getLocalBounds().top));

        m_Menus.push_back(menu);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool MenuBar::addMenuItem(const sf::String& menu, const sf::String& text)
    {
        // Search for the menu
        for (unsigned int i = 0; i < m_Menus.size(); ++i)
        {
            // If this is the menu then add the menu item to it
            if (m_Menus[i].text.getString() == menu)
            {
                sf::Text menuItem;
                menuItem.setFont(*m_TextFont);
                menuItem.setString(text);
                menuItem.setColor(m_TextColor);
                menuItem.setCharacterSize(m_TextSize);
                menuItem.setCharacterSize(static_cast<unsigned int>(menuItem.getCharacterSize() - menuItem.getLocalBounds().top));

                m_Menus[i].menuItems.push_back(menuItem);
                return true;
            }
        }

        // Couldn't find the menu
        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool MenuBar::removeMenu(const sf::String& menu)
    {
        // Search for the menu
        for (unsigned int i = 0; i < m_Menus.size(); ++i)
        {
            // If this is the menu then remove it
            if (m_Menus[i].text.getString() == menu)
            {
                m_Menus.erase(m_Menus.begin() + i);
                return true;
            }
        }

        // Couldn't find the menu
        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool MenuBar::removeMenuItem(const sf::String& menu, const sf::String& menuItem)
    {
        // Search for the menu
        for (unsigned int i = 0; i < m_Menus.size(); ++i)
        {
            // If this is the menu then search for the menu item
            if (m_Menus[i].text.getString() == menu)
            {
                for (unsigned int j = 0; j < m_Menus[i].menuItems.size(); ++j)
                {
                    // If this is the menu item then remove it
                    if (m_Menus[i].menuItems[j].getString() == menuItem)
                    {
                        m_Menus[i].menuItems.erase(m_Menus[i].menuItems.begin() + j);
                        return true;
                    }
                }
            }
        }

        // Couldn't find menu item
        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::changeColors(const sf::Color& backgroundColor, const sf::Color& textColor,
                               const sf::Color& selectedBackgroundColor, const sf::Color& selectedTextColor)
    {
        m_BackgroundColor = backgroundColor;
        m_TextColor = textColor;
        m_SelectedBackgroundColor = selectedBackgroundColor;
        m_SelectedTextColor = selectedTextColor;

        for (unsigned int i = 0; i < m_Menus.size(); ++i)
        {
            for (unsigned int j = 0; j < m_Menus[i].menuItems.size(); ++j)
            {
                if (m_Menus[i].selectedMenuItem == static_cast<int>(j))
                    m_Menus[i].menuItems[j].setColor(selectedTextColor);
                else
                    m_Menus[i].menuItems[j].setColor(textColor);
            }

            m_Menus[i].text.setColor(textColor);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::setBackgroundColor(const sf::Color& backgroundColor)
    {
        m_BackgroundColor = backgroundColor;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::setTextColor(const sf::Color& textColor)
    {
        m_TextColor = textColor;

        for (unsigned int i = 0; i < m_Menus.size(); ++i)
        {
            for (unsigned int j = 0; j < m_Menus[i].menuItems.size(); ++j)
            {
                if (m_Menus[i].selectedMenuItem != static_cast<int>(j))
                    m_Menus[i].menuItems[j].setColor(textColor);
            }

            m_Menus[i].text.setColor(textColor);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::setSelectedBackgroundColor(const sf::Color& selectedBackgroundColor)
    {
        m_SelectedBackgroundColor = selectedBackgroundColor;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::setSelectedTextColor(const sf::Color& selectedTextColor)
    {
        m_SelectedTextColor = selectedTextColor;

        for (unsigned int i = 0; i < m_Menus.size(); ++i)
        {
            for (unsigned int j = 0; j < m_Menus[i].menuItems.size(); ++j)
            {
                if (m_Menus[i].selectedMenuItem == static_cast<int>(j))
                    m_Menus[i].menuItems[j].setColor(selectedTextColor);
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const sf::Color& MenuBar::getBackgroundColor() const
    {
        return m_BackgroundColor;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const sf::Color& MenuBar::getTextColor() const
    {
        return m_TextColor;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const sf::Color& MenuBar::getSelectedBackgroundColor() const
    {
        return m_SelectedBackgroundColor;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const sf::Color& MenuBar::getSelectedTextColor() const
    {
        return m_SelectedTextColor;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::setTextFont(const sf::Font& font)
    {
        m_TextFont = &font;

        for (unsigned int i = 0; i < m_Menus.size(); ++i)
        {
            for (unsigned int j = 0; j < m_Menus[i].menuItems.size(); ++j)
            {
                m_Menus[i].menuItems[j].setFont(font);
            }

            m_Menus[i].text.setFont(font);
        }

        setTextSize(m_TextSize);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const sf::Font* MenuBar::getTextFont() const
    {
        return m_TextFont;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::setTextSize(unsigned int size)
    {
        m_TextSize = size;

        for (unsigned int i = 0; i < m_Menus.size(); ++i)
        {
            for (unsigned int j = 0; j < m_Menus[i].menuItems.size(); ++j)
            {
                m_Menus[i].menuItems[j].setCharacterSize(m_TextSize);
                m_Menus[i].menuItems[j].setCharacterSize(static_cast<unsigned int>(m_Menus[i].menuItems[j].getCharacterSize() - m_Menus[i].menuItems[j].getLocalBounds().top));
            }

            m_Menus[i].text.setCharacterSize(m_TextSize);
            m_Menus[i].text.setCharacterSize(static_cast<unsigned int>(m_Menus[i].text.getCharacterSize() - m_Menus[i].text.getLocalBounds().top));
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int MenuBar::getTextSize() const
    {
        return m_TextSize;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::setDistanceToSide(unsigned int distanceToSide)
    {
        m_DistanceToSide = distanceToSide;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int MenuBar::getDistanceToSide() const
    {
        return m_DistanceToSide;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::setMinimumSubMenuWidth(unsigned int minimumWidth)
    {
        m_MinimumSubMenuWidth = minimumWidth;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int MenuBar::getMinimumSubMenuWidth() const
    {
        return m_MinimumSubMenuWidth;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool MenuBar::mouseOnObject(float x, float y)
    {
        if (m_Loaded)
        {
            // Check if the mouse is on top of the menu bar
            if (getTransform().transformRect(sf::FloatRect(0, 0, m_Size.x, m_Size.y)).contains(x, y))
                return true;
            else
            {
                // Check if there is a menu open
                if (m_VisibleMenu != -1)
                {
                    // Search the left position of the open menu
                    float left = 0;
                    for (int i = 0; i < m_VisibleMenu; ++i)
                        left += m_Menus[i].text.getLocalBounds().width + (2 * m_DistanceToSide);

                    // Find out what the width of the menu should be
                    float width = 0;
                    for (unsigned int j = 0; j < m_Menus[m_VisibleMenu].menuItems.size(); ++j)
                    {
                        if (width < m_Menus[m_VisibleMenu].menuItems[j].getLocalBounds().width + (3 * m_DistanceToSide))
                            width = m_Menus[m_VisibleMenu].menuItems[j].getLocalBounds().width + (3 * m_DistanceToSide);
                    }

                    // There is a minimum width
                    if (width < m_MinimumSubMenuWidth)
                        width = m_MinimumSubMenuWidth;

                    // Check if the mouse is on top of the open menu
                    if (getTransform().transformRect(sf::FloatRect(left, m_Size.y, width, m_Size.y * m_Menus[m_VisibleMenu].menuItems.size())).contains(x, y))
                        return true;
                }
            }
        }

        // Check if there is still a menu open
        if (m_VisibleMenu != -1)
        {
            // If an item in that menu was selected then unselect it first
            if (m_Menus[m_VisibleMenu].selectedMenuItem != -1)
            {
                m_Menus[m_VisibleMenu].menuItems[m_Menus[m_VisibleMenu].selectedMenuItem].setColor(m_TextColor);
                m_Menus[m_VisibleMenu].selectedMenuItem = -1;
            }

            m_VisibleMenu = -1;
        }

        if (m_MouseHover)
            mouseLeftObject();

        m_MouseHover = false;
        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::leftMousePressed(float, float)
    {
        m_MouseDown = true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::leftMouseReleased(float x, float y)
    {
        if (m_MouseDown)
        {
            // Check if the mouse is on top of the menu bar (not on an open menus)
            if (y <= m_Size.y + getPosition().y)
            {
                // Loop through the menus to check if the mouse is on top of them
                float menuWidth = 0;
                for (unsigned int i = 0; i < m_Menus.size(); ++i)
                {
                    menuWidth += m_Menus[i].text.getLocalBounds().width + (2 * m_DistanceToSide);
                    if (x < menuWidth)
                    {
                        // Only give a callback when there is no menu to open
                        if (m_Menus[i].menuItems.empty())
                        {
                            if (m_CallbackFunctions[MenuItemClicked].empty() == false)
                            {
                                m_Callback.trigger = MenuItemClicked;
                                m_Callback.text = m_Menus[i].text.getString();
                                m_Callback.index = i;
                                addCallback();
                            }
                        }

                        break;
                    }
                }
            }
            else // The mouse is on top of one of the menus
            {
                unsigned int selectedMenuItem = static_cast<unsigned int>((y - m_Size.y - getPosition().y) / m_Size.y);

                if (selectedMenuItem < m_Menus[m_VisibleMenu].menuItems.size())
                {
                    if (m_CallbackFunctions[MenuItemClicked].empty() == false)
                    {
                        m_Callback.trigger = MenuItemClicked;
                        m_Callback.text = m_Menus[m_VisibleMenu].menuItems[selectedMenuItem].getString();
                        m_Callback.index = m_VisibleMenu;
                        addCallback();
                    }
                }
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::mouseMoved(float x, float y)
    {
        if (m_MouseHover == false)
            mouseEnteredObject();

        m_MouseHover = true;

        // Check if the mouse is on top of the menu bar (not on an open menus)
        if (y <= m_Size.y + getPosition().y)
        {
            // Loop through the menus to check if the mouse is on top of them
            float menuWidth = 0;
            for (unsigned int i = 0; i < m_Menus.size(); ++i)
            {
                menuWidth += m_Menus[i].text.getLocalBounds().width + (2 * m_DistanceToSide);
                if (x < menuWidth)
                {
                    // Check if the menu is already open
                    if (m_VisibleMenu == static_cast<int>(i))
                    {
                        // If one of the menu items is selected then unselect it
                        if (m_Menus[m_VisibleMenu].selectedMenuItem != -1)
                        {
                            m_Menus[m_VisibleMenu].menuItems[m_Menus[m_VisibleMenu].selectedMenuItem].setColor(m_TextColor);
                            m_Menus[m_VisibleMenu].selectedMenuItem = -1;
                        }
                    }
                    else // The menu isn't open yet
                    {
                        // If there is another menu open then close it first
                        if (m_VisibleMenu != -1)
                        {
                            // If an item in that other menu was selected then unselect it first
                            if (m_Menus[m_VisibleMenu].selectedMenuItem != -1)
                            {
                                m_Menus[m_VisibleMenu].menuItems[m_Menus[m_VisibleMenu].selectedMenuItem].setColor(m_TextColor);
                                m_Menus[m_VisibleMenu].selectedMenuItem = -1;
                            }

                            m_VisibleMenu = -1;
                        }

                        // If this menu can be opened then do so
                        if (!m_Menus[i].menuItems.empty())
                            m_VisibleMenu = static_cast<int>(i);
                    }
                    break;
                }
            }
        }
        else // The mouse is on top of one of the menus
        {
            // Calculate on what menu item the mouse is located
            int selectedMenuItem = static_cast<int>((y - m_Size.y - getPosition().y) / m_Size.y);

            // Check if the mouse is on a different item than before
            if (selectedMenuItem != m_Menus[m_VisibleMenu].selectedMenuItem)
            {
                // If another of the menu items is selected then unselect it
                if (m_Menus[m_VisibleMenu].selectedMenuItem != -1)
                    m_Menus[m_VisibleMenu].menuItems[m_Menus[m_VisibleMenu].selectedMenuItem].setColor(m_TextColor);

                // Mark the item below the mouse as selected
                m_Menus[m_VisibleMenu].selectedMenuItem = selectedMenuItem;
                m_Menus[m_VisibleMenu].menuItems[m_Menus[m_VisibleMenu].selectedMenuItem].setColor(m_SelectedTextColor);
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::initialize(tgui::Group *const parent)
    {
        m_Parent = parent;
        setTextFont(m_Parent->getGlobalFont());
        m_Size.x = m_Parent->getDisplaySize().x;
        m_Size.y = 20;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        // Draw the background
        sf::RectangleShape background(m_Size);
        background.setFillColor(m_BackgroundColor);
        target.draw(background, states);

        // Draw the menus
        for (unsigned int i = 0; i < m_Menus.size(); ++i)
        {
            states.transform.translate(m_DistanceToSide, 0);
            target.draw(m_Menus[i].text, states);

            // Is the menu open?
            if (m_VisibleMenu == static_cast<int>(i))
            {
                states.transform.translate(-static_cast<float>(m_DistanceToSide), m_Size.y);

                // Find out what the width of the menu should be
                float menuWidth = 0;
                for (unsigned int j = 0; j < m_Menus[i].menuItems.size(); ++j)
                {
                    if (menuWidth < m_Menus[i].menuItems[j].getLocalBounds().width + (3 * m_DistanceToSide))
                        menuWidth = m_Menus[i].menuItems[j].getLocalBounds().width + (3 * m_DistanceToSide);
                }

                // There is a minimum width
                if (menuWidth < m_MinimumSubMenuWidth)
                    menuWidth = m_MinimumSubMenuWidth;

                // Draw the background of the menu
                background = sf::RectangleShape(sf::Vector2f(menuWidth, m_Size.y * m_Menus[i].menuItems.size()));
                background.setFillColor(m_BackgroundColor);
                target.draw(background, states);

                // If there is a selected menu item then draw its background
                if (m_Menus[i].selectedMenuItem != -1)
                {
                    states.transform.translate(0, m_Menus[i].selectedMenuItem * m_Size.y);
                    background = sf::RectangleShape(sf::Vector2f(menuWidth, m_Size.y));
                    background.setFillColor(m_SelectedBackgroundColor);
                    target.draw(background, states);
                    states.transform.translate(0, m_Menus[i].selectedMenuItem * -m_Size.y);
                }

                states.transform.translate(2 * m_DistanceToSide, 0);

                // Draw the menu items
                for (unsigned int j = 0; j < m_Menus[i].menuItems.size(); ++j)
                {
                    target.draw(m_Menus[i].menuItems[j], states);
                    states.transform.translate(0, m_Size.y);
                }

                states.transform.translate(m_Menus[i].text.getLocalBounds().width, -m_Size.y * (m_Menus[i].menuItems.size()+1));
            }
            else // The menu isn't open
            {
                states.transform.translate(m_Menus[i].text.getLocalBounds().width + m_DistanceToSide, 0);
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////