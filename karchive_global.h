#pragma once

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

#if defined(Q_OS_WIN) && !defined(Q_OS_WINDOWS)
#define Q_OS_WINDOWS
#endif

#ifndef Q_DISABLE_COPY_MOVE
#define Q_DISABLE_COPY_MOVE(Class) \
    Q_DISABLE_COPY(Class) \
    Class(Class &&) = delete; \
    Class &operator=(Class &&) = delete;
#endif

#if (QT_VERSION < QT_VERSION_CHECK(5, 7, 0))
#define qAsConst(i) std::as_const(i)
#endif

#if (QT_VERSION < QT_VERSION_CHECK(5, 10, 0))
using qsizetype = QIntegerForSizeof<std::size_t>::Signed;
#endif

Q_DECLARE_LOGGING_CATEGORY(KArchiveLog)
