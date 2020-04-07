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

#include "inet/common/checksum/EthernetCRC.h"
#include "inet/linklayer/ethernet/EtherFrame_m.h"
#include "inet/protocol/ethernet/EthernetFragmentFcsChecker.h"
#include "inet/protocol/fragmentation/tag/FragmentTag_m.h"

namespace inet {

Define_Module(EthernetFragmentFcsChecker);

bool EthernetFragmentFcsChecker::checkComputedFcs(const Packet *packet, uint32_t receivedFcs) const
{
    auto data = packet->peekDataAsBytes();
    auto bytes = data->getBytes();
    uint32_t fragmentFcs = ethernetCRC(bytes.data(), packet->getByteLength());
    auto fragmentTag = packet->getTag<FragmentTag>();
    if (fragmentTag->getFirstFragment())
        completeFcs = 0;
    completeFcs = ethernetCRC(bytes.data(), packet->getByteLength(), completeFcs);
    bool lastFragment = receivedFcs != (fragmentFcs ^ 0xFFFF0000);
    fragmentTag->setLastFragment(lastFragment);
    return !lastFragment || receivedFcs == completeFcs;
}

bool EthernetFragmentFcsChecker::checkFcs(const Packet *packet, FcsMode fcsMode, uint32_t fcs) const
{
    switch (fcsMode) {
        case FCS_DECLARED_CORRECT:
            return checkDeclaredCorrectFcs(packet, fcs);
        case FCS_DECLARED_INCORRECT:
            return checkDeclaredIncorrectFcs(packet, fcs);
        case FCS_COMPUTED:
            return checkComputedFcs(packet, fcs);
        default:
            throw cRuntimeError("Unknown FCS mode");
    }
}

bool EthernetFragmentFcsChecker::matchesPacket(Packet *packet)
{
    const auto& header = packet->popAtBack<EthernetFragmentFcs>(B(4));
    auto fragmentTag = packet->getTag<FragmentTag>();
    fragmentTag->setLastFragment(!header->getMCrc());
    auto fcsMode = header->getFcsMode();
    auto fcs = header->getFcs();
    return checkFcs(packet, fcsMode, fcs);
}

void EthernetFragmentFcsChecker::dropPacket(Packet *packet)
{
    PacketFilterBase::dropPacket(packet, INCORRECTLY_RECEIVED);
}

} // namespace inet

