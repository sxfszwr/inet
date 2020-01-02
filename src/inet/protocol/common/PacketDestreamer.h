//
// Copyright (C) OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see http://www.gnu.org/licenses/.
//

#ifndef __INET_PACKETDESTREAMER_H
#define __INET_PACKETDESTREAMER_H

#include "inet/queueing/base/PacketFlowBase.h"

namespace inet {

using namespace inet::queueing;

class INET_API PacketDestreamer : public PacketFlowBase
{
  protected:
    virtual void processPacket(Packet *packet) override { }

  public:
    virtual Packet *pullPacket(cGate *gate = nullptr) override;
    virtual Packet *pullPacketStart(cGate *gate = nullptr) override;
    virtual Packet *pullPacketEnd(cGate *gate = nullptr) override;
    virtual Packet *pullPacketProgress(b& position, b& extraProcessableLength, cGate *gate = nullptr) override;
    virtual b getPulledPacketProcessedLength(Packet *packet, cGate *gate = nullptr) override;
    virtual void handlePullPacketConfirmation(Packet *packet, cGate *gate, bool successful) override;
};

} // namespace inet

#endif // ifndef __INET_PACKETDESTREAMER_H

