// Copyright 2011 Johann Duscher. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
//    1. Redistributions of source code must retain the above copyright notice, this list of
//       conditions and the following disclaimer.
//
//    2. Redistributions in binary form must reproduce the above copyright notice, this list
//       of conditions and the following disclaimer in the documentation and/or other materials
//       provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY JOHANN DUSCHER ''AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// The views and conclusions contained in the software and documentation are those of the
// authors and should not be interpreted as representing official policies, either expressed
// or implied, of Johann Duscher.

#ifndef REQREPSERVER_H
#define REQREPSERVER_H

#include <QObject>
#include <QDebug>
#include <QList>
#include <QByteArray>
#include <QTimer>
#include <QDateTime>

#include "nzmqt/nzmqt.hpp"


class ReqRepServer : public QObject
{
    Q_OBJECT

    typedef QObject super;

public:
    explicit ReqRepServer(const QString& address, const QString& replyMsg, QObject* parent)
        : super(parent), replyMsg_(replyMsg)
    {
        nzmqt::ZMQContext* context = new nzmqt::ZMQContext(4, this);
        socket_ = context->createSocket(ZMQ_REP);
        connect(socket_, SIGNAL(readyRead()), SLOT(requestReceived()));
        socket_->bindTo(address);
    }

    void run()
    {
    }

protected slots:
    void requestReceived()
    {
        static quint64 counter = 0;

        QList<QByteArray> request = socket_->receiveMessage();
        qDebug() << "ReqRepServer::requestReceived> " << request;

        QList<QByteArray> reply;
        reply += QString("REPLY[%1: %2]").arg(++counter).arg(QDateTime::currentDateTime().toString(Qt::ISODate)).toLocal8Bit();
        reply += replyMsg_.toLocal8Bit();
        qDebug() << "ReqRepServer::sendReply> " << reply;
        socket_->sendMessage(reply);
    }

private:
    nzmqt::ZMQSocket* socket_;
    QString replyMsg_;
};

#endif // REQREPSERVER_H
