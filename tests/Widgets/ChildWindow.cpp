/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus's Graphical User Interface
// Copyright (C) 2012-2015 Bruno Van de Velde (vdv_b@tgui.eu)
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

#include "../Tests.hpp"
#include <TGUI/Widgets/ChildWindow.hpp>

TEST_CASE("[ChildWindow]") {
    tgui::ChildWindow::Ptr childWindow = std::make_shared<tgui::ChildWindow>();

    SECTION("Signals") {
        REQUIRE_NOTHROW(childWindow->connect("MousePressed", [](){}));
        REQUIRE_NOTHROW(childWindow->connect("Closed", [](){}));

        REQUIRE_NOTHROW(childWindow->connect("MousePressed", [](sf::Vector2f){}));
        REQUIRE_NOTHROW(childWindow->connect("Closed", [](tgui::ChildWindow::Ptr){}));
    }

    SECTION("WidgetType") {
        REQUIRE(childWindow->getWidgetType() == "ChildWindow");
    }

    /// TODO: Test the functions in the ChildWindow class

    SECTION("Renderer") {
        auto renderer = childWindow->getRenderer();

        SECTION("colored") {
            SECTION("set serialized property") {
                REQUIRE_NOTHROW(renderer->setProperty("BackgroundColor", "rgb(10, 20, 30)"));
                REQUIRE_NOTHROW(renderer->setProperty("TitleColor", "rgb(40, 50, 60)"));
                REQUIRE_NOTHROW(renderer->setProperty("TitleBarColor", "rgb(70, 80, 90)"));
                REQUIRE_NOTHROW(renderer->setProperty("BorderColor", "rgb(100, 110, 120)"));
                REQUIRE_NOTHROW(renderer->setProperty("Borders", "(1, 2, 3, 4)"));
                REQUIRE_NOTHROW(renderer->setProperty("DistanceToSide", "2"));
                REQUIRE_NOTHROW(renderer->setProperty("TitleBarHeight", "25"));
                REQUIRE_NOTHROW(renderer->setProperty("CloseButton", "default"));
            }
            
            SECTION("set object property") {
                REQUIRE_NOTHROW(renderer->setProperty("BackgroundColor", sf::Color{10, 20, 30}));
                REQUIRE_NOTHROW(renderer->setProperty("TitleColor", sf::Color{40, 50, 60}));
                REQUIRE_NOTHROW(renderer->setProperty("TitleBarColor", sf::Color{70, 80, 90}));
                REQUIRE_NOTHROW(renderer->setProperty("BorderColor", sf::Color{100, 110, 120}));
                REQUIRE_NOTHROW(renderer->setProperty("Borders", tgui::Borders{1, 2, 3, 4}));
                REQUIRE_NOTHROW(renderer->setProperty("DistanceToSide", 2));
                REQUIRE_NOTHROW(renderer->setProperty("TitleBarHeight", 25));
                REQUIRE_NOTHROW(renderer->setProperty("CloseButton", tgui::ObjectConverter("")));
            }

            SECTION("functions") {
                renderer->setBackgroundColor({10, 20, 30});
                renderer->setTitleColor({40, 50, 60});
                renderer->setTitleBarColor({70, 80, 90});
                renderer->setBorderColor({100, 110, 120});
                renderer->setBorders({1, 2, 3, 4});
                renderer->setDistanceToSide(2);
                renderer->setTitleBarHeight(25);

                SECTION("getPropertyValuePairs") {
                    auto pairs = renderer->getPropertyValuePairs();
                    REQUIRE(pairs.size() == 7);
                    REQUIRE(pairs["BackgroundColor"].getColor() == sf::Color(10, 20, 30));
                    REQUIRE(pairs["TitleColor"].getColor() == sf::Color(40, 50, 60));
                    REQUIRE(pairs["TitleBarColor"].getColor() == sf::Color(70, 80, 90));
                    REQUIRE(pairs["BorderColor"].getColor() == sf::Color(100, 110, 120));
                    REQUIRE(pairs["Borders"].getBorders() == tgui::Borders(1, 2, 3, 4));
                    REQUIRE(pairs["DistanceToSide"].getNumber() == 2);
                    REQUIRE(pairs["TitleBarHeight"].getNumber() == 25);
                }
            }

            REQUIRE(renderer->getProperty("BackgroundColor").getColor() == sf::Color(10, 20, 30));
            REQUIRE(renderer->getProperty("TitleColor").getColor() == sf::Color(40, 50, 60));
            REQUIRE(renderer->getProperty("TitleBarColor").getColor() == sf::Color(70, 80, 90));
            REQUIRE(renderer->getProperty("BorderColor").getColor() == sf::Color(100, 110, 120));
            REQUIRE(renderer->getProperty("Borders").getBorders() == tgui::Borders(1, 2, 3, 4));
            REQUIRE(renderer->getProperty("DistanceToSide").getNumber() == 2);
            REQUIRE(renderer->getProperty("TitleBarHeight").getNumber() == 25);
            REQUIRE(renderer->getProperty("CloseButton").getString() == "");
        }

        SECTION("textured") {
            tgui::Texture textureTitleBar("resources/Black.png", {48, 154, 75, 25}, {16, 16, 16, 16});

            REQUIRE(!renderer->getProperty("TitleBarImage").getTexture().isLoaded());

            SECTION("set serialized property") {
                REQUIRE_NOTHROW(renderer->setProperty("TitleBarImage", tgui::Serializer::serialize(textureTitleBar)));
            }

            SECTION("set object property") {
                REQUIRE_NOTHROW(renderer->setProperty("TitleBarImage", textureTitleBar));
            }

            SECTION("functions") {
                renderer->setTitleBarTexture(textureTitleBar);

                SECTION("getPropertyValuePairs") {
                    auto pairs = renderer->getPropertyValuePairs();
                    REQUIRE(pairs.size() == 7);
                    REQUIRE(pairs["TitleBarImage"].getTexture().getData() == textureTitleBar.getData());
                }
            }

            REQUIRE(renderer->getProperty("TitleBarImage").getTexture().isLoaded());

            REQUIRE(renderer->getProperty("TitleBarImage").getTexture().getData() == textureTitleBar.getData());
        }
    }

    SECTION("Saving and loading from file") {
        REQUIRE_NOTHROW(childWindow = std::make_shared<tgui::Theme>()->load("ChildWindow"));

        auto theme = std::make_shared<tgui::Theme>("resources/Black.txt");
        REQUIRE_NOTHROW(childWindow = theme->load("ChildWindow"));
        REQUIRE(childWindow->getPrimaryLoadingParameter() == "resources/Black.txt");
        REQUIRE(childWindow->getSecondaryLoadingParameter() == "childwindow");

        auto parent = std::make_shared<tgui::GuiContainer>();
        parent->add(childWindow);
        
        childWindow->setTitle("Title");
        childWindow->setTitleAlignment(tgui::ChildWindow::TitleAlignment::Left);
        childWindow->setIcon({"resources/image.png"});
        childWindow->keepInParent();

        REQUIRE_NOTHROW(parent->saveWidgetsToFile("WidgetFileChildWindow1.txt"));
        
        parent->removeAllWidgets();
        REQUIRE_NOTHROW(parent->loadWidgetsFromFile("WidgetFileChildWindow1.txt"));

        REQUIRE_NOTHROW(parent->saveWidgetsToFile("WidgetFileChildWindow2.txt"));
        REQUIRE(compareFiles("WidgetFileChildWindow1.txt", "WidgetFileChildWindow2.txt"));
    }
}
