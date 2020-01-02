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

#include "inet/protocol/common/PacketDestreamer.h"

namespace inet {

Define_Module(PacketDestreamer);

Packet *PacketDestreamer::pullPacket(cGate *gate)
{
    Enter_Method("pullPacket");
    auto inputPathStartGate = inputGate->getPathStartGate();
    auto packet = provider->pullPacketStart(inputPathStartGate);
    packet = provider->pullPacketEnd(inputPathStartGate);
    return packet;
}

Packet *PacketDestreamer::pullPacketStart(cGate *gate)
{
    throw cRuntimeError("Invalid operation");
}

Packet *PacketDestreamer::pullPacketEnd(cGate *gate)
{
    throw cRuntimeError("Invalid operation");
}

Packet *PacketDestreamer::pullPacketProgress(b& position, b& extraProcessableLength, cGate *gate)
{
    throw cRuntimeError("Invalid operation");
}

b PacketDestreamer::getPulledPacketProcessedLength(Packet *packet, cGate *gate)
{
    throw cRuntimeError("Invalid operation");
}

void PacketDestreamer::handlePullPacketConfirmation(Packet *packet, cGate *gate, bool successful)
{
    throw cRuntimeError("Invalid operation");
}

} // namespace inet

