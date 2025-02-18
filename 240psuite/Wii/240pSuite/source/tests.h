/* 
 * 240p Test Suite
 * Copyright (C)2011 Artemio Urbina
 *
 * This file is part of the 240p Test Suite
 *
 * The 240p Test Suite is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The 240p Test Suite is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef TESTS_H
#define TESTS_H

#include "image.h"
void DropShadowTest();
void StripedSpriteTest();
void TimingReflexTest();
void ScrollTest();
void GridScrollTest();
void DrawStripes();
void DrawCheckBoard();
void SoundTest();
void AudioSyncTest();
void LEDZoneTest();
void PassiveLagTest();
void Alternate240p480i();
void TestVideoMode();
void DiagonalPatternTest();
void AudioEquipmentTest(ImagePtr back);

#endif

