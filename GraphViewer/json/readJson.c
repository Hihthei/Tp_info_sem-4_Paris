#include <stdio.h>
#include <stdlib.h>
#include "readJson.h"

const char* read_file(const char* path) {
	FILE* file = fopen(path, "r");
	if (file == NULL) {
		fprintf(stderr, "Expected file \"%s\" not found", path);
		return NULL;
	}
	fseek(file, 0, SEEK_END);
	long len = ftell(file);
	fseek(file, 0, SEEK_SET);
	char* buffer = malloc(len + 1);

	if (buffer == NULL) {
		fprintf(stderr, "Unable to allocate memory for file");
		fclose(file);
		return NULL;
	}

	fread(buffer, 1, len, file);
	buffer[len] = '\0';

	return (const char*)buffer;
}

typed(json_element) interpretJSon(const char* fileName) {
	const char* json = read_file(fileName);
	if (json == NULL) {
		return;
	}

	result(json_element) element_result = json_parse(json);
	free((void*)json);

	if (result_is_err(json_element)(&element_result)) {
		typed(json_error) error = result_unwrap_err(json_element)(&element_result);
		fprintf(stderr, "Error parsing JSON: %s\n", json_error_to_string(error));
		return;
	}
	typed(json_element) element = result_unwrap(json_element)(&element_result);

	/*json_print(&element, 2);*/
	//json_free(&element);
	return element;
}

bool hasElementWithName(typed(json_element) element, const char* nodeName) {
	result(json_element) result = json_object_find(element.value.as_object, nodeName);
	return !(result_is_err(json_element)(&result));
};

typed(json_element) getJsonElementFromName(typed(json_element) element, const char* nodeName)
{
	result(json_element) result = json_object_find(element.value.as_object, nodeName);
	if (result_is_err(json_element)(&result)) {
		typed(json_error) error = result_unwrap_err(json_element)(&result);
		fprintf(stderr, "Error getting element \"hello\": %s\n", json_error_to_string(error));
		return;
	}
	typed(json_element) resultUnwrapped = result_unwrap(json_element)(&result);
	return resultUnwrapped;
}
