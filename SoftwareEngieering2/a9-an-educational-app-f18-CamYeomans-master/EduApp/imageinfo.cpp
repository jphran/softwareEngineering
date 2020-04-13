#include "imageinfo.h"

imageInfo::imageInfo(std::string newImageName,std::string newImageDesc,std::string newImagePath)
{
 imageName = newImageName;
 imageDesc = newImageDesc;
 imagePath = newImagePath;
}

std::string imageInfo::getName()
{
    return imageName;
}

std::string imageInfo::getDesc()
{
    return imageDesc;
}
std::string imageInfo::getPath()
{
    return imagePath;
}
