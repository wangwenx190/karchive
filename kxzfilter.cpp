/* This file is part of the KDE libraries
   SPDX-FileCopyrightText: 2007-2008 Per Øyvind Karlsen <peroyvind@mandriva.org>

   Based on kbzip2filter:
   SPDX-FileCopyrightText: 2000-2005 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "kxzfilter.h"

extern "C" {
#include <lzma.h>
}

#include <QtCore/qdebug.h>
#include <QtCore/qiodevice.h>

class Q_DECL_HIDDEN KXzFilter::Private
{
    Q_DISABLE_COPY_MOVE(Private)
public:
    explicit Private()
        : isInitialized(false)
    {
        memset(&zStream, 0, sizeof(zStream));
        mode = 0;
    }

    lzma_stream zStream;
    int mode;
    bool isInitialized;
    KXzFilter::Flag flag;
};

KXzFilter::KXzFilter()
    : d(new Private)
{
}

KXzFilter::~KXzFilter()
{
    delete d;
}

bool KXzFilter::init(int mode)
{
    QVector<unsigned char> props;
    return init(mode, Flag::AUTO, props);
}

static void freeFilters(lzma_filter filters[])
{
    for (int i = 0; filters[i].id != LZMA_VLI_UNKNOWN; i++) {
        free(filters[i].options);
    }
}

bool KXzFilter::init(int mode, Flag flag, const QVector<unsigned char> &properties)
{
    if (d->isInitialized) {
        terminate();
    }

    d->flag = flag;
    lzma_ret result;
    d->zStream.next_in = nullptr;
    d->zStream.avail_in = 0;
    if (mode == QIODevice::ReadOnly) {
        // TODO when we can depend on Qt 5.12 Use a QScopeGuard to call freeFilters
        lzma_filter filters[5];
        filters[0].id = LZMA_VLI_UNKNOWN;

        switch (flag) {
        case Flag::AUTO:
            /* We set the memlimit for decompression to 100MiB which should be
             * more than enough to be sufficient for level 9 which requires 65 MiB.
             */
            result = lzma_auto_decoder(&d->zStream, 100 << 20, 0);
            if (result != LZMA_OK) {
                qCWarning(KArchiveLog) << "lzma_auto_decoder returned" << result;
                return false;
            }
            break;
        case Flag::LZMA: {
            filters[0].id = LZMA_FILTER_LZMA1;
            filters[0].options = nullptr;
            filters[1].id = LZMA_VLI_UNKNOWN;
            filters[1].options = nullptr;

            Q_ASSERT(properties.size() == 5);
            unsigned char props[5];
            for (int i = 0; i < properties.size(); ++i) {
                props[i] = properties[i];
            }

            result = lzma_properties_decode(&filters[0], nullptr, props, sizeof(props));
            if (result != LZMA_OK) {
                qCWarning(KArchiveLog) << "lzma_properties_decode returned" << result;
                freeFilters(filters);
                return false;
            }
            break;
        }
        case Flag::LZMA2: {
            filters[0].id = LZMA_FILTER_LZMA2;
            filters[0].options = nullptr;
            filters[1].id = LZMA_VLI_UNKNOWN;
            filters[1].options = nullptr;

            Q_ASSERT(properties.size() == 1);
            unsigned char props[1];
            props[0] = properties[0];

            result = lzma_properties_decode(&filters[0], nullptr, props, sizeof(props));
            if (result != LZMA_OK) {
                qCWarning(KArchiveLog) << "lzma_properties_decode returned" << result;
                freeFilters(filters);
                return false;
            }
            break;
        }
        case Flag::BCJ: {
            filters[0].id = LZMA_FILTER_X86;
            filters[0].options = nullptr;

            unsigned char props[5] = {0x5d, 0x00, 0x00, 0x08, 0x00};
            filters[1].id = LZMA_FILTER_LZMA1;
            filters[1].options = nullptr;
            result = lzma_properties_decode(&filters[1], nullptr, props, sizeof(props));
            if (result != LZMA_OK) {
                qCWarning(KArchiveLog) << "lzma_properties_decode1 returned" << result;
                freeFilters(filters);
                return false;
            }

            filters[2].id = LZMA_VLI_UNKNOWN;
            filters[2].options = nullptr;

            break;
        }
        case Flag::POWERPC:
        case Flag::IA64:
        case Flag::ARM:
        case Flag::ARMTHUMB:
        case Flag::SPARC:
            // qCDebug(KArchiveLog) << "flag" << flag << "props size" << properties.size();
            break;
        }

        if (flag != Flag::AUTO) {
            result = lzma_raw_decoder(&d->zStream, filters);
            if (result != LZMA_OK) {
                qCWarning(KArchiveLog) << "lzma_raw_decoder returned" << result;
                freeFilters(filters);
                return false;
            }
        }
        freeFilters(filters);

    } else if (mode == QIODevice::WriteOnly) {
        if (flag == Flag::AUTO) {
            result = lzma_easy_encoder(&d->zStream, LZMA_PRESET_DEFAULT, LZMA_CHECK_CRC32);
        } else {
            lzma_filter filters[5];
            if (flag == Flag::LZMA2) {
                lzma_options_lzma lzma_opt;
                lzma_lzma_preset(&lzma_opt, LZMA_PRESET_DEFAULT);

                filters[0].id = LZMA_FILTER_LZMA2;
                filters[0].options = &lzma_opt;
                filters[1].id = LZMA_VLI_UNKNOWN;
                filters[1].options = nullptr;
            }
            result = lzma_raw_encoder(&d->zStream, filters);
        }
        if (result != LZMA_OK) {
            qCWarning(KArchiveLog) << "lzma_easy_encoder returned" << result;
            return false;
        }
    } else {
        // qCWarning(KArchiveLog) << "Unsupported mode " << mode << ". Only QIODevice::ReadOnly and QIODevice::WriteOnly supported";
        return false;
    }
    d->mode = mode;
    d->isInitialized = true;
    return true;
}

