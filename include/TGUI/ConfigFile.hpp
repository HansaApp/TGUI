/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus's Graphical User Interface
// Copyright (C) 2012-2014 Bruno Van de Velde (vdv_b@tgui.eu)
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


#ifndef TGUI_CONFIG_FILE_HPP
#define TGUI_CONFIG_FILE_HPP


#include <TGUI/Global.hpp>

#include <fstream>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \internal
    // Reads the config files that are used to load widgets.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TGUI_API ConfigFile : public sf::NonCopyable
    {
      public:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Cpen and read the properties inside a section of the given file.
        // An exception will be thrown when the file could not be opened or when the section could not be read.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ConfigFile(const std::string& filename, const std::string& section);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Returns the list of properties and their values.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const std::vector<std::pair<std::string, std::string>>& getProperties() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Reads a bool from a string value.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool readBool(std::vector<std::pair<std::string, std::string>>::const_iterator it) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Reads a color from a string value.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        sf::Color readColor(std::vector<std::pair<std::string, std::string>>::const_iterator it) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Loads a texture based on the contents of the string value.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void readTexture(std::vector<std::pair<std::string, std::string>>::const_iterator it, const std::string& rootPath, Texture& texture) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      private:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Reads an int rectangle from a string value.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool readIntRect(std::string value, sf::IntRect& rect) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Remove the whitespace from the line, starting from the character c.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool removeWhitespace(const std::string& line, std::string::const_iterator& c) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Reads the next word, starting from character c.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::string readWord(const std::string& line, std::string::const_iterator& c) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Check if line is a section header.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool isSection(const std::string& line, std::string::const_iterator c, std::string& sectionName) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      private:

        std::string m_filename;
        std::string m_section;
        std::vector<std::pair<std::string, std::string>> m_properties;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_CONFIG_FILE_HPP

