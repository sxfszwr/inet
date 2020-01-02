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

#include "inet/protocol/common/PacketStreamer.h"

namespace inet {

Define_Module(PacketStreamer);

void PacketStreamer::pushPacket(Packet *packet, cGate *gate)
{
    Enter_Method("pushPacket");
    pushOrSendPacketStart(packet, gate, consumer);
    pushOrSendPacketEnd(packet, gate, consumer);
}

b PacketStreamer::getPushedPacketProcessedLength(Packet *packet, cGate *gate)
{
    throw cRuntimeError("Invalid operation");
}

void PacketStreamer::handlePushPacketConfirmation(Packet *packet, cGate *gate, bool successful)
{
    producer->handlePushPacketConfirmation(packet, inputGate->getPathStartGate(), successful);
}

} // namespace inet

