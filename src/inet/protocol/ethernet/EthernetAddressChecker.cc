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
#include "inet/linklayer/common/MacAddressTag_m.h"
#include "inet/protocol/ethernet/EthernetAddressChecker.h"
#include "inet/protocol/ethernet/EthernetHeaders_m.h"

namespace inet {

Define_Module(EthernetAddressChecker);

void EthernetAddressChecker::initialize(int stage)
{
    PacketFilterBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        promiscuous = par("promiscuous");
        interfaceEntry = getContainingNicModule(this);
    }
}

bool EthernetAddressChecker::matchesPacket(Packet *packet)
{
    const auto& header = packet->popAtFront<Ieee8023MacAddresses>();
    auto macAddressInd = packet->addTagIfAbsent<MacAddressInd>();
    auto destAddress = header->getDest();
    macAddressInd->setSrcAddress(header->getSrc());
    macAddressInd->setDestAddress(destAddress);
    if (promiscuous || destAddress.isBroadcast())
        return true;
    else if (interfaceEntry != nullptr) {
        if (destAddress.isMulticast())
            // TODO check it in the multicast address list
            return true;
        else
            return interfaceEntry->getMacAddress() == destAddress;
    }
    else
        return false;
}

void EthernetAddressChecker::dropPacket(Packet *packet)
{
    PacketFilterBase::dropPacket(packet, NOT_ADDRESSED_TO_US);
}

} // namespace inet

