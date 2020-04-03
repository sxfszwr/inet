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

#include "inet/common/IProtocolRegistrationListener.h"
#include "inet/protocol/contract/IProtocol.h"
#include "inet/protocol/fragmentation/FragmentNumberHeader_m.h"
#include "inet/protocol/fragmentation/FragmentNumberHeaderChecker.h"
#include "inet/protocol/fragmentation/FragmentTag_m.h"

namespace inet {

Define_Module(FragmentNumberHeaderChecker);

void FragmentNumberHeaderChecker::initialize(int stage)
{
    PacketFlowBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        headerPosition = parseHeaderPosition(par("headerPosition"));
        registerService(IProtocol::fragmentation, inputGate, inputGate);
        registerProtocol(IProtocol::fragmentation, outputGate, outputGate);
    }
}

void FragmentNumberHeaderChecker::processPacket(Packet *packet)
{
    Ptr<const FragmentNumberHeader> fragmentHeader;
    switch (headerPosition) {
        case HP_FRONT:
            fragmentHeader = packet->popAtFront<FragmentNumberHeader>();
            break;
        case HP_BACK:
            fragmentHeader = packet->popAtBack<FragmentNumberHeader>(B(1));
            break;
        default:
            throw cRuntimeError("Unknown headerPosition parameter value");
    }
    auto fragmentTag = packet->addTag<FragmentTag>();
    fragmentTag->setFirstFragment(fragmentHeader->getFragmentNumber() == 0);
    fragmentTag->setLastFragment(fragmentHeader->getLastFragment());
    fragmentTag->setFragmentNumber(fragmentHeader->getFragmentNumber());
    fragmentTag->setNumFragments(-1);
}

} // namespace inet

