/* This file is part of the KDE libraries
   SPDX-FileCopyrightText: 2014 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "krcc.h"
#include "karchive_p.h"

#include <QtCore/qdatetime.h>
#include <QtCore/qdebug.h>
#include <QtCore/qdir.h>
#include <QtCore/qfile.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qresource.h>
#include <QtCore/quuid.h>

class Q_DECL_HIDDEN KRcc::KRccPrivate
{
    Q_DISABLE_COPY_MOVE(KRccPrivate)
public:
    explicit KRccPrivate()
    {
    }
    void createEntries(const QDir &dir, KArchiveDirectory *parentDir, KRcc *q);

    QString m_prefix; // '/' + uuid
};

/**
 * A KRccFileEntry represents a file in a rcc archive.
 */
class KRccFileEntry : public KArchiveFile
{
    Q_DISABLE_COPY_MOVE(KRccFileEntry)
public:
    explicit KRccFileEntry(KArchive *archive,
                  const QString &name,
                  int access,
                  const QDateTime &date,
                  const QString &user,
                  const QString &group,
                  qint64 size,
                  const QString &resourcePath)
        : KArchiveFile(archive, name, access, date, user, group, QString(), 0, size)
        , m_resourcePath(resourcePath)
    {
    }

    QByteArray data() const override
    {
        QFile f(m_resourcePath);
        if (f.open(QIODevice::ReadOnly)) {
            return f.readAll();
        }
        qCWarning(KArchiveLog) << "Couldn't open" << m_resourcePath;
        return QByteArray();
    }
    QIODevice *createDevice() const override
    {
        return new QFile(m_resourcePath);
    }

private:
    QString m_resourcePath;
};

KRcc::KRcc(const QString &filename)
    : KArchive(filename)
    , d(new KRccPrivate)
{
}

KRcc::~KRcc()
{
    if (isOpen()) {
        close();
    }
    delete d;
}

bool KRcc::doPrepareWriting(const QString &, const QString &, const QString &, qint64, mode_t, const QDateTime &, const QDateTime &, const QDateTime &)
{
    setErrorString(tr("Cannot write to RCC file"));
    qCWarning(KArchiveLog) << "doPrepareWriting not implemented for KRcc";
    return false;
}

bool KRcc::doFinishWriting(qint64)
{
    setErrorString(tr("Cannot write to RCC file"));
    qCWarning(KArchiveLog) << "doFinishWriting not implemented for KRcc";
    return false;
}

bool KRcc::doWriteDir(const QString &, const QString &, const QString &, mode_t, const QDateTime &, const QDateTime &, const QDateTime &)
{
    setErrorString(tr("Cannot write to RCC file"));
    qCWarning(KArchiveLog) << "doWriteDir not implemented for KRcc";
    return false;
}

bool KRcc::doWriteSymLink(const QString &, const QString &, const QString &, const QString &, mode_t, const QDateTime &, const QDateTime &, const QDateTime &)
{
    setErrorString(tr("Cannot write to RCC file"));
    qCWarning(KArchiveLog) << "doWriteSymLink not implemented for KRcc";
    return false;
}

bool KRcc::openArchive(QIODevice::OpenMode mode)
{
    // Open archive

    if (mode == QIODevice::WriteOnly) {
        return true;
    }
    if (mode != QIODevice::ReadOnly && mode != QIODevice::ReadWrite) {
        setErrorString(tr("Unsupported mode %1").arg(mode));
        return false;
    }

    QUuid uuid = QUuid::createUuid();
    d->m_prefix = u'/' + uuid.toString();
    if (!QResource::registerResource(fileName(), d->m_prefix)) {
        setErrorString(tr("Failed to register resource %1 under prefix %2").arg(fileName(), d->m_prefix));
        return false;
    }

    QDir dir(u':' + d->m_prefix);
    d->createEntries(dir, rootDir(), this);
    return true;
}

void KRcc::KRccPrivate::createEntries(const QDir &dir, KArchiveDirectory *parentDir, KRcc *q)
{
    for (const QString &fileName : dir.entryList()) {
        const QString entryPath = dir.path() + u'/' + fileName;
        const QFileInfo info(entryPath);
        if (info.isFile()) {
            KArchiveEntry *entry = new KRccFileEntry(q, fileName, 0444, info.lastModified(), parentDir->user(), parentDir->group(), info.size(), entryPath);
            parentDir->addEntry(entry);
        } else {
            KArchiveDirectory *entry =
                new KArchiveDirectory(q, fileName, 0555, info.lastModified(), parentDir->user(), parentDir->group(), /*symlink*/ QString());
            if (parentDir->addEntryV2(entry)) {
                createEntries(QDir(entryPath), entry, q);
            }
        }
    }
}

bool KRcc::closeArchive()
{
    // Close the archive
    QResource::unregisterResource(fileName(), d->m_prefix);
    // ignore errors
    return true;
}

void KRcc::virtual_hook(int id, void *data)
{
    KArchive::virtual_hook(id, data);
}
