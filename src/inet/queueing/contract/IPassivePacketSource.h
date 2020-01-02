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

#ifndef __INET_IPASSIVEPACKETSOURCE_H
#define __INET_IPASSIVEPACKETSOURCE_H

#include "inet/common/packet/Packet.h"

namespace inet {
namespace queueing {

/**
 * This class defines the interface for passive packet sources.
 * See the corresponding NED file for more details.
 */
class INET_API IPassivePacketSource
{
  public:
    enum CanPullSomePacketResult {
        PULL_NONE = 0,
        PULL_SOME = 1,
        PULL_ANY  = 2,
    };

  public:
    virtual ~IPassivePacketSource() {}

    /**
     * Returns false if the packet source is empty at the given gate and no more
     * packets can be pulled from it without raising an error. The gate must
     * support pulling packets.
     */
    // TODO: change name and result to: ANY, SOME, NONE
    virtual bool canPullSomePacket(cGate *gate = nullptr) const = 0;

    /**
     * Returns the packet that can be pulled at the given gate. The returned
     * value may be nullptr if there is no such packet. The gate must support
     * pulling packets.
     */
    virtual Packet *canPullPacket(cGate *gate = nullptr) const = 0;

    /**
     * Pulls a packet from the packet source at the given gate. This operation
     * pulls the packet all at once. The source must not be empty at the given
     * gate. The returned packet is never nullptr, and the gate must support
     * pulling and passing packets.
     */
    virtual Packet *pullPacket(cGate *gate = nullptr) = 0;

    /**
     * Starts pulling the packet from the packet source at the given gate. This is
     * a streaming operation. The source must not be empty at the gate. The gate
     * must support pulling and streaming packets and the returned packet is
     * never nullptr.
     */
    virtual Packet *pullPacketStart(cGate *gate = nullptr) = 0;

    /**
     * Ends pulling the packet from the packet source at the given gate. This is
     * a streaming operation. The source must not be empty at the gate. The gate
     * must support pulling and streaming packets and the returned packet is
     * never nullptr.
     */
    virtual Packet *pullPacketEnd(cGate *gate = nullptr) = 0;

    /**
     * Progresses pulling the packet from the packet source at the given gate.
     * This is a streaming operation. The position specifies where the streaming
     * is at the moment. The extra length partially fixes the future of the
     * streaming operation. The source must not be empty at the gate. The gate
     * must support pulling and streaming packets and the returned packet is
     * never nullptr.
     */
    virtual Packet *pullPacketProgress(b& position, b& extraProcessableLength, cGate *gate = nullptr) = 0;

    /**
     * Returns the processed length of the streamed packet. The packet must not
     * be nullptr and the gate must support pulling and streaming packets.
     */
    virtual b getPulledPacketProcessedLength(Packet *packet, cGate *gate = nullptr) = 0;
};

} // namespace queueing
} // namespace inet

#endif // ifndef __INET_IPASSIVEPACKETSOURCE_H

