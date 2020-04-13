#ifndef IMAGEINFO_H
#define IMAGEINFO_H
#include <string>

class imageInfo
{
    std::string imageName;
    std::string imageDesc;
    std::string imagePath;
public:
    imageInfo(std::string newImageName,std::string newImageDesc,std::string newImagePath);
    std::string getName();
    std::string getDesc();
    std::string getPath();
};

#endif // IMAGEINFO_H
