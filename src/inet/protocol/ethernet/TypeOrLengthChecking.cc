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

#include "inet/common/ProtocolGroup.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/linklayer/common/InterfaceTag_m.h"
#include "inet/networklayer/common/InterfaceEntry.h"
#include "inet/protocol/ethernet/EthernetHeaders_m.h"
#include "inet/protocol/ethernet/TypeOrLengthChecking.h"

namespace inet {

Define_Module(TypeOrLengthChecking);

void TypeOrLengthChecking::initialize(int stage)
{
    PacketFilterBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
    }
}

bool TypeOrLengthChecking::matchesPacket(Packet *packet)
{
    const auto& header = packet->popAtFront<Ieee8023TypeOrLength>();
    auto protocol = ProtocolGroup::ethertype.findProtocol(header->getTypeOrLength());
    packet->addTagIfAbsent<PacketProtocolTag>()->setProtocol(protocol);
    packet->addTagIfAbsent<DispatchProtocolReq>()->setProtocol(protocol);
    auto interfaceEntry = check_and_cast<InterfaceEntry *>(getParentModule());
    packet->addTagIfAbsent<InterfaceInd>()->setInterfaceId(interfaceEntry->getInterfaceId());
    return true;
}

} // namespace inet

