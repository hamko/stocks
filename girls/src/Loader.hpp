#ifndef __LOADER_HPP_INCLUDED__
#define __LOADER_HPP_INCLUDED__
#include <string>
class Loader {
protected:
    std::string filename;
public:
    virtual void load(std::string database) = 0;
};
#endif
