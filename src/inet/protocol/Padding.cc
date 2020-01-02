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

#include "inet/common/packet/chunk/BitCountChunk.h"
#include "inet/protocol/Padding.h"

namespace inet {

Define_Module(Padding);

void Padding::initialize(int stage)
{
    PacketFlowBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        minLength = b(par("minLength"));
    }
}

void Padding::processPacket(Packet *packet)
{
    auto length = packet->getTotalLength();
    if (length < minLength) {
        auto padding = makeShared<BitCountChunk>(minLength - length);
        packet->insertAtBack(padding);
    }
}

} // namespace inet

