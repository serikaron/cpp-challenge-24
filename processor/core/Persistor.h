//
// Created by serika on 2024/3/14.
//

#ifndef PROCESSOR_PERSISTOR_H
#define PROCESSOR_PERSISTOR_H

#include <vector>
#include <string>
#include "Mapping.h"

class Persistor {
public:
    Persistor(std::string &&saveTo,
              SharedMapping mapping
              );

public:
    void save();

private:
    using Content = std::vector<std::string>;
    Content genContent();
    void save(Content &&content);

private:
    std::string saveTo;
    SharedMapping mapping;
};


#endif //PROCESSOR_PERSISTOR_H
