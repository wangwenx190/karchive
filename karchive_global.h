#ifndef KARCHIVE_GLOBAL_H
#define KARCHIVE_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QtCore/qloggingcategory.h>

#ifndef KARCHIVE_API
#ifdef KARCHIVE_STATIC
#define KARCHIVE_API
#else
#ifdef KARCHIVE_BUILD_LIBRARY
#define KARCHIVE_API Q_DECL_EXPORT
#else
#define KARCHIVE_API Q_DECL_IMPORT
#endif
#endif
#endif

Q_DECLARE_LOGGING_CATEGORY(KArchiveLog)

#endif // KARCHIVE_GLOBAL_H
