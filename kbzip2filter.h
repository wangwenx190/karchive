/* This file is part of the KDE libraries
   SPDX-FileCopyrightText: 2000 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kfilterbase.h"

/**
 * Internal class used by KCompressionDevice
 * @internal
 */
class KBzip2Filter : public KFilterBase
{
    Q_DISABLE_COPY_MOVE(KBzip2Filter)
public:
    explicit KBzip2Filter();
    virtual ~KBzip2Filter();

    bool init(int) override;
    int mode() const override;
    bool terminate() override;
    void reset() override;
    bool readHeader() override
    {
        return true; // bzip2 handles it by itself ! Cool !
    }
    bool writeHeader(const QByteArray &) override
    {
        return true;
    }
    void setOutBuffer(char *data, uint maxlen) override;
    void setInBuffer(const char *data, uint size) override;
    int inBufferAvailable() const override;
    int outBufferAvailable() const override;
    Result uncompress() override;
    Result compress(bool finish) override;

private:
    class Private;
    Private *const d;
};