int KXzFilter::mode() const
{
    return d->mode;
}

bool KXzFilter::terminate()
{
    if (d->mode == QIODevice::ReadOnly || d->mode == QIODevice::WriteOnly) {
        lzma_end(&d->zStream);
    } else {
        // qCWarning(KArchiveLog) << "Unsupported mode " << d->mode << ". Only QIODevice::ReadOnly and QIODevice::WriteOnly supported";
        return false;
    }
    d->isInitialized = false;
    return true;
}

void KXzFilter::reset()
{
    // qCDebug(KArchiveLog) << "KXzFilter::reset";
    // liblzma doesn't have a reset call...
    terminate();
    init(d->mode);
}

void KXzFilter::setOutBuffer(char *data, uint maxlen)
{
    d->zStream.avail_out = maxlen;
    d->zStream.next_out = (uint8_t *)data;
}

void KXzFilter::setInBuffer(const char *data, unsigned int size)
{
    d->zStream.avail_in = size;
    d->zStream.next_in = (uint8_t *)const_cast<char *>(data);
}

int KXzFilter::inBufferAvailable() const
{
    return d->zStream.avail_in;
}

int KXzFilter::outBufferAvailable() const
{
    return d->zStream.avail_out;
}

KXzFilter::Result KXzFilter::uncompress()
{
    // qCDebug(KArchiveLog) << "Calling lzma_code with avail_in=" << inBufferAvailable() << " avail_out =" << outBufferAvailable();
    lzma_ret result;
    result = lzma_code(&d->zStream, LZMA_RUN);

    /*if (result != LZMA_OK) {
        qCDebug(KArchiveLog) << "lzma_code returned " << result;
        //qCDebug(KArchiveLog) << "KXzFilter::uncompress " << ( result == LZMA_STREAM_END ? KFilterBase::End : KFilterBase::Error );
    }*/

    switch (result) {
    case LZMA_OK:
        return KFilterBase::Result::Ok;
    case LZMA_STREAM_END:
        return KFilterBase::Result::End;
    default:
        return KFilterBase::Result::Error;
    }
}

KXzFilter::Result KXzFilter::compress(bool finish)
{
    // qCDebug(KArchiveLog) << "Calling lzma_code with avail_in=" << inBufferAvailable() << " avail_out=" << outBufferAvailable();
    lzma_ret result = lzma_code(&d->zStream, finish ? LZMA_FINISH : LZMA_RUN);
    switch (result) {
    case LZMA_OK:
        return KFilterBase::Result::Ok;
        break;
    case LZMA_STREAM_END:
        // qCDebug(KArchiveLog) << "  lzma_code returned " << result;
        return KFilterBase::Result::End;
        break;
    default:
        // qCDebug(KArchiveLog) << "  lzma_code returned " << result;
        return KFilterBase::Result::Error;
        break;
    }
}
