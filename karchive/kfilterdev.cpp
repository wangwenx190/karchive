/* This file is part of the KDE libraries
   SPDX-FileCopyrightText: 2000, 2006 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-only
*/

#include "kfilterdev.h"
#include <QMimeDatabase>

#include <QDebug>

static KCompressionDevice::CompressionType findCompressionByFileName(const QString &fileName)
{
    if (fileName.endsWith(QLatin1String(".gz"), Qt::CaseInsensitive)) {
        return KCompressionDevice::GZip;
    }
    if (fileName.endsWith(QLatin1String(".bz2"), Qt::CaseInsensitive)) {
        return KCompressionDevice::BZip2;
    }
    if (fileName.endsWith(QLatin1String(".lzma"), Qt::CaseInsensitive) || fileName.endsWith(QLatin1String(".xz"), Qt::CaseInsensitive)) {
        return KCompressionDevice::Xz;
    }
    else {
        // not a warning, since this is called often with other mimetypes (see #88574)...
        // maybe we can avoid that though?
        //qDebug() << "findCompressionByFileName : no compression found for " << fileName;
    }

    return KCompressionDevice::None;
}

KFilterDev::KFilterDev(const QString &fileName)
    : KCompressionDevice(fileName, findCompressionByFileName(fileName))
{
}

KCompressionDevice::CompressionType KFilterDev::compressionTypeForMimeType(const QString &mimeType)
{
    if (mimeType == QLatin1String("application/x-gzip")) {
        return KCompressionDevice::GZip;
    }
    if (mimeType == QLatin1String("application/x-bzip")
        || mimeType == QLatin1String("application/x-bzip2") // old name, kept for compatibility
       ) {
        return KCompressionDevice::BZip2;
    }
    if (mimeType == QLatin1String("application/x-lzma")    // legacy name, still used
        || mimeType == QLatin1String("application/x-xz")   // current naming
       ) {
        return KCompressionDevice::Xz;
    }
    QMimeDatabase db;
    const QMimeType mime = db.mimeTypeForName(mimeType);
    if (mime.isValid()) {
        if (mime.inherits(QStringLiteral("application/x-gzip"))) {
            return KCompressionDevice::GZip;
        }
        if (mime.inherits(QStringLiteral("application/x-bzip"))) {
            return KCompressionDevice::BZip2;
        }
        if (mime.inherits(QStringLiteral("application/x-lzma"))) {
            return KCompressionDevice::Xz;
        }

        if (mime.inherits(QStringLiteral("application/x-xz"))) {
            return KCompressionDevice::Xz;
        }
    }

    // not a warning, since this is called often with other mimetypes (see #88574)...
    // maybe we can avoid that though?
    //qDebug() << "no compression found for" << mimeType;
    return KCompressionDevice::None;
}
