#pragma once

// Definition der Pins
#include "pindef.h"

#define NUM_SEGMENTS 12 // Anzahl der schwarzen und weißen Segmente

#define DROPDELAY 55000  // Gesamtverzögerung Auslösen - Aufprall in Microsekunden
#define DROPTARGETPOS 50  // Position des Abwurfzieles innerhalb des Segmentes in Prozent
#define HOLEPOS 0 // Nummer des Segmentes mit dem Loch

class DiscMonitor
{
  public:
    void registerPhotoTransition(); // verwurschteln des Photosensors
    void registerAbsolutePosition(); // verwurschteln des Hallsensors
    unsigned long getExpectedSegmentTime(int segment, int rev_ahead); // geschaetzte Dauer des Segments
    bool getRecommendedTriggerPos(int *segment, unsigned long *triggerdelay); // Triggerposition bestimmen (wann soll die Kugel fallen gelassen werden)

    volatile int currentPosition = 0; 	// Wirklich die derzeitige Position - also das Segment, das als nächstes verlassen wird
										//Index Null = das Segment mit der Markierung der Absolutposition
    volatile unsigned long lastImpulseMicros = 0;
    volatile unsigned long segmentTimes[NUM_SEGMENTS] = {0};
    volatile bool absEdgeRegistered = false;
};
