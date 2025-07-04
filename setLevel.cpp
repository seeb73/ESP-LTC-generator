/*
 @version 0.0.1
 @author Sebastian Pachla
 @package esp32 LTC timecode generator
 Do testów zoptymalizowana wersja.
 An optimised version for testing.
 Wersja do testów. Oryginalny kod po kompilacji zajmował dużo miejsca w pamięci IRAM, co dość mocno ograniczało funkcjonalność modułu.
 Test version. The original code took up a lot of space in IRAM after compilation, which limited the functionality quite a bit.
*/

void IRAM_ATTR setLevel(void)
{
    int dutyCycle;
    int bitSource = 0;
    int bitIndex = 0;

    if (bitCount <= 3) { // frameCount, jedności
        bitSource = frameCount % 10;
        bitIndex = bitCount;
    } 
    else if (bitCount == 8 || bitCount == 9) { // dziesiątki frameCount
        bitSource = (frameCount / 10) % 10;
        bitIndex = bitCount - 8;
    } 
    else if (bitCount >= 16 && bitCount <= 19) { // secondCount, jedności
        bitSource = secondCount % 10;
        bitIndex = bitCount - 16;
    }
    else if (bitCount >= 24 && bitCount <= 26) { // secondCount, dziesiątki
        bitSource = (secondCount / 10) % 10;
        bitIndex = bitCount - 24;
    }
    else if (bitCount >= 32 && bitCount <= 35) { // minuteCount, jedności
        bitSource = minuteCount % 10;
        bitIndex = bitCount - 32;
    }
    else if (bitCount >= 40 && bitCount <= 41) { // minuteCount, dziesiątki
        bitSource = (minuteCount / 10) % 10;
        bitIndex = bitCount - 40;
    }
    else {
        currentBit = 0; // W pozostałych przypadkach zawsze 0
    }

    if (bitCount != 4 && bitCount != 5 && bitCount != 6 && bitCount != 7 &&
        bitCount != 10 && bitCount != 11 && bitCount != 12 && bitCount != 13 &&
        bitCount != 14 && bitCount != 15 && bitCount != 20 && bitCount != 21 &&
        bitCount != 22 && bitCount != 23 && bitCount != 27 && bitCount != 28 &&
        bitCount != 29 && bitCount != 30 && bitCount != 31 && bitCount != 36 &&
        bitCount != 37 && bitCount != 38 && bitCount != 39) 
    {
        currentBit = (bitSource >> bitIndex) & 1;
    }

    // Obliczenie dutyCycle
    lastLevel = updateCnt ? (currentBit ? !lastLevel : lastLevel) : !lastLevel;
    dutyCycle = lastLevel ? highLevel : lowLevel;

    ledcWrite(0, dutyCycle);

    if (updateCnt && !currentBit) {
        polarBit = !polarBit;
    }
}
