/* This file is part of the KDE libraries
   SPDX-FileCopyrightText: 2001, 2002, 2007 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "karchive_global.h"
#include <QtCore/qdebug.h>
#include <QtCore/qiodevice.h>

/**
 * A readonly device that reads from an underlying device
 * from a given point to another (e.g. to give access to a single
 * file inside an archive).
 * @author David Faure <faure@kde.org>
 * @internal - used by KArchive
 */
class KLimitedIODevice : public QIODevice
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(KLimitedIODevice)
public:
    /**
     * Creates a new KLimitedIODevice.
     * @param dev the underlying device, opened or not
     * This device itself auto-opens (in readonly mode), no need to open it.
     * @param start where to start reading (position in bytes)
     * @param length the length of the data to read (in bytes)
     */
    explicit KLimitedIODevice(QIODevice *dev, qint64 start, qint64 length);
    virtual ~KLimitedIODevice()
    {
    }

    bool isSequential() const override;

    bool open(QIODevice::OpenMode m) override;
    void close() override;

    qint64 size() const override;

    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *, qint64) override
    {
        return -1; // unsupported
    }

    // virtual qint64 pos() const { return m_dev->pos() - m_start; }
    bool seek(qint64 pos) override;
    qint64 bytesAvailable() const override;

private:
    QIODevice *m_dev;
    qint64 m_start;
    qint64 m_length;
};
