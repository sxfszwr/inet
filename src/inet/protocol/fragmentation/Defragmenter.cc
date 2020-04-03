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

#include "inet/protocol/fragmentation/Defragmenter.h"
#include "inet/protocol/fragmentation/FragmentTag_m.h"

namespace inet {

Define_Module(Defragmenter);

void Defragmenter::initialize(int stage)
{
    PacketPusherBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL)
        deleteSelf = par("deleteSelf");
}

void Defragmenter::startDefragmentation(Packet *fragmentPacket)
{
    std::string name = fragmentPacket->getName();
    name = name.substr(0, name.rfind('-'));
    packet = new Packet(name.c_str());
}

void Defragmenter::continueDefragmentation(Packet *fragmentPacket)
{
    packet->insertAtBack(fragmentPacket->peekData());
    expectedFragmentNumber++;
    processedTotalLength += fragmentPacket->getDataLength();
    numProcessedPackets++;
}

void Defragmenter::endDefragmentation(Packet *fragmentPacket)
{
    delete fragmentPacket;
    delete packet;
    packet = nullptr;
    expectedFragmentNumber = 0;
    if (deleteSelf)
        deleteModule();
}

void Defragmenter::pushPacket(Packet *fragmentPacket, cGate *gate)
{
    Enter_Method("pushPacket");
    auto fragmentTag = fragmentPacket->getTag<FragmentTag>();
    if (fragmentTag->getFragmentNumber() != -1 && fragmentTag->getFragmentNumber() != expectedFragmentNumber)
        endDefragmentation(fragmentPacket);
    else {
        if (fragmentTag->getFirstFragment())
            startDefragmentation(fragmentPacket);
        continueDefragmentation(fragmentPacket);
        updateDisplayString();
        if (fragmentTag->getLastFragment()) {
            pushOrSendPacket(packet->dup(), outputGate, consumer);
            endDefragmentation(fragmentPacket);
        }
    }
}

} // namespace inet

