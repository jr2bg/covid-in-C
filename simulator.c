#include <stdio.h>
#include <stdlib.h>

#include "lib.h"
#include "single_evolution.h"

typedef unsigned char ubyte;
typedef unsigned int uint;
// Y-coordinates y0, y1 and y2 are already pre-multiplied with world width.
static inline ubyte countAliveCells(ubyte* m_data,size_t x0, size_t x1, size_t x2,
    size_t y0, size_t y1, size_t y2) {
return m_data[x0 + y0] + m_data[x1 + y0] + m_data[x2 + y0]
    + m_data[x0 + y1] + m_data[x2 + y1]
    + m_data[x0 + y2] + m_data[x1 + y2] + m_data[x2 + y2];
}

int computeIterationSerial(
    ubyte* m_data,
    ubyte* m_resultData,
    size_t m_worldWidth,
    size_t m_worldHeight,
    size_t m_dataLength
) {
    ubyte temp;
    // use linear distribution of 2D array
	for (size_t y = 0; y < m_worldHeight; ++y) {
        // y positions of the neighbourhood using 1D array
		size_t y0 = ((y + m_worldHeight - 1) % m_worldHeight) * m_worldWidth;
		size_t y1 = y * m_worldWidth;
		size_t y2 = ((y + 1) % m_worldHeight) * m_worldWidth;

		for (size_t x = 0; x < m_worldWidth; ++x) {
			size_t x0 = (x + m_worldWidth - 1) % m_worldWidth;
			size_t x2 = (x + 1) % m_worldWidth;

			ubyte aliveCells = countAliveCells(m_data, x0, x, x2, y0, y1, y2);
			m_resultData[y1 + x] =
				aliveCells == 3 || (aliveCells == 2 && m_data[x + y1]) ? 1 : 0;
		}
	}

    // swap the variables
    temp = *m_data;
    *m_data = *m_resultData;
    *m_resultData = temp;

    return 0;
}

int main(int argc, char *argv[]){
    // at most three entries, model and cloud and help
    
    ubyte* m_data;
    ubyte* m_resultData;

    size_t m_worldWidth = 1000;
    size_t m_worldHeight = 1000;
    uint total_iterations = 1000;
    size_t m_dataLength = m_worldWidth * m_worldHeight;

    // allocate memory
    m_data = (ubyte *)malloc(sizeof(ubyte *) * m_dataLength);
    m_resultData = (ubyte *)malloc(sizeof(ubyte *) * m_dataLength);

    // iterations
    for (uint i = 0; i < total_iterations; i++){
        computeIterationSerial(
            m_data,
            m_resultData,
            m_worldWidth,
            m_worldHeight,
            m_dataLength
        );
        if (i % 100 == 0) {
            printf("%d iteration\n", i);
        }
    }

    // deallocate memory
    free(m_data);
    free(m_resultData);

    printf("Hello world\n");
    return 0;
}