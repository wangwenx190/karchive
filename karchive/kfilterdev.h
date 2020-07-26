/* This file is part of the KDE libraries
   SPDX-FileCopyrightText: 2000 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-only
*/
#ifndef __kfilterdev_h
#define __kfilterdev_h

#include <karchive_export.h>
#include <kcompressiondevice.h>
#include <QString>

class QFile;
class KFilterBase;

/**
 * @class KFilterDev kfilterdev.h KFilterDev
 *
 * A class for reading and writing compressed data onto a device
 * (e.g. file, but other usages are possible, like a buffer or a socket).
 *
 * To simply read/write compressed files, see deviceForFile.
 *
 * KFilterDev adds MIME type support to KCompressionDevice, and also
 * provides compatibility methods for KDE 4 code.
 *
 * @author David Faure <faure@kde.org>
 */
class KARCHIVE_EXPORT KFilterDev : public KCompressionDevice
{
    Q_OBJECT
public:
    /**
     * @since 5.0
     * Constructs a KFilterDev for a given FileName.
     * @param fileName the name of the file to filter.
     */
    KFilterDev(const QString &fileName);

    /**
     * Returns the compression type for the given mimetype, if possible. Otherwise returns None.
     * This handles simple cases like application/x-gzip, but also application/x-compressed-tar, and inheritance.
     */
    static CompressionType compressionTypeForMimeType(const QString &mimetype);
};

#endif
