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

#include "inet/common/ProtocolTag_m.h"
#include "inet/protocol/acknowledgement/Resending.h"

namespace inet {

Define_Module(Resending);

void Resending::initialize(int stage)
{
    PacketPusherBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL)
        numRetries = par("numRetries");
}

void Resending::pushPacket(Packet *packet, cGate *gate)
{
    Enter_Method("pushPacket");
    take(packet);
    handleMessage(packet);
}

void Resending::handleMessage(cMessage *message)
{
    ASSERT(retry == 0);
    packet = check_and_cast<Packet *>(message);
    pushOrSendPacket(packet->dup(), outputGate, consumer);
    retry++;
}

void Resending::handlePushPacketConfirmation(Packet *p, cGate *gate, bool successful)
{
    if (successful || retry == numRetries) {
        producer->handlePushPacketConfirmation(packet, inputGate->getPathStartGate(), successful);
        delete packet;
        packet = nullptr;
        retry = 0;
        producer->handleCanPushPacket(inputGate->getPathStartGate());
    }
    else {
        pushOrSendPacket(packet->dup(), outputGate, consumer);
        retry++;
    }
}

} // namespace inet

