#ifndef SPRITESHEETLIB_GLOBAL_H
#define SPRITESHEETLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SPRITESHEETLIB_LIBRARY)
#  define SPRITESHEETLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SPRITESHEETLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SPRITESHEETLIB_GLOBAL_H
