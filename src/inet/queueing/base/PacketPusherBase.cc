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

#include "inet/common/ModuleAccess.h"
#include "inet/common/Simsignals.h"
#include "inet/queueing/base/PacketPusherBase.h"

namespace inet {
namespace queueing {

void PacketPusherBase::initialize(int stage)
{
    PacketProcessorBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        inputGate = gate("in");
        outputGate = gate("out");
        producer = findConnectedModule<IActivePacketSource>(inputGate);
        consumer = findConnectedModule<IPassivePacketSink>(outputGate);
    }
    else if (stage == INITSTAGE_QUEUEING) {
        if (producer != nullptr)
            checkPushPacketSupport(inputGate);
        if (consumer != nullptr)
            checkPushPacketSupport(outputGate);
    }
}

void PacketPusherBase::handleMessage(cMessage *message)
{
    auto packet = check_and_cast<Packet *>(message);
    pushPacket(packet, packet->getArrivalGate());
}

bool PacketPusherBase::canPushSomePacket(cGate *gate) const
{
    return consumer->canPushSomePacket(outputGate->getPathEndGate());
}

bool PacketPusherBase::canPushPacket(Packet *packet, cGate *gate) const
{
    return consumer->canPushPacket(packet, outputGate->getPathEndGate());
}

void PacketPusherBase::handleCanPushPacket(cGate *gate)
{
    Enter_Method("handleCanPushPacket");
    if (producer != nullptr)
        producer->handleCanPushPacket(inputGate);
}

} // namespace queueing
} // namespace inet

