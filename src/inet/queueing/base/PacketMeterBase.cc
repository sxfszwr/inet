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
#include "inet/queueing/base/PacketMeterBase.h"
#include "inet/common/Simsignals.h"

namespace inet {
namespace queueing {

void PacketMeterBase::initialize(int stage)
{
    PacketProcessorBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        inputGate = gate("in");
        outputGate = gate("out");
        producer = findConnectedModule<IActivePacketSource>(inputGate);
        collector = findConnectedModule<IActivePacketSink>(outputGate);
        provider = findConnectedModule<IPassivePacketSource>(inputGate);
        consumer = findConnectedModule<IPassivePacketSink>(outputGate);
    }
    else if (stage == INITSTAGE_QUEUEING) {
        checkPackingPushingOrPullingSupport(inputGate);
        checkPackingPushingOrPullingSupport(outputGate);
    }
}

void PacketMeterBase::pushPacket(Packet *packet, cGate *gate)
{
    Enter_Method("pushPacket");
    EV_INFO << "Metering packet " << packet->getName() << "." << endl;
    meterPacket(packet);
    pushOrSendPacket(packet, outputGate, consumer);
    numProcessedPackets++;
    processedTotalLength += packet->getTotalLength();
    updateDisplayString();
}

bool PacketMeterBase::canPullSomePacket(cGate *gate) const
{
    return provider->canPullPacket(inputGate->getPathStartGate());
}

Packet *PacketMeterBase::pullPacket(cGate *gate)
{
    Enter_Method("pullPacket");
    auto packet = provider->pullPacket(inputGate->getPathStartGate());
    EV_INFO << "Metering packet " << packet->getName() << "." << endl;
    meterPacket(packet);
    numProcessedPackets++;
    processedTotalLength += packet->getTotalLength();
    updateDisplayString();
    animateSend(packet, outputGate);
    return packet;
}

void PacketMeterBase::handleCanPushPacket(cGate *gate)
{
    Enter_Method("handleCanPushPacket");
    if (producer != nullptr)
        producer->handleCanPushPacket(inputGate);
}

void PacketMeterBase::handleCanPullPacket(cGate *gate)
{
    Enter_Method("handleCanPullPacket");
    if (collector != nullptr)
        collector->handleCanPullPacket(outputGate);
}

} // namespace queueing
} // namespace inet

