#pragma once

#include <string>
#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif

void printVersion();

struct Version
{
    static const std::string GIT_SHA1;
    static const std::string GIT_DATE;
    static const std::string GIT_COMMIT_SUBJECT;
};


#ifdef __cplusplus
}
#endif
