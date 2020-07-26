/* This file is part of the KDE libraries
   SPDX-FileCopyrightText: 2000-2005 David Faure <faure@kde.org>
   SPDX-FileCopyrightText: 2003 Leo Savernik <l.savernik@aon.at>

   SPDX-License-Identifier: LGPL-2.0-only
*/

#pragma once

#include "karchive.h"

#include <QSaveFile>

class KArchivePrivate
{
    Q_DISABLE_COPY_MOVE(KArchivePrivate)
    Q_DECLARE_TR_FUNCTIONS(KArchivePrivate)

public:
    KArchivePrivate(KArchive *parent) : q(parent) {}
    ~KArchivePrivate()
    {
        delete saveFile;
        delete rootDir;
    }

    static bool hasRootDir(KArchive *archive) { return archive->d->rootDir; }

    void abortWriting();

    static QDateTime time_tToDateTime(uint time_t);

    KArchiveDirectory *findOrCreate(const QString &path, int recursionCounter);

    KArchive *q = nullptr;
    KArchiveDirectory *rootDir = nullptr;
    QSaveFile *saveFile = nullptr;
    QIODevice *dev = nullptr;
    QString fileName = {};
    QIODevice::OpenMode mode = QIODevice::NotOpen;
    bool deviceOwned = false; // if true, we (KArchive) own dev and must delete it
    QString errorStr = {tr("Unknown error")};
};
