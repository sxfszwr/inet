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

#include "inet/queueing/base/PassivePacketSinkBase.h"

namespace inet {
namespace queueing {

void PassivePacketSinkBase::handleMessage(cMessage *message)
{
    auto packet = check_and_cast<Packet *>(message);
    pushPacket(packet, packet->getArrivalGate());
}

void PassivePacketSinkBase::pushPacketStart(Packet *packet, cGate *gate)
{
    pushPacketProgress(packet, b(0), b(0), gate);
}

void PassivePacketSinkBase::pushPacketEnd(Packet *packet, cGate *gate)
{
    pushPacketProgress(packet, packet->getTotalLength(), b(0), gate);
}

} // namespace queueing
} // namespace inet

