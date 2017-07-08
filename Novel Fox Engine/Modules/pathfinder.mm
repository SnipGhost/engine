//-----------------------------------------------------------------------------
// pathfinder.mm
//                            Реализация функции поиска пути до файлов-ресурсов
//-----------------------------------------------------------------------------
#include "pathfinder.hpp"
#import <Foundation/Foundation.h>
//-----------------------------------------------------------------------------
std::string findPath()
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    std::string rpath;
    NSBundle* bundle = [NSBundle mainBundle];

    if (bundle == nil)
    {
        NSLog(@"Bundle is nil. No resources path can be found.");
    }
    else
    {
        NSString* path = [bundle resourcePath];
        //path = [path substringToIndex:[path length] - 5];
        //rpath = [path UTF8String] + std::string("Resources/");
        rpath = [path UTF8String] + std::string("/");
    }
    
    [pool drain];
    return rpath;
}
//-----------------------------------------------------------------------------
