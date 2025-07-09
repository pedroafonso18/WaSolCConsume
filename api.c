#include "api.h"
#include <curl/curl.h>
#include <string.h>

bool make_request(Request *request, Arena* arena) {
    CURLcode res;
    LogInfo("Making request for: %s", request->url.data);
    CURL *curl = curl_easy_init();
    if (!curl) {
        LogError("Couldn't begin curl, ending...");
        return false;
    }

    struct curl_slist *headers = nullptr;
    VecForEach(request->headers, header) {
        String header_str = F(arena, "%s: %s", header->key.data, header->value.data);
        headers = curl_slist_append(headers, header_str.data);
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    char *json_body = nullptr;
    if (request->body.type == cJSON_Object || request->body.type == cJSON_Array) {
        json_body = cJSON_PrintUnformatted(&request->body);
    }
    curl_easy_setopt(curl, CURLOPT_URL, request->url.data);
    if (json_body && json_body[0] != '\0') {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_body);
    }
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, request->method.data);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        LogError("curl_easy_perform() failed: %s", curl_easy_strerror(res));
        if (json_body) free(json_body);
        if (headers) curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        return false;
    }
    if (json_body) free(json_body);
    if (headers) curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return true;

}