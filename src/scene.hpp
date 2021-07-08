#include <vector>


float map(float current, float min1, float max1, float min2, float max2) {
    return min2 + current / (max1 - min1) * (max2 - min2);
}
