#ifndef LOADER_H
#define LOADER_H

#include <string>
#include "memory.h"

class Loader {
    public:
        void load(const std::string& filename, Memory& mem);
};

#endif