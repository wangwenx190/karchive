#pragma once

#include <QtCore/qglobal.h>

#ifndef KARCHIVE_EXPORT
#ifdef KARCHIVE_STATIC
#define KARCHIVE_EXPORT
#else
#ifdef KARCHIVE_LIBRARY
#define KARCHIVE_EXPORT Q_DECL_EXPORT
#else
#define KARCHIVE_EXPORT Q_DECL_IMPORT
#endif
#endif
#endif