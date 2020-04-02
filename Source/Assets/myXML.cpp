//
// Created by dominik on 21.06.19.
//

#include <TinyXML2/tinyxml2.h>
#include "Assets/Textures/MaterialsLoader.h"
#include "myXML.h"

std::string elementTextToString(tinyxml2::XMLElement *xmlElement) {
    using namespace tinyxml2;
    const char* elementText = xmlElement->GetText();
    if(elementText == nullptr){
        return "";
    }
    return elementText;
}