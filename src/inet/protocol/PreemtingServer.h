//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#ifndef __INET_PREEMTINGSERVER_H
#define __INET_PREEMTINGSERVER_H

#include "inet/queueing/base/PacketServerBase.h"

namespace inet {

using namespace inet::queueing;

class INET_API PreemtingServer : public PacketServerBase
{
  protected:
    b roundingLength = b(-1);
    cGate *preemtedOutputGate = nullptr;
    IPassivePacketSink *preemtedConsumer = nullptr;

    Packet *packet = nullptr;

  protected:
    virtual void initialize(int stage) override;

    virtual void startSendingPacket();
    virtual void endSendingPacket();

    virtual int getPriority(Packet *packet) const;

  public:
    virtual bool supportsPushPacket(cGate *gate) const override { return preemtedOutputGate == gate || PacketServerBase::supportsPushPacket(gate); }

    virtual void handleCanPushPacket(cGate *gate) override;
    virtual void handleCanPopPacket(cGate *gate) override;
};

} // namespace inet

#endif // ifndef __INET_PREEMTINGSERVER_H

