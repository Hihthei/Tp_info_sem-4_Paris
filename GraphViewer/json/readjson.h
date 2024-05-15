#pragma once
#include "json.h"

typed(json_element) interpretJSon(const char* fileName);

typed(json_element) getJsonElementFromName(typed(json_element) element, const char* nodeName);

bool hasElementWithName(typed(json_element) element, const char* nodeName);

