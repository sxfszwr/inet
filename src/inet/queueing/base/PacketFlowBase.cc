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
#include "inet/queueing/base/PacketFlowBase.h"

namespace inet {
namespace queueing {

void PacketFlowBase::initialize(int stage)
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
        if (consumer != nullptr) {
            checkPushPacketSupport(inputGate);
            checkPushPacketSupport(outputGate);
        }
        if (provider != nullptr) {
            checkPopPacketSupport(inputGate);
            checkPopPacketSupport(outputGate);
        }
    }
}

bool PacketFlowBase::canPushSomePacket(cGate *gate) const
{
    return consumer->canPushSomePacket(outputGate->getPathEndGate());
}

bool PacketFlowBase::canPushPacket(Packet *packet, cGate *gate) const
{
    return consumer->canPushPacket(packet, outputGate->getPathEndGate());
}

void PacketFlowBase::pushPacket(Packet *packet, cGate *gate)
{
    Enter_Method("pushPacket");
    processPacket(packet);
    pushOrSendPacket(packet, outputGate, consumer);
    numProcessedPackets++;
    processedTotalLength += packet->getTotalLength();
    updateDisplayString();
}

void PacketFlowBase::pushPacketStart(Packet *packet, cGate *gate)
{
    Enter_Method("pushPacketStart");
    processPacket(packet);
    consumer->pushPacketStart(packet, outputGate->getPathEndGate());
    // TODO: numProcessedPackets? etc.
}

void PacketFlowBase::pushPacketProgress(Packet *packet, b position, b extraProcessableLength, cGate *gate)
{
    Enter_Method("pushPacketProgress");
    processPacket(packet);
    consumer->pushPacketProgress(packet, position, extraProcessableLength, outputGate->getPathEndGate());
    // TODO: numProcessedPackets? etc.
}

void PacketFlowBase::pushPacketEnd(Packet *packet, cGate *gate)
{
    Enter_Method("pushPacketEnd");
    processPacket(packet);
    consumer->pushPacketEnd(packet, outputGate->getPathEndGate());
    // TODO: numProcessedPackets? etc.
}

b PacketFlowBase::getPushedPacketConfirmedLength(Packet *packet, cGate *gate)
{
    return consumer->getPushedPacketConfirmedLength(packet, outputGate->getPathEndGate());
}

void PacketFlowBase::handlePushPacketConfirmation(Packet *packet, cGate *gate, bool successful)
{
    producer->handlePushPacketConfirmation(packet, gate, successful);
}

bool PacketFlowBase::canPopSomePacket(cGate *gate) const
{
    return provider->canPopSomePacket(inputGate->getPathStartGate());
}

Packet *PacketFlowBase::canPopPacket(cGate *gate) const
{
    return provider->canPopPacket(inputGate->getPathStartGate());
}

Packet *PacketFlowBase::popPacket(cGate *gate)
{
    Enter_Method("popPacket");
    auto packet = provider->popPacket(inputGate->getPathStartGate());
    processPacket(packet);
    numProcessedPackets++;
    processedTotalLength += packet->getTotalLength();
    updateDisplayString();
    animateSend(packet, outputGate);
    return packet;
}

void PacketFlowBase::handleCanPushPacket(cGate *gate)
{
    Enter_Method("handleCanPushPacket");
    if (producer != nullptr)
        producer->handleCanPushPacket(inputGate);
}

void PacketFlowBase::handleCanPopPacket(cGate *gate)
{
    Enter_Method("handleCanPopPacket");
    if (collector != nullptr)
        collector->handleCanPopPacket(outputGate);
}

} // namespace queueing
} // namespace inet

